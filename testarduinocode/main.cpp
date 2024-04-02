#include <iostream>
#include <stdio.h>
#include <string.h>
#include "inv8851.h"
using namespace std;

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

void HexStringToBytes(char *req_hex_state,unsigned char *req,unsigned int *len,int hexlen){
  if(strlen(req_hex_state)%2!=0) return;
  for(int i=0;i<hexlen;i++)
  { if(i%2==0){
        char byteval = converthexdigit(req_hex_state[i+1]);
        byteval+=converthexdigit(req_hex_state[i])*16;
        cout<<req_hex_state[i]<<" "<<req_hex_state[i+1]<<" "<<byteval<<" "<<*len<<endl;
        req[*len]=byteval;
        *len= *len + 1;
        //cout<<req<<" "<<" "<<*len<<endl;
  }
  }

}

void generateJSON(inv8851_state_s *state){
  cout<< "{";
  cout<< "\"proto\":";
  cout<< state->proto;
  cout<< ",\"command\":";
  cout<< state->command;
  cout<< ",\"addr\":";
  cout<< state->addr;
  cout<< ",\"data_size\":";
  cout<< state->data_size;
  cout<<  ",\"words\":";
  cout<<  state->words;
  cout<< ",\"run_mode\":";
  cout<< state->run_mode;
  cout<< ",\"inverter_topology\":";
  cout<< state->inverter_topology;
  cout<< ",\"llc_topology\":";
  cout<< state->llc_topology;
  cout<< ",\"pv_topology\":";
  cout<< state->pv_topology;
  cout<< ",\"buck_topology\":";
  cout<< state->buck_topology;
  cout<< ",\"system_power\":";
  cout<< (int)state->system_power;
  cout<< ",\"charge_finish\":";
  cout<< (int)state->charge_finish;
  cout<< ",\"bus_ok\":";
  cout<< (int)state->bus_ok;
  cout<< ",\"bus_n_grid_voltage_match\":";
  cout<< (int)state->bus_n_grid_voltage_match;
  cout<< ",\"no_battery\":";
  cout<< (int)state->no_battery;
  cout<< ",\"pv_excess\":";
  cout<< (int)state->pv_excess;
  cout<< ",\"floating_charge\":";
  cout<< (int)state->floating_charge;
  cout<< ",\"system_initial_finished\":";
  cout<< (int)state->system_initial_finished;
  cout<< ",\"inverter_topology_initial_finished\":";
  cout<< (int)state->inverter_topology_initial_finished;
  cout<< ",\"llc_topology_initial_finished\":";
  cout<< (int)state->llc_topology_initial_finished;
  cout<< ",\"pv_topology_initial_finished\":";
  cout<< (int)state->pv_topology_initial_finished;
  cout<< ",\"buck_topology_initial_finished\":";
  cout<< (int)state->buck_topology_initial_finished;
  cout<< ",\"eq_charge_start\":";
  cout<< (int)state->eq_charge_start;
  cout<< ",\"eq_charge_ready\":";
  cout<< (int)state->eq_charge_ready;
  cout<< ",\"w6\":";
  cout<< (int)state->w6;
  cout<< ",\"t0002\":";
  cout<< (int)state->t0002;
  cout<< ",\"t0003\":";
  cout<< (int)state->t0003;
  cout<< ",\"grid_pll_ok\":";
  cout<< (int)state->grid_pll_ok;
  cout<< ",\"lo0004\":";
  cout<< (int)state->lo0004;
  cout<< ",\"hi0004\":";
  cout<< (int)state->hi0004;
  cout<< ",\"disable_utility\":";
  cout<< (int)state->disable_utility;
  cout<< ",\"hi0004_\":";
  cout<< (int)state->hi0004_;
  cout<< ",\"t0005\":";
  cout<< (int)state->t0005;
  cout<< ",\"t0006\":";
  cout<< (int)state->t0006;
  cout<< ",\"t0007\":";
  cout<< (int)state->t0007;
  cout<< ",\"t0008\":";
  cout<< (int)state->t0008;
  cout<< ",\"t0009\":";
  cout<< (int)state->t0009;
  cout<< ",\"t0010\":";
  cout<< (int)state->t0010;
  cout<< ",\"t0011\":";
  cout<< (int)state->t0011;

  cout<< ",\"lo0013\":";
  cout<< (int)state->lo0013;
  cout<< ",\"pv_input_ok\":";
  cout<< (int)state->pv_input_ok;
  cout<< ",\"lo0013_\":";
  cout<< (int)state->lo0013_;
  cout<< ",\"parallel_lock_phase_ok\":";
  cout<< (int)state->parallel_lock_phase_ok;
  cout<< ",\"hi0013\":";
  cout<< (int)state->hi0013;

  cout<< ",\"software_version\":";
  cout<< (int)state->software_version;
  cout<< ",\"log_number\":";
  cout<< (int)state->log_number;
  cout<< ",\"t0016\":";
  cout<< (int)state->t0016;
  cout<< ",\"t0017\":";
  cout<< (int)state->t0017;
  cout<< ",\"t0018\":";
  cout<< (int)state->t0018;
  cout<< ",\"t0019\":";
  cout<< (int)state->t0019;
  cout<< ",\"t0020\":";
  cout<< (int)state->t0020;
  cout<< ",\"inv_voltage\":";
  cout<< (int)state->inv_voltage;
  cout<< ",\"inv_current\":";
  cout<< (int)state->inv_current;
  cout<< ",\"inv_freq\":";
  cout<<(int)state->inv_freq;
  cout<< ",\"inv_va\":";
  cout<< (int)state->inv_va;
  cout<< ",\"load_va\":";
  cout<< (int)state->load_va;
  cout<< ",\"t0026\":";
  cout<< (int)state->t0026;
  cout<< ",\"load_watt\":";
  cout<< (int)state->load_watt;
  cout<< ",\"inverter_va_percent\":";
  cout<<(int)state->inverter_va_percent;
  cout<< ",\"inverter_watt_percent\":";
  cout<< (int)state->inverter_watt_percent;
  cout<< ",\"load_current\":";
  cout<< (int)state->load_current;
  cout<< ",\"low_load_current\":";
  cout<< (int)state->low_load_current;
  cout<< ",\"t0032\":";
  cout<< (int)state->t0032;
  cout<< ",\"grid_voltage\":";
  cout<< (int)state->grid_voltage;
  cout<< ",\"grid_current\":";
  cout<<(int) state->grid_current;
  cout<< ",\"grid_freq\":";
  cout<< (int)state->grid_freq;
  cout<< ",\"parallel_voltage\":";
  cout<< (int)state->parallel_voltage;
  cout<< ",\"parallel_current\":";
  cout<< (int)state->parallel_current;
  cout<< ",\"parallel_frequency\":";
  cout<< (int)state->parallel_frequency;
  cout<< ",\"battery_voltage\":";
  cout<< (int)state->battery_voltage;
  cout<< ",\"batt_charge_current\":";
  cout<< (int)state->batt_charge_current;
  cout<< ",\"batt_charge_current\":";
  cout<< (int)state->batt_charge_current;
  cout<< ",\"t0041\":";
  cout<< (int)state->t0041;
  cout<< ",\"t0042\":";
  cout<< (int)state->t0042;
  cout<< ",\"pv_voltage\":";
  cout<< (int)state->pv_voltage;
  cout<< ",\"pv_current\":";
  cout<< (int)state->pv_current;
  cout<< ",\"pv_power\":";
  cout<< (int)state->pv_power;
  cout<< ",\"bus_voltage\":";
  cout<< (int)state->bus_voltage;
  cout<< ",\"t0047\":";
  cout<< (int)state->t0047;
  cout<< ",\"t0048\":";
  cout<<(int)state->t0048;
  cout<< ",\"inverter_voltage_dc_component\":";
  cout<< (int)state->inverter_voltage_dc_component;
  cout<< ",\"fan1_speed_percent\":";
  cout<< (int)state->fan1_speed_percent;
  cout<< ",\"fan2_speed_percent\":";
  cout<< (int)state->fan2_speed_percent;
  cout<< ",\"ntc2_temperature\":";
  cout<< (int)state->ntc2_temperature;
  cout<< ",\"ntc3_temperature\":";
  cout<< (int)state->ntc3_temperature;
  cout<< ",\"ntc4_temperature\":";
  cout<< (int)state->ntc4_temperature;
  cout<< ",\"bts_temperature\":";
  cout<< (int)state->bts_temperature;
  cout<< ",\"bms_battery_soc\":";
  cout<< (int)state->bms_battery_soc;
  cout<< ",\"bms_battery_voltage\":";
  cout<< (int)state->bms_battery_voltage;
  cout<< ",\"bms_battery_current\":";
  cout<< (int)state->bms_battery_current;
  cout<< ",\"bms_01cell_voltage\":";
  cout<< (int)state->bms_01cell_voltage;
  cout<< ",\"bms_02cell_voltage\":";
  cout<< (int)state->bms_02cell_voltage;
  cout<< ",\"bms_03cell_voltage\":";
  cout<< (int)state->bms_03cell_voltage;
  cout<< ",\"bms_04cell_voltage\":";
  cout<< (int)state->bms_04cell_voltage;
  cout<< ",\"bms_05cell_voltage\":";
  cout<< (int)state->bms_05cell_voltage;
  cout<< ",\"bms_06cell_voltage\":";
  cout<< (int)state->bms_06cell_voltage;
  cout<< ",\"bms_07cell_voltage\":";
  cout<< (int)state->bms_07cell_voltage;
  cout<< ",\"bms_08cell_voltage\":";
  cout<< (int)state->bms_08cell_voltage;
  cout<< ",\"bms_09cell_voltage\":";
  cout<< (int)state->bms_09cell_voltage;
  cout<< ",\"bms_10cell_voltage\":";
  cout<< (int)state->bms_10cell_voltage;
  cout<< ",\"bms_11cell_voltage\":";
  cout<< (int)state->bms_11cell_voltage;
  cout<< ",\"bms_12cell_voltage\":";
  cout<< (int)state->bms_12cell_voltage;
  cout<< ",\"bms_13cell_voltage\":";
  cout<< (int)state->bms_13cell_voltage;
  cout<< ",\"bms_14cell_voltage\":";
  cout<< (int)state->bms_14cell_voltage;
  cout<< ",\"bms_15cell_voltage\":";
  cout<< (int)state->bms_15cell_voltage;
  cout<< ",\"bms_16cell_voltage\":";
  cout<<(int) state->bms_16cell_voltage;
  cout<< "}";
}

int main()
{   unsigned char req[12];
    unsigned char resp[158];
    unsigned int req_len = 0,resp_len=0;
    //cout << "Hello world!1" << endl;
    char req_hex_state[] = "88510003000000004d08";
    char respValue[] = "88510003000094008830910b0000000001000000000000000000000000000000000000000f170000000000000000000000009008220088134a000800ceff00000100000004000000adff940825008613000000000000010b0f000000000087000d00000092110000010000000000342e25190000000000000000000000000000000000000000000000000000000000000000000000000000000000007ede";
    cout << strlen(req_hex_state) << endl<<endl;

    HexStringToBytes(req_hex_state, req, &req_len,20);

    cout<<"Converted Value "<< req<<" "<<req_len<< endl;

    cout<<"sizeof(inv8851_state_s) "<<sizeof(inv8851_state_s)<<endl;
    cout<<"sizeof(inv8851_state_s_reverse) "<<sizeof(inv8851_state_s_reverse)<<endl;
    cout << strlen(respValue) << endl<<endl;

    HexStringToBytes(respValue, resp, &resp_len,316);

    cout<<"Converted Value "<< resp<<" "<<resp_len<< endl;
    inv8851_state_s *test_state = (inv8851_state_s*) resp;
    inv8851_state_s_reverse  *test_state_reverse = (inv8851_state_s_reverse*) resp;
    cout<<"inv_voltage|"<<resp[25]<<endl;
    cout<<"test_state "<< test_state->inv_voltage <<" "<<test_state->inv_freq<<" "<<test_state->software_version<<" "<<(char)(test_state->no_battery+'0')<<" "<<(int)(test_state->inverter_voltage_dc_component)<< endl;

    cout<<"test_state_reverse "<< test_state_reverse->t0017 <<" "<< test_state_reverse->t0018 <<" "<< test_state_reverse->t0019 <<" "<< test_state_reverse->t0020 <<" "<< test_state_reverse->inv_voltage <<" "<< test_state_reverse->inv_current <<" "<< test_state_reverse->inv_freq <<" "<< test_state_reverse->inv_va<<" "<< test_state_reverse->load_va <<" "<< endl;

    generateJSON(test_state);
    return 0;
}
