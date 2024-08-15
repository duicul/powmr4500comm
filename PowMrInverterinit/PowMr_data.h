#ifndef AC_H
#define AC_H

#include <Arduino.h>
#include "Data.h"
#include <SoftwareSerial.h>
#include "inv8851a.h"

//#define ONE_WIRE_BUS 4
//#define TEMPERATURE_PRECISION 9
#define SOFTWARE_VERSION 5502
struct PowMr_energy{
  int16_t pv_power,load_watt,t0026_total_power,batt_power;
  int16_t batt_power_charge,batt_power_discharge;
  unsigned long duration,last_record;
  float pv_energy,load_energy,t0026_total_energy,batt_energy;
  float batt_energy_charge,batt_energy_discharge;
};

class PowMr_data: public Data_interface {
  private:    
    int pin_rec,pin_send;
    SoftwareSerial *mySerial;
    void sendStateReq();
    void HexStringToBytes(unsigned char *req_hex_state,uint8_t *req,unsigned int *len,int hexlen);
    char converthexdigit(char hexdigit);
    unsigned char reverse(unsigned char b);
    void reversearray(unsigned char resp[] , int resp_len);
    bool readState(uint8_t * resp);
    unsigned char respStore[160];
    String runModeSerialize(enum run_mode);
  public:
    PowMr_energy pm_energy;
    PowMr_data(int pin_rec, int pin_send,SoftwareSerial* serial);
    String convertToJSON(inv8851_state_s *state);
    String convertToJSON(PowMr_energy Mr_energy);
    String read_data(); 
    inv8851_state_s* read_data_value();
    PowMr_energy initEnergy();
    PowMr_energy readEnergy();
    PowMr_energy readEnergyClean();
};

#endif
