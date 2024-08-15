#include "Powland_data_modbus_settings.h"

int register_addresses_settings [] = {300,301,302,303,305,306,307,308,309,310,313,320,321,323,324,325,326,327,329,331,332,333,334,335,336,336};

String register_name_settings [] = {"OutputMode","OutputPriority","InputVoltageRange","BuzzerMode","LCD_Backlight","LCD_AutomaticallyReturnsToTheHomepage","EnergySavingMode",\
                                    "OverloadAutomaticRestart","OverTemperatureAutomaticRestart","OverloadTransferToBypassEnabled","BatteryEqModeIsEnabled","OutputVoltage",\
                                    "OutputFrequency","BatteryOvervoltageProtectionPoint","MaxChargingVoltage","FloatingChargingVoltage","BatteryDischargeRecoveryPointInMainsMode",\
                                    "BatteryLowVoltageProtectionPointInMainsMode","BatteryLowVoltageProtectionPointInOffGridMode","BatteryChargingPriority","MaximumChargingCurrent",\
                                    "MaximumMainsChargingCurrent","EqChargingVoltage","BatteryEqualizationTime","EqualizationTimeoutExit","TwoEqualizationChargingIntervals"};

int register_multiplier_settings [] = {1,1,1,1,1,1,1,1,1,1,1,10,100,10,10,10,10,10,10,1,10,10,10,1,1,1};

// instantiate ModbusMaster object


Powland_data_class_settings::Powland_data_class_settings(SoftwareSerial* serial){
  mySerial = serial;
}

float Powland_data_class_settings::calculate2complement(float number){
  int aux = number;
  int i;
  for(i=0;i<16;i++){
    aux ^= 1<<i;
  }
  aux +=1;
  return aux * (-1);
}

String Powland_data_class_settings::extractResponse(uint8_t result){
  JsonDocument doc;
  String ouput_str;
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
  serializeJson(doc, ouput_str);
  return ouput_str;
}

String Powland_data_class_settings::read_data(){
  JsonDocument doc,doc_new,doc_old,doc_addr;
  static uint32_t i;
  uint8_t j, result;
  float data[ADDR_LEN_SETTINGS+ADDR_LEN_SETTINGS/20];
  float data_store[ADDR_LEN_SETTINGS+ADDR_LEN_SETTINGS/20];
// use Serial (port 0); initialize Modbus communication baud rate
  mySerial->begin(9600);
  //mySerial->setTimeout(10000);
  // communicate with Modbus slave ID 0 over SoftwareSerial (port 0)
  node_modbus_main.begin(0, *mySerial);
  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node_modbus_main.readHoldingRegisters(START_ADDR_SETTINGS, ADDR_LEN_SETTINGS+2);
  String ouput_str;
  // do something with data if read is successful
  if (result == node_modbus_main.ku8MBSuccess)
  {
    for (j = 0; j < ADDR_LEN_SETTINGS; j++)
    { data[j] = node_modbus_main.getResponseBuffer(j);
      data_store[j] = data[j];
    }
    for(int q=0;q<TOTAL_ADDR_SETTINGS;q++){
      int data_index = register_addresses_settings[q]-START_ADDR_SETTINGS;
      float data_value = data[data_index] / register_multiplier_settings[q];
      doc_new[register_name_settings[q]] = data_value;
      String nameold = register_name_settings[q];
      //nameold += "Old";
      doc_old[nameold] = data_store[data_index];
      String nameaddr = register_name_settings[q];
      //nameaddr += "Addr";
      doc_addr[nameaddr] = register_addresses_settings[q];
    }       

    doc["values"] = doc_new;
    doc["addr"] = doc_addr;
    doc["old"] = doc_old;


    doc["resp"] = 1;    
  }else{
    return extractResponse(result);
  }
  serializeJson(doc, ouput_str);
  return ouput_str;
}