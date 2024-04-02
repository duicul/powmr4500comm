#include "inv8851.h"
#include <SoftwareSerial.h>
#define RX 3
#define TX 1
SoftwareSerial mySerial(RX, TX); // RX, TX

char converthexdigit(char hexdigit){
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

void HexStringToBytes(unsigned char *req_hex_state,unsigned char *req,unsigned int *len,int hexlen){
  for(int i=0;i<hexlen;i++)
  { if(i%2==0){
        char byteval = converthexdigit(req_hex_state[i+1]);
        byteval+=converthexdigit(req_hex_state[i])*16;
        //cout<<req_hex_state[i]<<" "<<req_hex_state[i+1]<<" "<<byteval<<" "<<*len<<endl;
        req[*len]=byteval;
        *len= *len + 1;
        //cout<<req<<" "<<" "<<*len<<endl;
  }
  }

}

void setup() {
  Serial.begin(9600);
  Serial.println(sizeof(inv8851_state_s));
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
}

void sendStateReq(){
  unsigned char req[12];
  unsigned int req_len = 0;

  unsigned char req_hex_state[] = "88510003000000004d08";

  HexStringToBytes(req_hex_state, req, &req_len,20);
  mySerial.write(req, req_len); 
}

inv8851_state_s* readState(){
  unsigned char resp[160];
  unsigned int resp_len=0;
  int cnt = 0;
  while(mySerial.available() && cnt++<500){
    resp[resp_len++]=mySerial.read();
  }

  inv8851_state_s *test_state = (inv8851_state_s*) resp;
  return test_state;
}

String convertToJSON(inv8851_state_s *state){
  String message = "";
  message += "{";
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
  message += state->inverter_topology;
  message += ",\"llc_topology\":";
  message += state->llc_topology;
  message += ",\"pv_topology\":";
  message += state->pv_topology;
  message += ",\"buck_topology\":";
  message += state->buck_topology;
  message += ",\"system_power\":";
  message += (int)state->system_power;
  message += ",\"charge_finish\":";
  message += (int)state->charge_finish;
  message += ",\"bus_ok\":";
  message += (int)state->bus_ok;
  message += ",\"bus_n_grid_voltage_match\":";
  message += (int)state->bus_n_grid_voltage_match;
  message += ",\"no_battery\":";
  message += (int)state->no_battery;
  message += ",\"pv_excess\":";
  message += (int)state->pv_excess;
  message += ",\"floating_charge\":";
  message += (int)state->floating_charge;
  message += ",\"system_initial_finished\":";
  message += (int)state->system_initial_finished;
  message += ",\"inverter_topology_initial_finished\":";
  message += (int)state->inverter_topology_initial_finished;
  message += ",\"llc_topology_initial_finished\":";
  message += (int)state->llc_topology_initial_finished;
  message += ",\"pv_topology_initial_finished\":";
  message += (int)state->pv_topology_initial_finished;
  message += ",\"buck_topology_initial_finished\":";
  message += (int)state->buck_topology_initial_finished;
  message += ",\"eq_charge_start\":";
  message += (int)state->eq_charge_start;
  message += ",\"eq_charge_ready\":";
  message += (int)state->eq_charge_ready;
  message += ",\"w6\":";
  message += (int)state->w6;
  message += ",\"t0002\":";
  message += (int)state->t0002;
  message += ",\"t0003\":";
  message += (int)state->t0003;
  message += ",\"grid_pll_ok\":";
  message += (int)state->grid_pll_ok;
  message += ",\"lo0004\":";
  message += (int)state->lo0004;
  message += ",\"hi0004\":";
  message += (int)state->hi0004;
  message += ",\"disable_utility\":";
  message += (int)state->disable_utility;
  message += ",\"hi0004_\":";
  message += (int)state->hi0004_;
  message += ",\"t0005\":";
  message += (int)state->t0005;
  message += ",\"t0006\":";
  message += (int)state->t0006;
  message += ",\"t0007\":";
  message += (int)state->t0007;
  message += ",\"t0008\":";
  message += (int)state->t0008;
  message += ",\"t0009\":";
  message += (int)state->t0009;
  message += ",\"t0010\":";
  message += (int)state->t0010;
  message += ",\"t0011\":";
  message += (int)state->t0011;

  message += ",\"lo0013\":";
  message += (int)state->lo0013;
  message += ",\"pv_input_ok\":";
  message += (int)state->pv_input_ok;
  message += ",\"lo0013_\":";
  message += (int)state->lo0013_;
  message += ",\"parallel_lock_phase_ok\":";
  message += (int)state->parallel_lock_phase_ok;
  message += ",\"hi0013\":";
  message += (int)state->hi0013;

  message += ",\"software_version\":";
  message += (int)state->software_version;
  message += ",\"log_number\":";
  message += (int)state->log_number;
  message += ",\"t0016\":";
  message += (int)state->t0016;
  message += ",\"t0017\":";
  message += (int)state->t0017;
  message += ",\"t0018\":";
  message += (int)state->t0018;
  message += ",\"t0019\":";
  message += (int)state->t0019;
  message += ",\"t0020\":";
  message += (int)state->t0020;
  message += ",\"inv_voltage\":";
  message += (int)state->inv_voltage;
  message += ",\"inv_current\":";
  message += (int)state->inv_current;
  message += ",\"inv_freq\":";
  message += (int)state->inv_freq;
  message += ",\"inv_va\":";
  message += (int)state->inv_va;
  message += ",\"load_va\":";
  message += (int)state->load_va;
  message += ",\"t0026\":";
  message += (int)state->t0026;
  message += ",\"load_watt\":";
  message += (int)state->load_watt;
  message += ",\"inverter_va_percent\":";
  message += (int)state->inverter_va_percent;
  message += ",\"inverter_watt_percent\":";
  message += (int)state->inverter_watt_percent;
  message += ",\"load_current\":";
  message += (int)state->load_current;
  message += ",\"low_load_current\":";
  message += (int)state->low_load_current;
  message += ",\"t0032\":";
  message += (int)state->t0032;
  message += ",\"grid_voltage\":";
  message += (int)state->grid_voltage;
  message += ",\"grid_current\":";
  message += (int) state->grid_current;
  message += ",\"grid_freq\":";
  message += (int)state->grid_freq;
  message += ",\"parallel_voltage\":";
  message += (int)state->parallel_voltage;
  message += ",\"parallel_current\":";
  message += (int)state->parallel_current;
  message += ",\"parallel_frequency\":";
  message += (int)state->parallel_frequency;
  message += ",\"battery_voltage\":";
  message += (int)state->battery_voltage;
  message += ",\"batt_charge_current\":";
  message += (int)state->batt_charge_current;
  message += ",\"batt_charge_current\":";
  message += (int)state->batt_charge_current;
  message += ",\"t0041\":";
  message +=  (int)state->t0041;
  message +=  ",\"t0042\":";
  message +=  (int)state->t0042;
  message +=  ",\"pv_voltage\":";
  message +=  (int)state->pv_voltage;
  message +=  ",\"pv_current\":";
  message +=  (int)state->pv_current;
  message +=  ",\"pv_power\":";
  message +=  (int)state->pv_power;
  message +=  ",\"bus_voltage\":";
  message +=  (int)state->bus_voltage;
  message +=  ",\"t0047\":";
  message +=  (int)state->t0047;
  message +=  ",\"t0048\":";
  message += (int)state->t0048;
  message +=  ",\"inverter_voltage_dc_component\":";
  message +=  (int)state->inverter_voltage_dc_component;
  message +=  ",\"fan1_speed_percent\":";
  message +=  (int)state->fan1_speed_percent;
  message +=  ",\"fan2_speed_percent\":";
  message +=  (int)state->fan2_speed_percent;
  message +=  ",\"ntc2_temperature\":";
  message +=  (int)state->ntc2_temperature;
  message +=  ",\"ntc3_temperature\":";
  message +=  (int)state->ntc3_temperature;
  message +=  ",\"ntc4_temperature\":";
  message +=  (int)state->ntc4_temperature;
  message +=  ",\"bts_temperature\":";
  message +=  (int)state->bts_temperature;
  message +=  ",\"bms_battery_soc\":";
  message +=  (int)state->bms_battery_soc;
  message +=  ",\"bms_battery_voltage\":";
  message +=  (int)state->bms_battery_voltage;
  message +=  ",\"bms_battery_current\":";
  message +=  (int)state->bms_battery_current;
  message +=  ",\"bms_01cell_voltage\":";
  message +=  (int)state->bms_01cell_voltage;
  message +=  ",\"bms_02cell_voltage\":";
  message +=  (int)state->bms_02cell_voltage;
  message +=  ",\"bms_03cell_voltage\":";
  message +=  (int)state->bms_03cell_voltage;
  message +=  ",\"bms_04cell_voltage\":";
  message +=  (int)state->bms_04cell_voltage;
  message +=  ",\"bms_05cell_voltage\":";
  message +=  (int)state->bms_05cell_voltage;
  message +=  ",\"bms_06cell_voltage\":";
  message +=  (int)state->bms_06cell_voltage;
  message +=  ",\"bms_07cell_voltage\":";
  message +=  (int)state->bms_07cell_voltage;
  message +=  ",\"bms_08cell_voltage\":";
  message +=  (int)state->bms_08cell_voltage;
  message +=  ",\"bms_09cell_voltage\":";
  message +=  (int)state->bms_09cell_voltage;
  message +=  ",\"bms_10cell_voltage\":";
  message +=  (int)state->bms_10cell_voltage;
  message +=  ",\"bms_11cell_voltage\":";
  message +=  (int)state->bms_11cell_voltage;
  message +=  ",\"bms_12cell_voltage\":";
  message +=  (int)state->bms_12cell_voltage;
  message +=  ",\"bms_13cell_voltage\":";
  message +=  (int)state->bms_13cell_voltage;
  message +=  ",\"bms_14cell_voltage\":";
  message +=  (int)state->bms_14cell_voltage;
  message +=  ",\"bms_15cell_voltage\":";
  message +=  (int)state->bms_15cell_voltage;
  message +=  ",\"bms_16cell_voltage\":";
  message += (int) state->bms_16cell_voltage;
  message +=  "}";
  return message;
}

void loop() {
 sendStateReq();
 inv8851_state_s* stateResp = readState();
 String resp = convertToJSON(stateResp);
 Serial.println(resp);
 delay(10000);
}
