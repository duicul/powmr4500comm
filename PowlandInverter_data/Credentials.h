#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>
#include <EEPROM.h>

#ifndef APSSID
#define APSSID "PowMrMeasurement_0001"
#define APPSK  ""
#endif

class Credentials {
  
  private:
    char ssid[20], password[20];
    int polling_duration;
  public:
    char* getssid();
    char* getpassword();
    float getpolling_duration();
    void loadCredentials();
    void saveCredentials();
    void setCredentials(char *s,char *p,int r);
    Credentials();
};

#endif
