/* 
* i2c_port_address_scanner (fixed and expanded)
* Scans ports D0 to D8 on an ESP8266 and searches for I2C device. based on the original code
* available on Arduino.cc and later improved by user Krodal and Nick Gammon (www.gammon.com.au/forum/?id=10896)
* D8/GPIO15 will fail because of the pulldown resistor, thus it has been left out.
* Additionally, the original scanner used addresses 1-7 and 120-127 which are reserved addresses.
* https://www.instructables.com/ESP8266-I2C-PORT-and-Address-Scanner/
* https://github.com/jainrk/i2c_port_address_scanner
* 
* Serial uses UART0, which is mapped to pins GPIO1 (TX) and GPIO3 (RX). If you need to use GPIO1 or GPIO3 for 
* I2C then Serial may be remapped to GPIO15 (TX) and GPIO13 (RX) by calling Serial.swap() after Serial.begin.
* Calling Serial.swap() again maps UART0 back to GPIO1 and GPIO3.
*/

#include <Wire.h>

void setup() {
  Serial.begin(115200);
//  Serial.swap();  
  Serial.println("\n\nI2C Scanner to scan for devices on each port pair D0 to D7");
  scanPorts();
}

uint8_t portArray[] = {16, 5, 4, 0, 2, 14, 12, 13};
//uint8_t portArray[] = {16, 5, 4, 0, 1, 2, 3, 14, 12}; //if using Serial.swap()

//String portMap[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"}; //for D1 Mini
String portMap[] = {"GPIO16", "GPIO5", "GPIO4", "GPIO0", "GPIO2", "GPIO14", "GPIO12", "GPIO13"};
//String portMap[] = {"GPIO16", "GPIO5", "GPIO4", "GPIO0", "GPIO1", "GPIO2", "GPIO3", "GPIO14", "GPIO12"}; //if using Serial.swap()

void scanPorts() { 
  for (uint8_t i = 0; i < sizeof(portArray); i++) {
    for (uint8_t j = 0; j < sizeof(portArray); j++) {
      if (i != j){
        Serial.print("Scanning (SDA : SCL) - " + portMap[i] + " : " + portMap[j] + " - ");
        Wire.begin(portArray[i], portArray[j]);
        check_if_exist_I2C();
      }
    }
  }
}

void check_if_exist_I2C() {
  byte error, address;
  int nDevices;
  nDevices = 0;
  for (address = 8; address < 120; address++ )  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    delayMicroseconds(100);
	
    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  } //for loop
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("**********************************\n");
//  delay(1000);           // wait 1 seconds for next scan
}

void loop() {
}
