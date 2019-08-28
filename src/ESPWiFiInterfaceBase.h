#ifndef _ESP_WIFI_INTERFACE_BASE_H_
#define _ESP_WIFI_INTERFACE_BASE_H_

#define MAX_ESP_WIFI_INTERFACE_STRING_SIZE 120

enum WiFiConnectionState {
  WiFiDisconnected = 0,
  WiFiConnecting,
  WiFiConnected
};

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

class ESPWiFiInterfaceBase {
private:
  WiFiMode_t mode;

  IPAddress staticIP;
  IPAddress gatewayIP;
  IPAddress subnetIP;

#ifdef ESP8266
  bool alreadyStarted;
#endif

  bool accessPointRunning;
  bool dhcpOn;

protected:
  WiFiConnectionState connectionState;  
  std::function<void(WiFiConnectionState)> connectionStateCallback;

public:
  ESPWiFiInterfaceBase();
  virtual ~ESPWiFiInterfaceBase();

  virtual void startWiFi(WiFiMode_t mode, const char *ssid, const char *pass = "");
  virtual void stopWiFi();
  virtual void update();

  virtual WiFiMode_t getMode();
  virtual void setConnectionStateListener(std::function<void(WiFiConnectionState)> callback);

  virtual void enableDHCP(bool e);
  virtual void setStaticIP(IPAddress ip, bool autoConfig = true);
  virtual void setGatewayIP(IPAddress ip);
  virtual void setSubnetIP(IPAddress ip);
  virtual void getMacAddress(uint8_t *mac); // mac should be a uint8_t array of length 6

protected:
  bool isActive();
};

#endif /* _ESP_WIFI_INTERFACE_BASE_H_ */