#include <Arduino.h>
#include "Data.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ModbusMaster.h>
#define START_ADDR 200
#define ADDR_LEN 35

#define TOTAL_ADDR 28

class Powland_data_class{ //PowlandInverter_data
  private:    
    ModbusMaster node_modbus_main;
    int pin_rec,pin_send;
    SoftwareSerial *mySerial;
    float calculate2complement(float);
    JsonDocument extractResponse(uint8_t);
    JsonDocument read_data_value();
  public:
    Powland_data_class(SoftwareSerial* serial);
    String read_data();
};
