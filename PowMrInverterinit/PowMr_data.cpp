#include "PowMr_data.h"
//#define RX D5
//#define TX D6
//SoftwareSerial mySerial(RX, TX); // RX, TX

PowMr_data::PowMr_data(int _pin_rec,int _pin_send,SoftwareSerial* serial){
  mySerial = serial;
  pin_rec = _pin_rec;
  pin_send = _pin_send;
  mySerial->begin(9600,SWSERIAL_8N1,_pin_rec,_pin_send,false,256,0);
}

char PowMr_data::converthexdigit(char hexdigit){
switch(hexdigit){
case '0':return 0;
case '1':return 1;
case '2':return 2;
case '3':return 3;
case '4':return 4;
case '5':return 5;
case '6':return 6;
case '7':return 7;
case '8':return 8;
case '9':return 9;
case 'a':return 10;
case 'b':return 11;
case 'c':return 12;
case 'd':return 13;
case 'e':return 14;
case 'f':return 15;
}
return 0;
}

void PowMr_data::HexStringToBytes(unsigned char *req_hex_state,uint8_t *req,unsigned int *len,int hexlen){
  for(int i=0;i<hexlen;i++)
  { if(i%2==0){
        unsigned char byteval = converthexdigit(req_hex_state[i+1]);
        byteval+=converthexdigit(req_hex_state[i])*16;
        //cout<<req_hex_state[i]<<" "<<req_hex_state[i+1]<<" "<<byteval<<" "<<*len<<endl;
        Serial.println(byteval);
        req[*len]=byteval;
        *len= *len + 1;
        //cout<<req<<" "<<" "<<*len<<endl;
  }
  }

}

void PowMr_data::sendStateReq(){
  Serial.println("sending state req");
  uint8_t req[12];
  unsigned int req_len = 0;

  unsigned char req_hex_state[] = "88510003000000004d08";

  HexStringToBytes(req_hex_state, req, &req_len,20);
  Serial.println(req_len);
  mySerial->write(req, req_len); 
}


unsigned char PowMr_data::reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

String PowMr_data::runModeSerialize(enum run_mode mode){
   switch(mode){
    case 0:return "0-standby_mode";
    case 1:return "1-fault_mode";
    case 2:return "2-shutdown_mode";
    case 3:return "3-normal_mode";
    case 4:return "4-no_battery_mode";
    case 5:return "5-discharge_mode";
    case 6:return "6-parallel_discharge";
    case 7:return "7-bypass_mode";
    case 8:return "8-charge_mode";
    case 9:return "9-grid_discharge_mode";
    case 10:return "10-micro_grid_discharge_mode";
    case 11:return "11-no_battery_mode";
    case 12:return "12-unknown";
    case 13:return "13-unknown";
    case 14:return "14-unknown";
    case 15:return "15-unknown";
    default: return "unknown";
   };
}

void PowMr_data::reversearray(unsigned char resp[] , int resp_len){
  unsigned char aux[resp_len];
  for(int i=resp_len-1,j=0;i>=0;i--,j++)
    aux[i]=resp[j];
  
  for(int i=0;i<resp_len;i++)
    resp[i]=reverse(aux[i]);
  Serial.println("values");
  for(int i=0;i<resp_len;i++){
    int val = (int)resp[i];
    Serial.print(i);
    Serial.print(" ");
    Serial.println(val);}
}

 bool PowMr_data::readState(uint8_t * resp){
  Serial.println("reading state req");
  //unsigned char resp[160];
  unsigned int resp_len=0;
  bool read = false;
  Serial.println("reading state req available ");
  resp_len=mySerial->available();
  Serial.println(resp_len);
  //if(mySerial->available()<sizeof(inv8851_state_s))
  //  return NULL;
  size_t rlen = mySerial->readBytes(resp,resp_len);
  Serial.println(rlen);
  /*int i = 0;
  while(mySerial->available()){
    resp[i] = mySerial->read();
    int val = (int)resp[i];
    Serial.print(i);
    Serial.print(" ");
    Serial.println(val);
    i++;}*/
  if(rlen == 0 )
    return false;
  return true;
}

String PowMr_data::convertToJSON(inv8851_state_s *state){
  String message = "";
  if(state == NULL)
    return message;
  message += "{";
  //if(state->software_version != SOFTWARE_VERSION)
  //  message += "\"status\":\"disconnected\",";
  message += "\"proto\":";
  message += state->proto;
  message += ",\"command\":";
  message += state->command;
  message += ",\"addr\":";
  message += state->addr;
  message += ",\"data_size\":";
  message += state->data_size;
  message +=  ",\"words\":";
  message += (int) state->words;
  message += ",\"run_mode\":";
  message += state->run_mode;
  message += ",\"inverter_topology\":";
  message += "\""+runModeSerialize(state->inverter_topology)+"\"";
  message += ",\"llc_topology\":";
  message += "\""+runModeSerialize(state->llc_topology)+"\"";
  message += ",\"pv_topology\":";
  message += "\""+runModeSerialize(state->pv_topology)+"\"";
  message += ",\"buck_topology\":";
  message += "\""+runModeSerialize(state->buck_topology)+"\"";
  message += ",\"system_power\":";
  message += state->system_power == 0 ? "false":"true";
  message += ",\"charge_finish\":";
  message += state->charge_finish == 0 ? "false":"true";
  message += ",\"bus_ok\":";
  message += state->bus_ok == 0 ? "false":"true";
  message += ",\"bus_n_grid_voltage_match\":";
  message += state->bus_n_grid_voltage_match == 0 ? "false":"true";
  message += ",\"no_battery\":";
  message += state->no_battery == 0 ? "false":"true";
  message += ",\"pv_excess\":";
  message += state->pv_excess == 0 ? "false":"true";
  message += ",\"floating_charge\":";
  message += state->floating_charge == 0 ? "false":"true";
  message += ",\"system_initial_finished\":";
  message += state->system_initial_finished == 0 ? "false":"true";
  message += ",\"inverter_topology_initial_finished\":";
  message += state->inverter_topology_initial_finished == 0 ? "false":"true";
  message += ",\"llc_topology_initial_finished\":";
  message += state->llc_topology_initial_finished == 0 ? "false":"true";
  message += ",\"pv_topology_initial_finished\":";
  message += state->pv_topology_initial_finished == 0 ? "false":"true";
  message += ",\"buck_topology_initial_finished\":";
  message += state->buck_topology_initial_finished == 0 ? "false":"true";
  message += ",\"eq_charge_start\":";
  message += state->eq_charge_start == 0 ? "false":"true";
  message += ",\"eq_charge_ready\":";
  message += state->eq_charge_ready == 0 ? "false":"true";
  message += ",\"w6\":";
  message += state->w6;
  message += ",\"t0002\":";
  message += state->t0002;
  message += ",\"t0003\":";
  message += state->t0003;
  message += ",\"grid_pll_ok\":";
  message += state->grid_pll_ok == 0 ? "false":"true";
  message += ",\"lo0004\":";
  message += state->lo0004;
  message += ",\"hi0004\":";
  message += state->hi0004;
  message += ",\"disable_utility\":";
  message += state->disable_utility == 0 ? "false":"true";
  message += ",\"hi0004_\":";
  message += state->hi0004_;
  message += ",\"t0005\":";
  message += state->t0005;
  message += ",\"t0006\":";
  message += state->t0006;
  message += ",\"t0007\":";
  message += state->t0007;
  message += ",\"t0008\":";
  message += state->t0008;
  message += ",\"t0009\":";
  message += state->t0009;
  message += ",\"t0010\":";
  message += state->t0010;
  message += ",\"t0011\":";
  message += state->t0011;

  message += ",\"lo0013\":";
  message += state->lo0013;
  message += ",\"pv_input_ok\":";
  message += state->pv_input_ok == 0 ? "false":"true";
  message += ",\"lo0013_\":";
  message += state->lo0013_;
  message += ",\"parallel_lock_phase_ok\":";
  message += state->parallel_lock_phase_ok == 0 ? "false":"true";
  message += ",\"hi0013\":";
  message += state->hi0013;

  message += ",\"software_version\":";
  message += state->software_version;
  message += ",\"log_number\":";
  message += state->log_number;
  message += ",\"t0016\":";
  message += state->t0016;
  message += ",\"t0017\":";
  message += state->t0017;
  message += ",\"t0018\":";
  message += state->t0018;
  message += ",\"t0019\":";
  message += state->t0019;
  message += ",\"t0020\":";
  message += state->t0020;
  message += ",\"inv_voltage\":";
  message += state->inv_voltage;
  message += ",\"inv_current\":";
  message += state->inv_current;
  message += ",\"inv_freq\":";
  message += state->inv_freq;
  message += ",\"inv_va\":";
  message += state->inv_va;
  message += ",\"load_va\":";
  message += state->load_va;
  message += ",\"t0026\":";
  message += state->t0026;
  message += ",\"load_watt\":";
  message += state->load_watt;
  message += ",\"inverter_va_percent\":";
  message += state->inverter_va_percent;
  message += ",\"inverter_watt_percent\":";
  message += state->inverter_watt_percent;
  message += ",\"load_current\":";
  message += state->load_current;
  message += ",\"low_load_current\":";
  message += state->low_load_current;
  message += ",\"t0032\":";
  message += state->t0032;
  message += ",\"grid_voltage\":";
  message += state->grid_voltage;
  message += ",\"grid_current\":";
  message +=  state->grid_current;
  message += ",\"grid_freq\":";
  message += state->grid_freq;
  message += ",\"parallel_voltage\":";
  message += state->parallel_voltage;
  message += ",\"parallel_current\":";
  message += state->parallel_current;
  message += ",\"parallel_frequency\":";
  message += state->parallel_frequency;
  message += ",\"battery_voltage\":";
  message += state->battery_voltage;
  message += ",\"batt_power\":";
  message += (float)abs(state->battery_voltage) * (float)abs(state->batt_charge_current)/1000;;
  message += ",\"batt_charge_current\":";
  message += state->batt_charge_current;
  message += ",\"batt_charge_current\":";
  message += state->batt_charge_current;
  message += ",\"t0041\":";
  message +=  state->t0041;
  message +=  ",\"t0042\":";
  message +=  state->t0042;
  message +=  ",\"pv_voltage\":";
  message +=  state->pv_voltage;
  message +=  ",\"pv_current\":";
  message +=  state->pv_current;
  message +=  ",\"pv_power\":";
  message +=  state->pv_power;
  message +=  ",\"bus_voltage\":";
  message +=  state->bus_voltage;
  message +=  ",\"t0047\":";
  message +=  state->t0047;
  message +=  ",\"t0048\":";
  message += state->t0048;
  message +=  ",\"inverter_voltage_dc_component\":";
  message +=  state->inverter_voltage_dc_component;
  message +=  ",\"fan1_speed_percent\":";
  message +=  state->fan1_speed_percent;
  message +=  ",\"fan2_speed_percent\":";
  message +=  state->fan2_speed_percent;
  message +=  ",\"ntc2_temperature\":";
  message +=  state->ntc2_temperature;
  message +=  ",\"ntc3_temperature\":";
  message +=  state->ntc3_temperature;
  message +=  ",\"ntc4_temperature\":";
  message +=  state->ntc4_temperature;
  message +=  ",\"bts_temperature\":";
  message +=  state->bts_temperature;
  message +=  ",\"bms_battery_soc\":";
  message +=  state->bms_battery_soc;
  message +=  ",\"bms_battery_voltage\":";
  message +=  state->bms_battery_voltage;
  message +=  ",\"bms_battery_current\":";
  message +=  state->bms_battery_current;
  message +=  ",\"bms_01cell_voltage\":";
  message +=  state->bms_01cell_voltage;
  message +=  ",\"bms_02cell_voltage\":";
  message +=  state->bms_02cell_voltage;
  message +=  ",\"bms_03cell_voltage\":";
  message +=  state->bms_03cell_voltage;
  message +=  ",\"bms_04cell_voltage\":";
  message +=  state->bms_04cell_voltage;
  message +=  ",\"bms_05cell_voltage\":";
  message +=  state->bms_05cell_voltage;
  message +=  ",\"bms_06cell_voltage\":";
  message +=  state->bms_06cell_voltage;
  message +=  ",\"bms_07cell_voltage\":";
  message +=  state->bms_07cell_voltage;
  message +=  ",\"bms_08cell_voltage\":";
  message +=  state->bms_08cell_voltage;
  message +=  ",\"bms_09cell_voltage\":";
  message +=  state->bms_09cell_voltage;
  message +=  ",\"bms_10cell_voltage\":";
  message +=  state->bms_10cell_voltage;
  message +=  ",\"bms_11cell_voltage\":";
  message +=  state->bms_11cell_voltage;
  message +=  ",\"bms_12cell_voltage\":";
  message +=  state->bms_12cell_voltage;
  message +=  ",\"bms_13cell_voltage\":";
  message +=  state->bms_13cell_voltage;
  message +=  ",\"bms_14cell_voltage\":";
  message +=  state->bms_14cell_voltage;
  message +=  ",\"bms_15cell_voltage\":";
  message +=  state->bms_15cell_voltage;
  message +=  ",\"bms_16cell_voltage\":";
  message +=  state->bms_16cell_voltage;
  message +=  "}";
  return message;
}

String PowMr_data::convertToJSON(PowMr_energy Mr_energy){
  String message = "";
  message += "{";
  message += "\"load_energy\":";
  message += Mr_energy.load_energy;
  message += ",\"load_watt\":";
  message += Mr_energy.load_watt;
  message += ",\"batt_energy\":";
  message += Mr_energy.batt_energy;
  message += ",\"batt_energy_charge\":";
  message += Mr_energy.batt_energy_charge;
  message += ",\"batt_energy_discharge\":";
  message += Mr_energy.batt_energy_discharge;
  message += ",\"batt_power\":";
  message += Mr_energy.batt_power;
  message += ",\"batt_power_charge\":";
  message += Mr_energy.batt_power_charge;
  message += ",\"batt_power_discharge\":";
  message += Mr_energy.batt_power_discharge;
  message += ",\"pv_energy\":";
  message += Mr_energy.pv_energy;
  message += ",\"pv_power\":";
  message += Mr_energy.pv_power;
  message += ",\"t0026_total_energy\":";
  message += Mr_energy.t0026_total_energy;
  message += ",\"t0026_total_power\":";
  message += Mr_energy.t0026_total_power;
  message += ",\"duration\":";
  message += Mr_energy.duration;
  message +=  "}";
  return message;
}

inv8851_state_s* PowMr_data::read_data_value(){
  mySerial->begin(9600,SWSERIAL_8N1,pin_rec,pin_send,false,256,0);
  sendStateReq();
  inv8851_state_s* stateResp;
  delay(200);
  bool read = readState(respStore);
  Serial.println("readState");
  Serial.println(read);
  if(read == true){
    Serial.println("readState true");
    stateResp = (inv8851_state_s*) respStore;
    if(stateResp==NULL){
      Serial.println("readState stateResp NULL");}
    return stateResp;}
  return NULL;
}

String PowMr_data::read_data(){
  inv8851_state_s* stateResp = read_data_value() ;
  if(stateResp == NULL){
    Serial.println("read_data stateResp NULL");
    return "{\"status\":\"disconnected\"}";}
  String respjson = convertToJSON(stateResp);
  return respjson;
}

PowMr_energy PowMr_data::initEnergy(){
  pm_energy.batt_energy = 0;
  pm_energy.batt_energy_charge = 0;
  pm_energy.batt_energy_discharge = 0;
  pm_energy.batt_power = 0;
  pm_energy.batt_power_charge = 0;
  pm_energy.batt_power_discharge = 0;
  pm_energy.load_energy = 0;
  pm_energy.load_watt = 0;
  pm_energy.pv_energy = 0;
  pm_energy.pv_power = 0;
  pm_energy.t0026_total_energy = 0;
  pm_energy.t0026_total_power = 0;
  pm_energy.duration = 0;
  pm_energy.last_record = millis()/1000;
  return pm_energy;
}

PowMr_energy PowMr_data::readEnergyClean(){
  PowMr_energy pm_energyaux = readEnergy();
  pm_energy.batt_energy = 0;
  pm_energy.batt_energy_charge = 0;
  pm_energy.batt_energy_discharge = 0;
  pm_energy.load_energy = 0;
  pm_energy.pv_energy = 0;
  pm_energy.t0026_total_energy = 0;
  pm_energy.duration = 0;
  return pm_energyaux;
}

PowMr_energy PowMr_data::readEnergy(){
  unsigned long duration;
  duration = millis()/1000 - pm_energy.last_record;
  pm_energy.duration += duration;
  pm_energy.last_record = millis()/1000;
  inv8851_state_s* stateResp = read_data_value();
  if(stateResp == NULL){
    Serial.println("readEnergy 0 stateResp NULL");
    inv8851_state_s* stateResp = read_data_value();
    if(stateResp == NULL){
      Serial.println("readEnergy 1 stateResp NULL");
        pm_energy.batt_power = 0;
        pm_energy.load_watt = 0;
        pm_energy.pv_power = 0;
        pm_energy.t0026_total_power = 0;
      return pm_energy;}
  }
  if(stateResp->software_version != SOFTWARE_VERSION) {
    delay(200);
    Serial.println("readEnergy SOFTWARE_VERSION !="+SOFTWARE_VERSION);
    stateResp = read_data_value();
  }
  if(stateResp->software_version == SOFTWARE_VERSION) {
    float curr_batt_power = (float)(stateResp->battery_voltage) * (float)(stateResp->batt_charge_current)/1000; // 100 for v, 10 for A
    pm_energy.batt_energy += ((float)curr_batt_power + (float)pm_energy.batt_power)/2*duration/3600;
    if(curr_batt_power>0){
      pm_energy.batt_energy_charge += ((float)curr_batt_power + (float)pm_energy.batt_power_charge)/2*duration/3600;
      pm_energy.batt_power_charge = curr_batt_power;
      pm_energy.batt_power_discharge = 0;
    }
    else{
      pm_energy.batt_energy_discharge += ((float)curr_batt_power + (float)pm_energy.batt_power_discharge)/2*duration/3600;
      pm_energy.batt_power_charge = 0;
      pm_energy.batt_power_discharge = curr_batt_power;
    }
    pm_energy.batt_power = curr_batt_power;
    pm_energy.load_energy += ((float)abs(stateResp->load_watt) + (float)pm_energy.load_watt)/2*duration/3600;
    pm_energy.load_watt = abs(stateResp->load_watt);
    pm_energy.pv_energy += ((float)abs(stateResp->pv_power) + (float)pm_energy.pv_power)/2*duration/3600; // /10 for voltage /100 for current
    pm_energy.pv_power = abs(stateResp->pv_power);
    pm_energy.t0026_total_energy += ((float)abs(stateResp->t0026) + (float)pm_energy.t0026_total_power)/2*duration/3600;
    pm_energy.t0026_total_power = abs(stateResp->t0026);}
  return pm_energy;
}


