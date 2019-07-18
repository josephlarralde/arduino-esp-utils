#ifdef ESP32
#include <SPIFFS.h>
#endif
#include <FS.h>
#include "ESPConfigFile.h"

bool
ESPConfigFile::load() {
  if (!fileExists()) return false;

#ifdef ESP8266
  File file = SPIFFS.open((const char *) fileName, "r");
#elif defined(ESP32)
  File file = SPIFFS.open((const char *) fileName);
#endif

  if (!file) return false;

  char param[MAX_ESP_CONFIG_FILE_STRING_SIZE];
  char value[MAX_ESP_CONFIG_FILE_STRING_SIZE];

  strcpy(param, "");
  strcpy(value, "");

  bool readingName = true;

  while (file.available()) {

    char carr[2];
    file.readBytes(carr, 1);
    carr[1] = '\0';
    char c = carr[0];
    // Serial.print(c);

    if (c == '\n' && !readingName) {
      Parameter *p = getParameter((const char *) param);

      if (p != NULL) {
        switch (p->getType()) {
          case 'b':
            setBoolParameter((const char *) param, strcmp(value, "0") != 0);
            break;
          case 'i':
            setIntParameter((const char *) param, atol(value));
            break;
          case 'f':
            setFloatParameter((const char *) param, atof(value));
            break;
          case 's':
            setStringParameter((const char *) param, (const char *) value);
            break;
        }
      }

      strcpy(param, "");
      strcpy(value, "");

      readingName = true;
    } else if (c == '\n' && readingName) {
      readingName = false;
    } else {
      const char *str = (const char *) carr;

      if (readingName) {
        strcat(param, str);
      } else {
        strcat(value, str);
      }
    }
  }

  file.close();
  return true;
}

bool
ESPConfigFile::store() {
#ifdef ESP8266
  File file = SPIFFS.open((const char *) fileName, "w+");
#elif defined(ESP32)
  File file = SPIFFS.open((const char *) fileName, FILE_WRITE);
#endif

  if (!file) return false;

  char line[MAX_ESP_CONFIG_FILE_STRING_SIZE];

  for (int i = 0; i < nbParameters; i++) {
    Parameter *p = parameters[i];

    strcpy(line, p->getName());
    strcat(line, "\n");
    char number[MAX_ESP_CONFIG_FILE_DBL_DIGITS * 2];

    switch (p->getType()) {
      case 'b':
        strcat(line, p->getBoolValue() ? "1" : "0");
        break;
      case 'i':
        sprintf(number, "%ld", p->getIntValue());
        strcat(line, (const char *) number);
        break;
      case 'f':
        sprintf(number, "%.*f", MAX_ESP_CONFIG_FILE_DBL_DIGITS, p->getFloatValue());
        strcat(line, (const char *) number);
        break;
      case 's':
        strcat(line, p->getStringValue());
        break;
    }

    strcat(line, "\n");
    file.print((const char *) line);
  }

  file.close();
  return true;
}

bool
ESPConfigFile::clear() {
  for (int i = 0; i < nbParameters; i++) {
    parameters[i]->setDefault();
  }

  if (fileExists()) {
    return SPIFFS.remove(fileName);
  }

  return false;
}

bool
ESPConfigFile::fileExists() {
#ifdef ESP8266

  if (!SPIFFS.begin()) return false;
  return SPIFFS.exists(fileName);

#elif defined(ESP32)

  if (!SPIFFS.begin(true)) return false;

  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  bool exists = false;

  while (file) {
    if (!file.isDirectory()) {
      if (strcmp(file.name(), (const char *) fileName) == 0) {
        exists = true;
        break;
      }
    }

    file = root.openNextFile();
  }

  return exists;

#endif
}
