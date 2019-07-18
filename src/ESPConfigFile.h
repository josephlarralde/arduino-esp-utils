#ifndef _ESP_CONFIG_FILE_H_
#define _ESP_CONFIG_FILE_H_

#define MAX_ESP_CONFIG_FILE_PARAMETERS 120
#define MAX_ESP_CONFIG_FILE_STRING_SIZE 120

// see https://stackoverflow.com/questions/9999221/double-precision-decimal-places
#define MAX_ESP_CONFIG_FILE_DBL_DIGITS 15

/************************* Persistent config class ****************************/

class ESPConfigFile {
private:

  /************************ Parameter utility class ***************************/

  class Parameter {
  private:
    char name[MAX_ESP_CONFIG_FILE_STRING_SIZE];
    char type;

    bool b, defb;
    long i, defi;
    double f, deff;
    char s[MAX_ESP_CONFIG_FILE_STRING_SIZE];
    char defs[MAX_ESP_CONFIG_FILE_STRING_SIZE];

  public:
    Parameter(const char *n) :
    b(false), defb(false), i(0), defi(0), f(0), deff(0) {
      type = 'b';
      strcpy(name, n);
      strcpy(s, "");
      strcpy(defs, "");
    }

    Parameter(const char *n, bool v) : Parameter(n) { b = defb = v; }
    Parameter(const char *n, long v) : Parameter(n) { type = 'i'; i = defi = v; }
    Parameter(const char *n, double v) : Parameter(n) { type = 'f'; f = deff = v; }
    Parameter(const char *n, const char *v) : Parameter(n) { type = 's'; strcpy(s, v); strcpy(defs, v); }

    void setValue(bool v) { b = v; }
    void setValue(long v) { i = v; }
    void setValue(double v) { f = v; }
    void setValue(const char *v) { strcpy(s, v); }

    void setDefault() {
      switch (type) {
        case 'b':
          b = defb;
          break;
        case 'i':
          i = defi;
          break;
        case 'f':
          f = deff;
          break;
        case 's':
          strcpy(s, defs);
          break;
      }
    }

    const char *getName() { return (const char *) name; }
    char getType() { return type; }

    bool getBoolValue() { return b; }
    long getIntValue() { return i; }
    double getFloatValue() { return f; }
    const char *getStringValue() { return (const char *) s; }
  };

  /************************* Other private fields *****************************/

  char fileName[MAX_ESP_CONFIG_FILE_STRING_SIZE];
  int nbParameters;
  Parameter *parameters[MAX_ESP_CONFIG_FILE_PARAMETERS];

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

public:
  ESPConfigFile(const char *fn) : nbParameters(0) {
    strcpy(fileName, "/");
    strcat(fileName, fn);
    strcat(fileName, ".txt");
  }

  ~ESPConfigFile() {
    for (int i = 0; i < nbParameters; i++) {
      delete parameters[i];
    }
  }

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  ////////// boolean parameters

  void addBoolParameter(const char *name, bool value) {
    parameters[nbParameters] = new Parameter(name, value);
    nbParameters++;
  }

  ////////// integer parameters

  void addIntParameter(const char *name, long value) {
    parameters[nbParameters] = new Parameter(name, value);
    nbParameters++;
  }

  void addIntParameter(const char *name, unsigned long value) {
    addIntParameter(name, static_cast<long>(value));
  }

  void addIntParameter(const char *name, int value) {
    addIntParameter(name, static_cast<long>(value));
  }

  void addIntParameter(const char *name, unsigned int value) {
    addIntParameter(name, static_cast<long>(value));
  }

  void addIntParameter(const char *name, short value) {
    addIntParameter(name, static_cast<long>(value));
  }

  void addIntParameter(const char *name, unsigned short value) {
    addIntParameter(name, static_cast<long>(value));
  }

  ////////// float parameters

  void addFloatParameter(const char *name, double value) {
    parameters[nbParameters] = new Parameter(name, value);
    nbParameters++;
  }

  void addFloatParameter(const char *name, float value) {
    addFloatParameter(name, static_cast<double>(value));
  }

  void addFloatParameter(const char *name, long double value) {
    addFloatParameter(name, static_cast<double>(value));
  }

  ////////// string parameters

  void addStringParameter(const char *name, const char *value) {
    parameters[nbParameters] = new Parameter(name, value);
    nbParameters++;
  }

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  ////////// boolean parameters

  void setBoolParameter(const char *name, bool value) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 'b') p->setValue(value);
  }

  ////////// integer parameters

  void setIntParameter(const char *name, long value) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 'i') p->setValue(value);
  }

  void setIntParameter(const char *name, unsigned long value) {
    setIntParameter(name, static_cast<long>(value));
  }

  void setIntParameter(const char *name, int value) {
    setIntParameter(name, static_cast<long>(value));
  }

  void setIntParameter(const char *name, unsigned int value) {
    setIntParameter(name, static_cast<long>(value));
  }

  void setIntParameter(const char *name, short value) {
    setIntParameter(name, static_cast<long>(value));
  }

  void setIntParameter(const char *name, unsigned short value) {
    setIntParameter(name, static_cast<long>(value));
  }

  ////////// float parameters

  void setFloatParameter(const char *name, double value) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 'f') p->setValue(value);
  }

  void setFloatParameter(const char *name, float value) {
    setFloatParameter(name, static_cast<double>(value));
  }

  void setFloatParameter(const char *name, long double value) {
    setFloatParameter(name, static_cast<double>(value));
  }

  ////////// string parameters

  void setStringParameter(const char *name, const char *value) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 's') p->setValue(value);
  }

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  bool getBoolParameter(const char *name) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 'b') return p->getBoolValue();
    return false;
  }

  long getIntParameter(const char *name) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 'i') return p->getIntValue();
    return 0;
  }

  double getFloatParameter(const char *name) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 'f') return p->getFloatValue();
    return 0;
  }

  const char *getStringParameter(const char *name) {
    Parameter *p = getParameter(name);
    if (p != NULL && p->getType() == 's') return p->getStringValue();
    return "";
  }

  bool load();
  bool store();
  bool clear();

private:
  bool fileExists();

  Parameter *getParameter(const char *name) {
    for (int i = 0; i < nbParameters; i++) {
      if (strcmp(parameters[i]->getName(), name) == 0) {
        return parameters[i];
      }
    }

    return NULL;
  }
};

#endif /* _ESP_CONFIG_FILE_H_ */