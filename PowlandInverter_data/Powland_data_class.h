#ifndef AC_H
#define AC_H

#include <Arduino.h>
#include "Data.h"
#include <SoftwareSerial.h>

#define QPIGS "5150494753b7a90d"
#define QMOD  "514d4F4449c10d"
#define QPIWS "5150495753b4da0d"
#define QTEST "050313ab0001f0ea"

//#define ONE_WIRE_BUS 4
//#define TEMPERATURE_PRECISION 9
#define SOFTWARE_VERSION 5502

class Powland_data_class{ //PowlandInverter_data
  private:    
    int pin_rec,pin_send;
    SoftwareSerial *mySerial;
    void sendStateReq();
    void sendStateReq_Comm(uint8_t*,int);
    void HexStringToBytes(unsigned char *req_hex_state,uint8_t *req,unsigned int *len,int hexlen);
    char converthexdigit(char hexdigit);
    unsigned char reverse(unsigned char b);
    void reversearray(unsigned char resp[] , int resp_len);
    bool readState(char * resp);
    char respStore[160];
    String read_data_command(unsigned char*,int);
  public:
    Powland_data_class(int pin_rec, int pin_send,SoftwareSerial* serial);
    //String read_data(); 
    String read_data_QPIGS();
    String read_data_QPIWS();
    String read_data_QMOD();
    String read_data_QTEST();
};

#endif
