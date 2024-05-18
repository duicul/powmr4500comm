#include "Credentials.h"

Credentials::Credentials(){
  this->loadCredentials();
}

void Credentials::loadCredentials(){
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0 + sizeof(ssid), password);
  EEPROM.get(0 + sizeof(ssid) + sizeof(password), polling_duration);
  char ok[2 + 1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password)+sizeof(polling_duration), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(polling_duration);
}
void Credentials::saveCredentials(){
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0 + sizeof(ssid), password);
  EEPROM.put(0 + sizeof(ssid) + sizeof(password), polling_duration);
  char ok[2 + 1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password)+sizeof(polling_duration), ok);
  EEPROM.commit();
  EEPROM.end();
}

void Credentials::setCredentials(char *s,char *p,int r){
  strcpy(ssid,s);
  strcpy(password,p);
  polling_duration = r;
}

char* Credentials::getssid(){
  return ssid;
}

char* Credentials::getpassword(){
  return password;
}

float Credentials::getpolling_duration(){
  return polling_duration;
}
