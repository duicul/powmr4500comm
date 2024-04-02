import json
values = ["88510003000094008830910b0000000001000000000000000000000000000000000000000f170000000000000000000000009008220088134a000800ceff00000100000004000000adff940825008613000000000000010b0f000000000087000d00000092110000010000000000342e25190000000000000000000000000000000000000000000000000000000000000000000000000000000000007ede",
          "88510003000094008830910b0000000001000000000000000000000000000000000000000f1700000000000000000000000092082d00881362000600aaff00000200000003000000d8ff9b0812008513000000000000ff0a1000000000008700080000008f110000010000000000312e22190000000000000000000000000000000000000000000000000000000000000000000000000000000000004f23"]

def hex_to_little_endian(hex_string):
    print(hex_string)
    print(bytearray.fromhex(hex_string))
    little_endian_hex = bytearray.fromhex(hex_string)[::-1]
    return little_endian_hex

split_lists = []
for value in values:
    split_size = 4
    split_list =[]
    print(len(value))
    for i in range(0,len(value),4):
        if i+split_size > len(value):
            last_string=value[i:len(value)]
            added_zero = i+split_size+1 - len(value)
            
            split_list.append(last_string+(added_zero * '0'))
        else:
            split_list.append(value[i:i+split_size])
            i=i+split_size
    print(split_list)
    print("batt voltage: "+str(int(split_list[int(172/4)],16)))
    print("grid voltage: "+str(int(split_list[int(148/4)],16)))
    print("grid voltage1: "+str(int(split_list[4],16)))
    split_lists.append(split_list)

for i in range(len(split_lists[0])):
    if(split_lists[0][i]!=split_lists[1][i]):
        print(str(i)+" "+str(split_lists[0][i])+ " <-> "+str(split_lists[1][i]))
        print(str(i)+" "+str(int(split_lists[0][i],16))+ " <-> "+str(int(split_lists[1][i],16)))
        print()

read_index = {"software_version":18,
              "log_number":19,
              "inv_voltage":25,
              "inv_current":26,
              "inv_freq":27,
              "inv_va":28,
              "load_va":29,
               "t0026":30,
              "load_watt":31,
             "inverter_va_percent":32,
             "inverter_watt_percent":33,
             "load_current":34,
             "low_load_current":35,
             "t0032":36,
             "grid_voltage":37,
             "grid_current":38,
             "grid_freq":39,
             "parallel_voltage":40,
             "parallel_current":41,
             "parallel_frequency":42,
             "batt_voltage":43,
             "batt_charge_current":44,
             "t0041":45,
             "t0042":46,
             "pv_voltage":47,
             "pv_current":48,
             "pv_power":49,
             "bus_voltage":50,
             "t0047":51,
             "t0048":52,
             "inverter_voltage_dc_component":53}
'''
              "grid_voltage":38,
              "pv_power":49,
              "batt_voltage":43}'''
dataExt = []
for i in range(len(values)):
    data_aux = {}#"data":split_lists[i]}
    for j in read_index:
        k=read_index[j]        
        print(str(j)+" "+str(int(split_lists[i][k],16))+ " <-> "+str(split_lists[i][k]))
        int_val = int(split_lists[i][k],16)
        little_edian = hex_to_little_endian(split_lists[i][k])
        print(little_edian)
        print(int(little_edian.hex(),16))
        data_aux[j]=int(little_edian.hex(),16)
    print()
    dataExt.append(data_aux)
out_file = open("dataValuesExt.json", "w") 
  
json.dump(dataExt, out_file, indent = 6) 

out_file.close()
