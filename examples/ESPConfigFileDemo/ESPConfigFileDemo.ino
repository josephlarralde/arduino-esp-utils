#include <ESPConfigFile.h>

// this will manage parameters using a file named "machin.txt" placed at the root of
// the file system (its absolute path will be "/machin.txt")
ESPConfigFile config("machin");

bool success;

void setup() {
  Serial.begin(115200);

  delay(5000);

  // NOTES :

  // initial parameter values (defined in add...Parameter methods) are restored
  // at the same time the file is deleted when the clear() method is called

  // bool, long, double and const char * are the four types used by the class
  // internally for bool, int, float and string, respectively
  // you can use other integer and float types with the add...Parameter and
  // set...Parameter methods but they will always be converted internally : thus
  // getIntParameter always returns a long, and getFloatParameter always returns
  // a double

  config.addIntParameter("one two three", 123);
  config.addStringParameter("my string", "coucou");
  config.addBoolParameter("do it", true);
  config.addFloatParameter("pi", 3.1415926535);

  success = config.load(); // should return false, as we didn't store the file yet
  Serial.println(success ? "loaded" : "not loaded");

  Serial.print("\"one two three\" parameter value : ");
  Serial.println(config.getIntParameter("one two three"));
  Serial.print("\"my string\" parameter value : ");
  Serial.println(config.getStringParameter("my string"));
  Serial.print("\"do it\" parameter value : ");
  Serial.println(config.getBoolParameter("do it"));
  Serial.print("\"pi\" parameter value : ");
  Serial.println(config.getFloatParameter("pi"), 15); // we want a lot of decimals displayed

  success = config.store(); // should always return true whether the file exists or not
  Serial.println(success ? "stored" : "not stored");

  config.setIntParameter("one two three", 321);
  config.setFloatParameter("pi", 3.141592);
  config.setBoolParameter("do it", false);
  config.setStringParameter("my string", "bye bye");

  Serial.print("\"one two three\" parameter value : ");
  Serial.println(config.getIntParameter("one two three"));
  Serial.print("\"my string\" parameter value : ");
  Serial.println(config.getStringParameter("my string"));
  Serial.print("\"do it\" parameter value : ");
  Serial.println(config.getBoolParameter("do it"));
  Serial.print("\"pi\" parameter value : ");
  Serial.println(config.getFloatParameter("pi"), 15);

  success = config.load(); // should return true, now that we stored the file
  Serial.println(success ? "loaded" : "not loaded");

  Serial.print("\"one two three\" parameter value : ");
  Serial.println(config.getIntParameter("one two three"));
  Serial.print("\"my string\" parameter value : ");
  Serial.println(config.getStringParameter("my string"));
  Serial.print("\"do it\" parameter value : ");
  Serial.println(config.getBoolParameter("do it"));
  Serial.print("\"pi\" parameter value : ");
  Serial.println(config.getFloatParameter("pi"), 15);

  success = config.clear(); // should return true, as we have a stored file
  Serial.println(success ? "cleared" : "not cleared");

  success = config.load(); // should return false, as we just deleted the file
  Serial.println(success ? "loaded" : "not loaded");

  success = config.clear(); // should return false, as we already deleted the file
  Serial.println(success ? "cleared" : "not cleared");

  // next time we run the sketch, the SPIFFS will not contain "machin.txt", as
  // we didn't call store() after clear()
}

void loop() {
  delay(1000);
}