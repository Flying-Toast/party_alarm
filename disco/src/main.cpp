#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "shared_defs.h"

#define MOTOR_PIN 2
#define RED_PIN 15
#define GREEN_PIN 13
#define BLUE_PIN 4

static volatile bool keep_partying = false;
static volatile bool needs_kill = false;

void kill_party() {
	digitalWrite(RED_PIN, LOW);
	digitalWrite(GREEN_PIN, LOW);
	digitalWrite(BLUE_PIN, LOW);
	digitalWrite(MOTOR_PIN, LOW);
}

void do_discotime() {
	digitalWrite(MOTOR_PIN, HIGH);
	digitalWrite(RED_PIN, HIGH);
	digitalWrite(GREEN_PIN, HIGH);
	digitalWrite(BLUE_PIN, LOW);
	keep_partying = true;
}

void handle_op(uint8_t op) {
	switch (op) {
		case OP_DISCOTIME:
			do_discotime();
			break;
		case OP_STOP_DISCOTIME:
			needs_kill = true;
			keep_partying = false;
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
	pinMode(RED_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);
	pinMode(BLUE_PIN, OUTPUT);
	pinMode(MOTOR_PIN, OUTPUT);

	kill_party();

	WiFi.mode(WIFI_STA);
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
	esp_now_register_recv_cb(recv_callback);
}

void loop() {
	while (keep_partying) {
		digitalWrite(RED_PIN, LOW);
		digitalWrite(BLUE_PIN, HIGH);
		delay(625);
		digitalWrite(RED_PIN, HIGH);
		digitalWrite(GREEN_PIN, LOW);
		delay(625);
		digitalWrite(GREEN_PIN, HIGH);
		digitalWrite(BLUE_PIN, LOW);
		delay(625);
	}

	if (needs_kill) {
		kill_party();
		needs_kill = false;
	}
}
