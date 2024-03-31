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

minimalmodbus.BAUDRATE = 9600
minimalmodbus.TIMEOUT = 2


registers = {
    "Grid Voltage": {'register':531, 'multiplier':0.1, 'unit':'V'},
    "Grid Current": {'register':532, 'multiplier':0.1, 'unit':'A'},
    
    "Ambient Temperature": {'register':546, 'multiplier':0.1, 'unit':'C'},

    "PV Temperature": {'register':544, 'multiplier':0.1, 'unit':'C'},
    "Solar Voltage": {'register':263, 'multiplier':0.1, 'unit':'V'},
    "Solar Current": {'register':548, 'multiplier':0.1, 'unit':'A'},
    "Solar Power": {'register':265, 'multiplier':1, 'unit':'W'},
    
    "Battery Voltage": {'register':257, 'multiplier':0.1, 'unit':'V'},
    "Battery Current In": {'register':0, 'multiplier':1, 'unit':'A'},
    "Battery Power In": {'register':270, 'multiplier':0.1, 'unit':'W'},
    "Battery Current Out": {'register':0, 'multiplier':0.1, 'unit':'A'},
    "Battery Power Out": {'register':0, 'multiplier':1, 'unit':'W'},
    
    "Inv Temperature": {'register':545, 'multiplier':0.1, 'unit':'C'},
    "Inv Current": {'register':537, 'multiplier':0.1, 'unit':'A'},
    "Inv Voltage": {'register':534, 'multiplier':0.1, 'unit':'V'},
    "Inv VA": {'register':540, 'multiplier':1, 'unit':'VA'},
    "Inv Power": {'register':539, 'multiplier':1, 'unit':'W'}
}

class PowMr(minimalmodbus.Instrument):
    """
    Communicates using the Modbus RTU protocol (via provided USB<->RS232 cable)
    """

    def __init__(self, portname, slaveaddress):
        minimalmodbus.Instrument.__init__(self, portname, slaveaddress)

    def dump_registers(self):
        registers = {}
        for r in list(range(600)) + list(range(4400,4600)):
            try:
                if r%100 == 0:
                    print('readregister '+str(r))
                register = self.read_register(r)
                registers[str(r)]=str(register)
                print(f"Register {r} \t=>  {register}")
            except Exception as e:
                #print(f"Register {r} \t=>   Exception "+str(e))
                pass
        return registers

    
    def dump_register(self, reg):
        try:
            register = self.read_register(reg)
            print(f"Register {reg} \t=>  {register}")
        except:
            print(f"Register {reg} \t=>   Exception")


    def full_report(self):
        for key, val in registers.items():
            try:
                measurement  = int(self.read_register(val['register'])) * val['multiplier']
                print(f"{key}: {measurement}{val['unit']}")
                time.sleep(.1)
            except Exception as e:
                print(e)

               
        
if __name__ == "__main__":
    print(minimalmodbus._get_diagnostic_string())
    # You can generate a Token from the "Tokens Tab" in the UI
    token = "<YOUR_TOKEN_HERE>"
    org = "home"
    bucket = "solar_controller"
    #client = InfluxDBClient(url="http://localhost:8086", token=token)
    #write_api = client.write_api(write_options=SYNCHRONOUS)

    for i in range(257):
        powmr = PowMr('COM4', i)
        powmr.serial.baudrate = 2400
        #powmr.clear_buffers_before_each_transaction = True
        #powmr.debug = True
        #powmr.serial.port # this is the serial port name
        #powmr.serial.baudrate = 9600 # Baud
        #powmr.serial.bytesize = 8
        #powmr.serial.parity = serial.PARITY_EVEN
        powmr.serial.stopbits = 1
        #instrument.serial.timeout = 0.05 # seconds

        #instrument.address # this is the slave address number
        #powmr.mode = minimalmodbus.MODE_RTU # rtu or ascii mode
        try:
                register = powmr.read_registers(4501,45)
                print(f"Register {i} \t=>  {register}")
        except Exception as e:
                print(f"Register {i} \t=>   Exception "+str(e))
                pass
    powmr = PowMr('COM4', 1)
    #powmr.read_registers(4501,45)
    powmr.serial.baudrate = 2400
    #powmr.clear_buffers_before_each_transaction = True
    #powmr.debug = True
    data = powmr.dump_registers()
    out_file = open("dataValues.json", "w") 
  
    json.dump(data, out_file, indent = 6) 
  
    out_file.close() 
    '''while True:
        for key, val in registers.items():
            try:
                print('reading')
                measurement  = int(powmr.read_register(val['register'])) * val['multiplier']
                #write_api.write(bucket, org, Point("powmr").tag("host", "controller").field(key, measurement).time(datetime.utcnow(), WritePrecision.NS))
                time.sleep(1)
            except Exception as e:
                print(e)'''



# from solarshed.controllers.powmr import PowMr
# pow =  PowMr('/dev/ttyUSB1', 1)
# pow.dump_registers()
