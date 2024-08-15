"""
Driver for the PowMr Solar Controller using the Modbus RTU protocol
"""

import minimalmodbus
import time
from datetime import datetime
import json
import serial
#from influxdb_client import InfluxDBClient, Point, WritePrecision
#from influxdb_client.client.write_api import SYNCHRONOUS

q="88510003000090008533810b0000000034000000000000000000000000000000000010000c00000000000000000000000000f4089f0088136c01c7008b008700080403035700000003000000060000000000000000004d09030000000000d20608000700c50d0000fcff00000000303130190000000000000000000000000000000000000000000000000000000000000000000000000000cb2a"
print(len(q))
ser = serial.Serial('COM6', 2400, bytesize=serial.EIGHTBITS, stopbits=1, timeout=2,parity=serial.PARITY_NONE)

print(ser.name)         # check which port was really used
state = bytes.fromhex('88510003000000004d08')
print(state)
print(len(state))
# Convert to Integer
#int_value_state = int(state, base=16)
#print(int_value_state)
#bin_value_state = bin(int_value_state)
no_wrote = ser.write(state)     # write a string
print('wrote '+str(no_wrote))
totaldata=""
total_hes = []
for i in range(1000):
    data = ser.read()
    print(data.hex())
    totaldata+=str(data.hex())
    total_hes.append(str(data.hex()))
    if len(data) == 0:
        break
ser.close()             # close port
print(totaldata)
print(total_hes[37])
print(total_hes[50])
