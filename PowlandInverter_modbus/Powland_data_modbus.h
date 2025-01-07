#include <Arduino.h>
#include "Data.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ModbusMaster.h>
#define START_ADDR 200
#define ADDR_LEN 35

#define TOTAL_ADDR 28

struct PowLand_energy{
  public:
    float AverageInverterPower,AverageMainsPower,BatteryAveragePower,InverterChargingPower,OutputActivePower,OutputApparentPower,PVAveragePower,PVChargingAveragePower;
    unsigned long duration;
    float AverageInverterEnergy,AverageMainsEnergy,BatteryAverageEnergy,InverterChargingEnergy,OutputActiveEnergy,OutputApparentEnergy,PVAverageEnergy,PVChargingAverageEnergy;
    JsonDocument  read_data_value();
    unsigned long last_record;
    String read_data();
};

class Powland_data_class{ //PowlandInverter_data
  private:    
    ModbusMaster node_modbus_main;
    int pin_rec,pin_send;
    SoftwareSerial *mySerial;
    float calculate2complement(float);
    JsonDocument  extractResponse(uint8_t);
    JsonDocument  read_data_value();
  public:
    PowLand_energy pl_energy;
    Powland_data_class(SoftwareSerial* serial);
    String read_data();

    PowLand_energy initEnergy();
    PowLand_energy readEnergy();
    PowLand_energy readEnergyClean();

};
