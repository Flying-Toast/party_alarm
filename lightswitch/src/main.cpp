#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "shared_defs.h"

#define RELAY_PIN 12

void handle_op(uint8_t op) {
	switch (op) {
		case OP_LIGHTS_ON:
			digitalWrite(RELAY_PIN, HIGH);
			break;
		case OP_LIGHTS_OFF:
			digitalWrite(RELAY_PIN, LOW);
			break;
		default:
			// unknown op
			break;
	}
}

void recv_callback(uint8_t *mac, uint8_t *data, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		handle_op(data[i]);
	}
}

void setup() {
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, HIGH);

	WiFi.mode(WIFI_STA);
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(recv_callback);
}

void loop() {}
