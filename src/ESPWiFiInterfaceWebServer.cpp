#include "ESPWiFiInterfaceWebServer.h"

ESPWiFiInterfaceWebServer::ESPWiFiInterfaceWebServer(int webPort, int socketPort) :
ESPWiFiInterfaceBase(),
rootWebPage(NULL),
socketConnectionListener(NULL),
socketDisconnectionListener(NULL),
socketTextMessageListener(NULL) {

#ifdef ESP8266
  webServer = new ESP8266WebServer(webPort);
#elif defined(ESP32)
  webServer = new WebServer(webPort);
#endif

  webServer->on("/", [&]() {
    if (rootWebPage != NULL) {
      webServer->send(200, "text/html", rootWebPage);
    } else {
      webServer->send(200, "text/plain", "Page not found");
    }
  });

  socketServer = new WebSocketsServer(socketPort);

  socketServer->onEvent(
    [&](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
      onSocketEvent(num, type, payload, length);
    }
  );
}

ESPWiFiInterfaceWebServer::~ESPWiFiInterfaceWebServer() {
  delete webServer;
  delete socketServer;
}

void
ESPWiFiInterfaceWebServer::startWiFi(WiFiMode_t mode,
                                     const char *ssid, const char *pass) {
  ESPWiFiInterfaceBase::startWiFi(mode, ssid, pass);

  webServer->begin();
  socketServer->begin();
}

void
ESPWiFiInterfaceWebServer::stopWiFi() {
  ESPWiFiInterfaceBase::stopWiFi();

  webServer->close();
  webServer->stop();
  socketServer->disconnect();
  socketServer->close();
}

void
ESPWiFiInterfaceWebServer::update() {
  ESPWiFiInterfaceBase::update();

  webServer->handleClient();
  socketServer->loop();
}

void
ESPWiFiInterfaceWebServer::setRootWebPage(const char *page) {
  rootWebPage = page;
}

void
ESPWiFiInterfaceWebServer::setSocketConnectionListener(void (*callback)(uint8_t num)) {
  socketConnectionListener = callback;
}

void
ESPWiFiInterfaceWebServer::setSocketDisconnectionListener(void (*callback)(uint8_t num)) {
  socketDisconnectionListener = callback;
}

void
ESPWiFiInterfaceWebServer::setSocketTextMessageListener(void (*callback)(uint8_t num, const char *message, size_t length)) {
  socketTextMessageListener = callback;
}

void
ESPWiFiInterfaceWebServer::sendTextMessageToSocket(uint8_t num, const char *message) {
  if (isActive()) {
    socketServer->sendTXT(num, message);
  }
}

void
ESPWiFiInterfaceWebServer::broadcastTextMessageToSockets(const char *message) {
  if (isActive()) {
    socketServer->broadcastTXT(message);
  }
}

void
ESPWiFiInterfaceWebServer::onSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: {
        if (socketDisconnectionListener != NULL) {
          socketDisconnectionListener(num);
        }
      }
      break;
    case WStype_CONNECTED: {
        if (socketConnectionListener != NULL) {
          socketConnectionListener(num);
        }
      }
      break;
    case WStype_TEXT: {
        if (socketTextMessageListener != NULL) {
          socketTextMessageListener(num, reinterpret_cast<const char *>(payload), length);
        }
      }
      break;
    default:
      // we don't manage other types yet
      break;
  }
}
