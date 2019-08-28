#ifndef _ESP_WIFI_INTERFACE_WEB_SERVER_H_
#define _ESP_WIFI_INTERFACE_WEB_SERVER_H_

#include "ESPWiFiInterfaceBase.h"
#include <WebSocketsServer.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#elif defined(ESP32)
#include <WebServer.h>
#endif

class ESPWiFiInterfaceWebServer : public ESPWiFiInterfaceBase {
private:

#ifdef ESP8266
  ESP8266WebServer *webServer;
#elif defined(ESP32)
  WebServer *webServer;
#endif

  WebSocketsServer *socketServer;

  const char *rootWebPage;
  
  std::function<void(uint8_t)> socketConnectionListener;
  std::function<void(uint8_t)> socketDisconnectionListener;
  std::function<void(uint8_t, const char *, size_t)> socketTextMessageListener;

public:
  ESPWiFiInterfaceWebServer(int webPort = 80, int socketPort = 81);
  ~ESPWiFiInterfaceWebServer();

  void startWiFi(WiFiMode_t mode, const char *ssid, const char *pass = "");
  void stopWiFi();
  void update();

  void setRootWebPage(const char *page);

  void setSocketConnectionListener(std::function<void(uint8_t)> callback);
  void setSocketDisconnectionListener(std::function<void(uint8_t)> callback);
  void setSocketTextMessageListener(std::function<void(uint8_t, const char *, size_t)> callback);

  void sendTextMessageToSocket(uint8_t num, const char *message);
  void broadcastTextMessageToSockets(const char *message);

private:
  void onSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
};

#endif /* _ESP_WIFI_INTERFACE_WEB_SERVER_H_ */
