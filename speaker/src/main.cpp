#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "shared_defs.h"

#define PLAYPAUSE_PIN D7
#define AMP_POW_PIN D8

static volatile bool playing = false;

void handle_op(uint8_t op) {
	switch (op) {
		case OP_PLAYPAUSE_MUSIC:
			digitalWrite(PLAYPAUSE_PIN, HIGH);
			delay(100);
			digitalWrite(PLAYPAUSE_PIN, LOW);

			playing = !playing;
			digitalWrite(AMP_POW_PIN, playing);
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

	WiFi.mode(WIFI_STA);
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(recv_callback);
}

void loop() {
}
