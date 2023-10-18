#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "shared_defs.h"

#define PLAYPAUSE_PIN D7
#define AMP_POW_PIN D8
#define PLAYBACK_MON_PIN D2

static uint8_t disco_mac[6] = {0xa4,0xe5,0x7c,0xbc,0xdb,0xe9};

static volatile bool playback = false;
static volatile bool triggered = false;
static volatile bool send_discotime = false;

void playpause() {
	digitalWrite(PLAYPAUSE_PIN, HIGH);
	delay(100);
	digitalWrite(PLAYPAUSE_PIN, LOW);
}

void do_discotime_message() {
	uint8_t op = OP_DISCOTIME;
	esp_now_send(disco_mac, &op, /* length: */ 1);
}

IRAM_ATTR void playback_change() {
	playback = !playback;

	if (playback && triggered) {
		digitalWrite(AMP_POW_PIN, HIGH);
		if (send_discotime) {
			delay(500);
			do_discotime_message();
			send_discotime = false;
		}
	} else {
		digitalWrite(AMP_POW_PIN, LOW);
	}
}

void handle_op(uint8_t op) {
	switch (op) {
		case OP_PLAY_MUSIC_WITH_DISCOTIME:
			if (!triggered)
				send_discotime = true;
			// case fallthrough intentional...
		case OP_PLAY_MUSIC:
			if (!triggered) {
				triggered = true;
				if (playback) {
					digitalWrite(AMP_POW_PIN, HIGH);
					if (send_discotime) {
						do_discotime_message();
						send_discotime = false;
					}
				} else {
					playpause();
				}
			}
			break;
		case OP_PAUSE_MUSIC:
			if (triggered) {
				triggered = false;
				if (playback)
					playpause();
			}
			break;
		default:
			// unsupported op
			break;
	}
}

void recv_callback(uint8_t *mac, uint8_t *data, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		handle_op(data[i]);
	}
}

void setup() {
	pinMode(PLAYPAUSE_PIN, OUTPUT);
	pinMode(AMP_POW_PIN, OUTPUT);

	digitalWrite(PLAYPAUSE_PIN, LOW);
	digitalWrite(AMP_POW_PIN, LOW);

	attachInterrupt(
		digitalPinToInterrupt(PLAYBACK_MON_PIN),
		playback_change,
		CHANGE
	);

	WiFi.mode(WIFI_STA);
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
	esp_now_add_peer(disco_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
	esp_now_register_recv_cb(recv_callback);
}

void loop() {
}
