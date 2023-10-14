#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "shared_defs.h"

#define PLAYPAUSE_PIN D7
#define AMP_POW_PIN D8
#define PLAYBACK_MON_PIN D2

static volatile bool amp_powered = false;

IRAM_ATTR void playback_change() {
	amp_powered = !amp_powered;
	digitalWrite(AMP_POW_PIN, amp_powered);
}

void playpause()
{
	digitalWrite(PLAYPAUSE_PIN, HIGH);
	delay(100);
	digitalWrite(PLAYPAUSE_PIN, LOW);
}

void handle_op(uint8_t op) {
	switch (op) {
		case OP_PLAY_MUSIC:
			if (!amp_powered)
				playpause();
			break;
		case OP_PAUSE_MUSIC:
			if (amp_powered)
				playpause();
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
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(recv_callback);
}

void loop() {
}
