#include <ESPWiFiInterfaceWebServer.h>
#include "configWebPage.h"

ESPWiFiInterfaceWebServer wifi;

void setup() {
  wifi.enableDHCP(false);
  wifi.setStaticIP(IPAddress(192, 168, 1, 1));
  wifi.setRootWebPage(configWebPage);

  wifi.setSocketTextMessageListener([&](uint8_t num, const char *message, size_t length) {
    if (strcmp(message, "hello") == 0) {
      wifi.sendTextMessageToSocket(num, "hello");
    }
  });

  wifi.startWiFi(WIFI_AP, "my-esp-server");
}

void loop() {
  wifi.update();
}