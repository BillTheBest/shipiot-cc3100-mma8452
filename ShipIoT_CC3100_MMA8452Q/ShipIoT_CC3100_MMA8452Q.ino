// ShipIoT with TI CC3100 wifi module and a MMA8452Q 3 axis 

#ifndef __CC3200R1M1RGC__ 
// the CC3200 doesn't use SPI
#include <SPI.h>            // SPI connects to the CC3100 (NB: this uses the same pins as I2C0
#endif
#include <WiFi.h>           // WiFi allows us to use the CC3100
#include <ShipIoT.h>        // ShipIoT client library
#include <Wire.h>           // Must include Wire library for I2C
#include <SFE_MMA8452Q.h>   // Includes the SFE_MMA8452Q library

char ssid[] = "Lab";
char password[] = "thisisatest";

ShipIoT shipiot("http://yourname.api.shipiot.net/bip/http/sheets","test","test");
MMA8452Q accel(0x1D);      // 0x1d is the I2C addr, if pulled to ground 0x1c

void setup() {
  Serial.begin(115200);
  shipiot.setup(ssid,password);
   accel.init();
}

void loop() {
  while (shipiot.available()) {   // when we have data from the CC3100
    char c = shipiot.read();      // read it
    Serial.write(c);              // and write it to the Serial port for logging
  }
  if (accel.available()) {        // when we have data from the MMA9452Q
    accel.read();                 // read the values from the MMA9452Q
    String json = "{\"x\":" + String(accel.x) + ",\"y\":" + String(accel.y) + ",\"z\":" + String(accel.z) + "}";
    shipiot.send(json);           // send the JSON message to ShipIoT 
  }
 delay(1000);
}




