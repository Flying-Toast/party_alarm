#include <user_interface.h>
#include <espnow.h>
#include "shared_defs.h"

// this is a weak symbol in esp8266ArduinoCore code, override it so we save time by not disabling WiFi
extern "C" void __disableWiFiAtBootTime(void) {}

static uint8_t disco_mac[6] = {0xa4,0xe5,0x7c,0xbc,0xdb,0xe9};

extern "C" void setup() {
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	esp_now_add_peer(disco_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

	uint8_t op = OP_DISCOTIME;
	esp_now_send(disco_mac, &op, /* length: */ 1);
}

extern "C" void loop() {
}
