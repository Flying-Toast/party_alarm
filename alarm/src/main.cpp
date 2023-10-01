#include <Arduino.h>
#include <user_interface.h>
#include <espnow.h>
#include "shared_defs.h"

#define PARTY_STOP_PIN D2

// this is a weak symbol in esp8266ArduinoCore code, override it so we save time by not disabling WiFi
extern "C" void __disableWiFiAtBootTime(void) {}

static uint8_t disco_mac[6] = {0xa4,0xe5,0x7c,0xbc,0xdb,0xe9};

IRAM_ATTR void stop_party() {
	uint8_t op = OP_STOP_DISCOTIME;
	esp_now_send(disco_mac, &op, /* length: */ 1);
	// delay to prevent re-interrupting from button bounces
	delay(100);
}

extern "C" void setup() {
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	esp_now_add_peer(disco_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

	uint8_t op = OP_DISCOTIME;
	esp_now_send(disco_mac, &op, /* length: */ 1);

	pinMode(PARTY_STOP_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(PARTY_STOP_PIN), stop_party, RISING);
}

extern "C" void loop() {
}
