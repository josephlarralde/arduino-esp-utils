#include <Arduino.h>

#ifdef ESP32
#include <Adafruit_NeoPixel.h>
#endif

class Blinker {
private:

#ifdef ESP8266
  #define LED_PIN 2
#elif defined(ESP32)
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 15, NEO_GRB + NEO_KHZ800);
#endif

  bool state;
  bool blinking;
  unsigned long period;
  unsigned long lastTickDate;

public:
  Blinker() : state(false), blinking(false), period(100) {
    lastTickDate = millis();

#ifdef ESP8266
    pinMode(LED_PIN, OUTPUT);
#elif defined(ESP32)
    strip.begin();
    strip.clear();
    strip.show();    
#endif
  }

  ~Blinker() {}

  void setPeriod(unsigned long p) {
    blinking = true;
    period = p;
  }

  void setHigh() {
    blinking = false;
    state = true;
  }

  void setLow() {
    blinking = false;
    state = false;
  }

  void update() {
    unsigned long now = millis();

    if (blinking && now - lastTickDate >= period) {
      lastTickDate = now;
      state = !state;
    }

#ifdef ESP8266
    digitalWrite(LED_PIN, state ? LOW : HIGH);
#elif defined(ESP32)
    strip.clear();
    strip.setPixelColor(0, strip.Color(0, 0, state ? 127 : 0)); // blue blink
    strip.show();
#endif
  }
};
