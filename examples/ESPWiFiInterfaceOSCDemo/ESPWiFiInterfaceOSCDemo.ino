#include <ESPWiFiInterfaceOSC.h>
#include "Blinker.h"

ESPWiFiInterfaceOSC wifi;
Blinker blinker;

const char *staSsid = "XXXX-147e";
const char *staPass = "phewvomnamregcuvyeb2";

// AP password must be at least 8 characters long, otherwise AP never starts 
// const char *apSsid = "yabadabadooo";
// const char *apPass = "yahooooo";

OSCMessage msg("/yo");

void setup() {
  Serial.begin(115200);

  blinker.setLow();
  blinker.update();

  wifi.enableDHCP(true);
  wifi.setHostIP(IPAddress(192, 168, 0, 12));

  msg.add(42);

  delay(5000);

  wifi.setConnectionStateListener([&](WiFiConnectionState s) {
    switch (s) {
      case WiFiDisconnected: {
          Serial.println("disconnected");
          blinker.setLow();
          blinker.update();
        }
        break;

      case WiFiConnecting: {
          Serial.println("connecting");
          blinker.setPeriod(100);
          blinker.update();
        }
        break;

      case WiFiConnected: {
          Serial.println("connected");
          blinker.setHigh();
          blinker.update();
        }
        break;
    }
  });

  wifi.startWiFi(WIFI_STA, staSsid, staPass);
}

void loop() {
  wifi.update();
  blinker.update();

  wifi.sendUdpOSCMessage(msg);
  delay(100);
}