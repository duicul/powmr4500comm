#ifndef AC_H
#define AC_H

#include <Arduino.h>
#include "Data.h"
#include <SoftwareSerial.h>
#include "inv8851a.h"

//#define ONE_WIRE_BUS 4
//#define TEMPERATURE_PRECISION 9

struct PowMr_energy{
  int16_t pv_power,pv_energy,load_watt,load_energy;
  int16_t t0026_total_power,t0026_total_energy;
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
    bool readState(unsigned char resp[]);
    String convertToJSON(inv8851_state_s *state);
    PowMr_energy pm_energy;
    String runModeSerialize(enum run_mode);
  public:
    PowMr_data(int pin_rec, int pin_send,SoftwareSerial* serial);
    String read_data(); 
};

#endif
