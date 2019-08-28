#include "ESPWiFiInterfaceOSC.h"

ESPWiFiInterfaceOSC::ESPWiFiInterfaceOSC() :
ESPWiFiInterfaceBase(),
udpInputPort(8001),
udpOutputPort(8000) {
  hostIP = IPAddress(192, 168, 0, 100);
}

ESPWiFiInterfaceOSC::~ESPWiFiInterfaceOSC() {}

void
ESPWiFiInterfaceOSC::startWiFi(WiFiMode_t mode,
                               const char *ssid, const char *pass) {
  ESPWiFiInterfaceBase::startWiFi(mode, ssid, pass);
  udp.begin(udpInputPort);
}

void
ESPWiFiInterfaceOSC::stopWiFi() {
  ESPWiFiInterfaceBase::stopWiFi();
  udp.stop();
}

void
ESPWiFiInterfaceOSC::update() {
  ESPWiFiInterfaceBase::update();

  if (isActive()) {
    int packetSize = udp.parsePacket();

    if (packetSize > 0) {
      while (packetSize--) {
        inputOSCMessage.fill(udp.read());
      }

      if (!inputOSCMessage.hasError()) {
        oscEmitter.emitOSCMessage(inputOSCMessage);
      }

      inputOSCMessage.empty();
    }
  }
}

void
ESPWiFiInterfaceOSC::setHostIP(IPAddress ip) {
  hostIP = ip;
}

void
ESPWiFiInterfaceOSC::setUdpInputPort(int port) {
  if (isActive() && udpInputPort != port) {
    udp.begin(port);
  }

  udpInputPort = port;
}

void
ESPWiFiInterfaceOSC::setUdpOutputPort(int port) {
  udpOutputPort = port;
}

void
ESPWiFiInterfaceOSC::sendUdpOSCMessage(OSCMessage& msg) {
  if (isActive()) {
    udp.beginPacket(hostIP, udpOutputPort);
    msg.send(udp);
    udp.endPacket();
  }
}

void
ESPWiFiInterfaceOSC::addOSCMessageListener(const char *event, std::function<void(OSCMessage&)> callback) {
  oscEmitter.addListener(event, callback);
}

void
ESPWiFiInterfaceOSC::removeOSCMessageListeners() {
  oscEmitter.removeAllListeners();
}
