#ifndef _ESP_WIFI_INTERFACE_WEB_SERVER_H_
#define _ESP_WIFI_INTERFACE_WEB_SERVER_H_

#ifdef ESP8266
#include <ESP8266WebServer.h>
#elif defined(ESP32)
#include <WebServer.h>
#endif

#include <WebSocketsServer.h>
#include "ESPWiFiInterfaceBase.h"

class ESPWiFiInterfaceWebServer : public ESPWiFiInterfaceBase {
private:

#ifdef ESP8266
  ESP8266WebServer *webServer;
#elif defined(ESP32)
  WebServer *webServer;
#endif

  WebSocketsServer *socketServer;

  const char *rootWebPage;
  void (*socketConnectionListener)(uint8_t num);
  void (*socketDisconnectionListener)(uint8_t num);
  void (*socketTextMessageListener)(uint8_t num, const char *message, size_t length);

public:
  ESPWiFiInterfaceWebServer(int webPort = 80, int socketPort = 81);
  ~ESPWiFiInterfaceWebServer();

  void startWiFi(WiFiMode_t mode, const char *ssid, const char *pass = "");
  void stopWiFi();
  void update();

  void setRootWebPage(const char *page);
  void setSocketConnectionListener(void (*callback)(uint8_t num));
  void setSocketDisconnectionListener(void (*callback)(uint8_t num));
  void setSocketTextMessageListener(void (*callback)(uint8_t num, const char *message, size_t length));
  void sendTextMessageToSocket(uint8_t num, const char *message);
  void broadcastTextMessageToSockets(const char *message);

private:
  void onSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
};

#endif /* _ESP_WIFI_INTERFACE_WEB_SERVER_H_ */
