#include "ESPWiFiInterfaceBase.h"

#ifdef ESP32
// how to read mac address, found here :
// https://github.com/espressif/arduino-esp32/issues/932
#include "esp_system.h"
#endif

ESPWiFiInterfaceBase::ESPWiFiInterfaceBase() :
mode(WIFI_OFF),
#ifdef ESP8266
alreadyStarted(false),
#endif
accessPointRunning(false),
dhcpOn(true),
connectionState(WiFiDisconnected),
connectionStateCallback(NULL) {
  // erase stored credentials
  WiFi.disconnect();
  WiFi.softAPdisconnect();
  setStaticIP(IPAddress(192, 168, 0, 1));
}

ESPWiFiInterfaceBase::~ESPWiFiInterfaceBase() {}

void
ESPWiFiInterfaceBase::startWiFi(WiFiMode_t mode,
                                const char *ssid, const char *pass) {
  if (mode != WIFI_STA && mode != WIFI_AP) return;

  connectionState = WiFiConnecting;
  if (connectionStateCallback != NULL) {
    connectionStateCallback(connectionState);
  }

  this->mode = mode;
  WiFi.mode(mode);

#ifdef ESP8266
  if (alreadyStarted) {
    WiFi.forceSleepWake();
  } else {
    alreadyStarted = true;
  }
#endif

  // to turn back dhcp on, see :
  // https://stackoverflow.com/questions/40069654/how-to-clear-static-ip-configuration-and-start-dhcp

  switch (mode) {
    case WIFI_STA: {
        if (dhcpOn) {
          WiFi.config(0u, 0u, 0u);
        } else {
          WiFi.config(staticIP, gatewayIP, subnetIP);
        }

        WiFi.begin(ssid, pass);
      }
      break;

    case WIFI_AP: {
        if (dhcpOn) {
          WiFi.softAPConfig(0u, 0u, 0u);
        } else {
          WiFi.softAPConfig(staticIP, gatewayIP, subnetIP);
        }

        if (WiFi.softAP(ssid, pass)) {
          accessPointRunning = true;

          connectionState = WiFiConnected;
          if (connectionStateCallback != NULL) {
            connectionStateCallback(connectionState);
          }
        }
      }
      break;

    default:
      break;
  }
}

void
ESPWiFiInterfaceBase::stopWiFi() {
  WiFi.disconnect();
  WiFi.softAPdisconnect();
  accessPointRunning = false;

  WiFi.mode(WIFI_OFF);
  
#ifdef ESP8266
  if (alreadyStarted) {
    WiFi.forceSleepBegin();
  }
#endif

  connectionState = WiFiDisconnected;
  if (connectionStateCallback != NULL) {
    connectionStateCallback(connectionState);
  }
}

void
ESPWiFiInterfaceBase::update() {
  if (connectionState == WiFiConnecting && isActive()) {
    connectionState = WiFiConnected;
    if (connectionStateCallback != NULL) {
      connectionStateCallback(connectionState);
    }
  }
}

WiFiMode_t
ESPWiFiInterfaceBase::getMode() {
  return mode;
}

void
ESPWiFiInterfaceBase::setConnectionStateListener(std::function<void(WiFiConnectionState)> callback) {
  connectionStateCallback = callback;
}

void
ESPWiFiInterfaceBase::enableDHCP(bool e) {
  dhcpOn = e;
}

void
ESPWiFiInterfaceBase::setStaticIP(IPAddress ip, bool autoConfig) {
  staticIP = ip;

  if (autoConfig) {
    gatewayIP = ip;
    gatewayIP[3] = 1;
    subnetIP = IPAddress(255, 255, 255, 0);
  }
}

void
ESPWiFiInterfaceBase::setGatewayIP(IPAddress ip) {
  gatewayIP = ip;
}

void
ESPWiFiInterfaceBase::setSubnetIP(IPAddress ip) {
  subnetIP = ip;
}

void
ESPWiFiInterfaceBase::getMacAddress(uint8_t *mac) {
#ifdef ESP8266
  WiFi.macAddress(static_cast<byte *>(mac));
#elif defined(ESP32)
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
#endif
}

bool
ESPWiFiInterfaceBase::isActive() {
  return WiFi.status() == WL_CONNECTED || accessPointRunning;
}
