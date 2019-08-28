# arduino-esp-utils

### A collection of utility classes for esp8266 and esp32 chips

This library is intended to speed up the development of esp8266 and esp32 based projects, providing a high-level class for each typical use case :

* `ESPConfigFile` : a configuration file management class based on SPIFFS
* `ESPWiFiInterfaceWebServer` : a simple web server with web sockets
* `ESPWiFiInterfaceOSC` : an OSC data streaming utility

It relies on the following third-party libraries :

* [WebSockets](https://github.com/Links2004/arduinoWebSockets)
* [OSC](https://github.com/CNMAT/OSC)
* [EventEmitter](https://github.com/josephlarralde/ArduinoEventEmitter)
