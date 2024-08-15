#include <ModbusMaster.h>


// instantiate ModbusMaster object
ModbusMaster node;


void setup()
{
  // use Serial (port 0); initialize Modbus communication baud rate
  Serial.begin(9600);

  // communicate with Modbus slave ID 2 over Serial (port 0)
  node.begin(0, Serial);
}


void loop()
{
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[500];

  i++;

  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node.readHoldingRegisters(0, 500);

  // do something with data if read is successful
  if (result == node.ku8MBSuccess)
  {
    for (j = 0; j < 6; j++)
    {
      data[j] = node.getResponseBuffer(j);
    }
  }
}