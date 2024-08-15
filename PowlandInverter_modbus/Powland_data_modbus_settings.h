#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ModbusMaster.h>
#define START_ADDR_SETTINGS 300
#define ADDR_LEN_SETTINGS 38

#define TOTAL_ADDR_SETTINGS 26

class Powland_data_class_settings{ //PowlandInverter_data
  private:   
    ModbusMaster node_modbus_main; 
    int pin_rec,pin_send;
    SoftwareSerial *mySerial;
    float calculate2complement(float);
    String extractResponse(uint8_t);
  public:
    Powland_data_class_settings(SoftwareSerial* serial);
    String read_data();
};
