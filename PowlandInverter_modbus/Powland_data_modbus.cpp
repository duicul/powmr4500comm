#include "Powland_data_modbus.h"
int register_addresses [] = {201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,219,220,223,224,225,226,227,229,232,233,234};

String register_name [] = {"OperationMode","EffectiveMainsVoltage","MainsFrequency","AverageMainsPower","EffectiveInverterVoltage","EffectiveInverterCurrent","InverterFrequency",\
                       "AverageInverterPower","InverterChargingPower","OutputEffectiveVoltage","OutputEffectiveCurrent","OutputFrequency","OutputActivePower","OutputApparentPower",\
                       "BatteryAverageVoltage","BatteryAverageCurrent","BatteryAveragePower","PVAverageVoltage","PVAverageCurrent","PVAveragePower","PVChargingAveragePower",\
                       "LoadPercentage","DCDCTemperature","InverterTemperature","BatteryStateOfCharge","BatteryAverageCurrentFlow","InverterChargingAverageCurrent","PVChargingAverageCurrent"};

int register_multiplier [] = {1,10,100,1,10,10,100,1,1,10,10,100,1,1,10,10,1,10,10,1,1,1,1,1,1,10,10,10};

Powland_data_class::Powland_data_class(SoftwareSerial* serial){
  mySerial = serial;
}

float Powland_data_class::calculate2complement(float number){
  int aux = number;
  int i;
  for(i=0;i<16;i++){
    aux ^= 1<<i;
  }
  aux +=1;
  return aux * (-1);
}

JsonDocument Powland_data_class::extractResponse(uint8_t result){
  JsonDocument doc;
  //String ouput_str;
  String status = "no response ";
  status += result;
  doc["status"] = status;

  if(result == node_modbus_main.ku8MBIllegalFunction)
      doc["status"] = "ku8MBIllegalFunction";

  if(result == node_modbus_main.ku8MBIllegalDataAddress)
      doc["status"] = "ku8MBIllegalDataAddress";

  if(result == node_modbus_main.ku8MBInvalidFunction)
      doc["status"] = "ku8MBInvalidFunction";

  if(result == node_modbus_main.ku8MBIllegalDataValue)
      doc["status"] = "ku8MBIllegalDataValue";
  
  if(result == node_modbus_main.ku8MBInvalidCRC)
      doc["status"] = "ku8MBInvalidCRC";

  if(result == node_modbus_main.ku8MBInvalidSlaveID)
      doc["status"] = "ku8MBInvalidSlaveID";
    
  if(result == node_modbus_main.ku8MBSlaveDeviceFailure)
      doc["status"] = "ku8MBSlaveDeviceFailure";
    
  if(result == node_modbus_main.ku8MBResponseTimedOut)
      doc["status"] = "ku8MBResponseTimedOut";
  
  doc["OK"] = 0;
  //serializeJson(doc, ouput_str);
  return doc;
}

JsonDocument Powland_data_class::read_data_value(){
  JsonDocument doc,doc_new,doc_old,doc_addr;
  static uint32_t i;
  uint8_t j, result;
  float data[ADDR_LEN+ADDR_LEN/20];
  float data_store[ADDR_LEN+ADDR_LEN/20];
// use Serial (port 0); initialize Modbus communication baud rate
  mySerial->begin(9600);
  //mySerial->setTimeout(10000);
  // communicate with Modbus slave ID 0 over SoftwareSerial (port 0)
  node_modbus_main.begin(0, *mySerial);
  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node_modbus_main.readHoldingRegisters(START_ADDR, ADDR_LEN+2);
  String ouput_str;
  // do something with data if read is successful
  if (result == node_modbus_main.ku8MBSuccess)
  {
    for (j = 0; j < ADDR_LEN; j++)
    { data[j] = node_modbus_main.getResponseBuffer(j);
      data_store[j] = data[j];
      float aux = data[j];//htons(data[j]);
      if(aux > 4500 && (j+START_ADDR == 208 || j+START_ADDR == 216 || j+START_ADDR == 217 || j+START_ADDR == 232)){
        aux = calculate2complement(aux);
        data[j]=aux;
      }
    }
        //batery power negative if battery current negative
    if(data[217-START_ADDR] >0 && data[232-START_ADDR] < 0)
      data[217-START_ADDR] = calculate2complement(data[217-START_ADDR]);
    
    if(data[232-START_ADDR] >0 && data[217-START_ADDR] < 0)
      data[232-START_ADDR] = calculate2complement(data[232-START_ADDR]);

    for(int q=0;q<TOTAL_ADDR;q++){
      int data_index = register_addresses[q]-START_ADDR;
      float data_value = data[data_index] / register_multiplier[q];
      doc_new[register_name[q]] = data_value;
      String nameold = register_name[q];
      //nameold += "Old";
      doc_old[nameold] = data_store[data_index];
      String nameaddr = register_name[q];
      //nameaddr += "Addr";
      doc_addr[nameaddr] = register_addresses[q];
    }       

    doc["values"] = doc_new;
    doc["addr"] = doc_addr;
    doc["old"] = doc_old;


    doc["resp"] = 1;    
  }else{
    doc =extractResponse(result);
  }
  return doc;
}

String Powland_data_class::read_data(){
  JsonDocument doc = read_data_value();
  String ouput_str;
  serializeJson(doc, ouput_str);
  return ouput_str;
}