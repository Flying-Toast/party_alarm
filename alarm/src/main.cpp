#include <user_interface.h>
#include <espnow.h>
#include "shared_defs.h"

// this is a weak symbol in esp8266ArduinoCore code, override it so we save time by not disabling WiFi
extern "C" void __disableWiFiAtBootTime(void) {}

static uint8_t lightswitch_mac[6] = {0x84,0xf3,0xeb,0xb2,0x32,0x9e};

extern "C" void setup() {
	esp_now_init();
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	esp_now_add_peer(lightswitch_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

	uint8_t ops[] = {OP_LIGHTS_OFF, OP_DISCOTIME, OP_PLAY_MUSIC};
	esp_now_send(lightswitch_mac, ops, sizeof(ops));
}

extern "C" void loop() {
}
