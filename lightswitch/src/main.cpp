#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "shared_defs.h"

void handle_op(uint8_t op) {
	switch (op) {
		case OP_LIGHTS_OFF:
			Serial.println("Turning lights off...");
			break;
		case OP_DISCOTIME:
			Serial.println("It's disco time!");
			break;
		case OP_PLAY_MUSIC:
			Serial.println("MUSIC PLAYING...");
			break;
		default:
			Serial.print("Unknown packet op: ");
			Serial.println(op);
			break;
	}
}

void recv_callback(uint8_t *mac, uint8_t *data, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		handle_op(data[i]);
	}
}

void setup() {
	Serial.begin(9600);
	WiFi.mode(WIFI_STA);
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(recv_callback);
}

void loop() {
}
