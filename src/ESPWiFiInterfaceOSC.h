#ifndef _ESP_WIFI_INTERFACE_OSC_H_
#define _ESP_WIFI_INTERFACE_OSC_H_

#include "ESPWiFiInterfaceBase.h"

#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "ESPEventEmitter.h"

class ESPWiFiInterfaceOSC : public ESPWiFiInterfaceBase {
private:

  /****************************************************************************/

  class OSCMessageEventEmitter : public ESPEventEmitter<OSCMessage&> {
  private:
    char address[128]; // maximum osc address length
    
  public:
    OSCMessageEventEmitter() : ESPEventEmitter<OSCMessage&>() {}
    ~OSCMessageEventEmitter() {}

    void emitOSCMessage(OSCMessage& msg) {
      msg.getAddress(address);
      emit(address, msg);
    }
  };

  /****************************************************************************/

  WiFiUDP udp;
  IPAddress hostIP;  
  int udpInputPort;
  int udpOutputPort;

  OSCMessage inputOSCMessage;
  OSCMessageEventEmitter oscEmitter;

public:
  ESPWiFiInterfaceOSC();
  ~ESPWiFiInterfaceOSC();

  void startWiFi(WiFiMode_t mode, const char *ssid, const char *pass = "");
  void stopWiFi();
  void update();

  void setHostIP(IPAddress ip);
  void setUdpInputPort(int port);
  void setUdpOutputPort(int port);

  void sendUdpOSCMessage(OSCMessage& msg);
  void addOSCMessageListener(const char *event, std::function<void(OSCMessage&)> callback);
  void removeOSCMessageListeners();
};

#endif /* _ESP_WIFI_INTERFACE_OSC_H_ */
