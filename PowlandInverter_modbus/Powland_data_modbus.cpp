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

JsonDocument  Powland_data_class::read_data_value(){
  JsonDocument  doc,doc_new,doc_old,doc_addr;
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

PowLand_energy Powland_data_class::initEnergy(){
  pl_energy.AverageInverterPower = 0;
  pl_energy.AverageMainsPower = 0;
  pl_energy.BatteryAveragePower = 0;
  pl_energy.InverterChargingPower = 0;
  pl_energy.OutputActivePower = 0;
  pl_energy.OutputApparentPower = 0;
  pl_energy.PVAveragePower = 0;
  pl_energy.PVChargingAveragePower = 0;
  
  pl_energy.AverageInverterEnergy = 0;
  pl_energy.AverageMainsEnergy = 0;
  pl_energy.BatteryAverageEnergy = 0;
  pl_energy.InverterChargingEnergy = 0;
  pl_energy.OutputActiveEnergy = 0;
  pl_energy.OutputApparentEnergy = 0;
  pl_energy.PVAverageEnergy = 0;
  pl_energy.PVChargingAverageEnergy = 0;

  pl_energy.duration = 0;
  pl_energy.last_record = millis()/1000;
  
  return pl_energy;
}

PowLand_energy Powland_data_class::readEnergyClean(){
  PowLand_energy pl_energyaux = readEnergy();
  pl_energy.AverageInverterEnergy=0;
  pl_energy.AverageMainsEnergy=0;
  pl_energy.BatteryAverageEnergy=0;
  pl_energy.InverterChargingEnergy=0;
  pl_energy.OutputActiveEnergy=0;
  pl_energy.OutputApparentEnergy=0;
  pl_energy.PVAverageEnergy=0;
  pl_energy.PVChargingAverageEnergy=0;
  pl_energy.duration = 0;
  return pl_energyaux;
}

PowLand_energy Powland_data_class::readEnergy(){
  unsigned long duration;
  float AverageInverterPower,AverageMainsPower,BatteryAveragePower,InverterChargingPower,OutputActivePower,OutputApparentPower,PVAveragePower,PVChargingAveragePower;
  duration = millis()/1000 - pl_energy.last_record;
  pl_energy.duration += duration;
  pl_energy.last_record = millis()/1000;
  JsonDocument  jsonVal = read_data_value();
  if(jsonVal["resp"] != 1){
    AverageInverterPower=0;
    AverageMainsPower=0;
    BatteryAveragePower=0;
    InverterChargingPower=0;
    OutputActivePower=0;
    OutputApparentPower=0;
    PVAveragePower=0;
    PVChargingAveragePower=0;
  }
  else{
    JsonDocument  vals = jsonVal["values"];
    AverageInverterPower=vals["AverageInverterPower"];
    AverageMainsPower=vals["AverageMainsPower"];
    BatteryAveragePower=vals["BatteryAveragePower"];
    InverterChargingPower=vals["InverterChargingPower"];
    OutputActivePower=vals["OutputActivePower"];
    OutputApparentPower=vals["OutputApparentPower"];
    PVAveragePower=vals["PVAveragePower"];
    PVChargingAveragePower=vals["PVChargingAveragePower"];
  }
  pl_energy.AverageInverterEnergy+=(AverageInverterPower+pl_energy.AverageInverterPower)/2*duration/3600;
  pl_energy.AverageMainsEnergy+=(AverageMainsPower+pl_energy.AverageMainsPower)/2*duration/3600;
  pl_energy.BatteryAverageEnergy+=(BatteryAveragePower+pl_energy.BatteryAveragePower)/2*duration/3600;
  pl_energy.InverterChargingEnergy+=(InverterChargingPower+pl_energy.InverterChargingPower)/2*duration/3600;
  pl_energy.OutputActiveEnergy+=(OutputActivePower+pl_energy.OutputActivePower)/2*duration/3600;
  pl_energy.OutputApparentEnergy+=(OutputApparentPower+pl_energy.OutputApparentPower)/2*duration/3600;
  pl_energy.PVAverageEnergy+=(PVAveragePower+pl_energy.PVAveragePower)/2*duration/3600;
  pl_energy.PVChargingAverageEnergy+=(PVChargingAveragePower+pl_energy.PVChargingAveragePower)/2*duration/3600;

  pl_energy.AverageInverterPower = AverageInverterPower;
  pl_energy.AverageMainsPower = AverageMainsPower;
  pl_energy.BatteryAveragePower = BatteryAveragePower;
  pl_energy.InverterChargingPower = InverterChargingPower;
  pl_energy.OutputActivePower = OutputActivePower;
  pl_energy.OutputApparentPower = OutputApparentPower;
  pl_energy.PVAveragePower = PVAveragePower;
  pl_energy.PVChargingAveragePower = PVChargingAveragePower;

  return pl_energy;
}
JsonDocument PowLand_energy::read_data_value(){
  JsonDocument val;
  val["AverageInverterPower"] = AverageInverterPower;
  val["AverageMainsPower"] = AverageMainsPower;
  val["BatteryAveragePower"] = BatteryAveragePower;
  val["InverterChargingPower"] = InverterChargingPower;
  val["OutputActivePower"] = OutputActivePower;
  val["OutputApparentPower"] = OutputApparentPower;
  val["PVAveragePower"] = PVAveragePower;
  val["PVChargingAveragePower"] = PVChargingAveragePower;
  
  val["AverageInverterEnergy"] = AverageInverterEnergy;
  val["AverageMainsEnergy"] = AverageMainsEnergy;
  val["BatteryAverageEnergy"] = BatteryAverageEnergy;
  val["InverterChargingEnergy"] = InverterChargingEnergy;
  val["OutputActiveEnergy"] = OutputActiveEnergy;
  val["OutputApparentEnergy"] = OutputApparentEnergy;
  val["PVAverageEnergy"] = PVAverageEnergy;
  val["PVChargingAverageEnergy"] = PVChargingAverageEnergy;

  val["duration"] = duration;
  val["last_record"] = last_record;
  return val;
}

String PowLand_energy::read_data(){
  String ouput_str;
  JsonDocument val = read_data_value();
  serializeJson(val, ouput_str);
  return ouput_str;}