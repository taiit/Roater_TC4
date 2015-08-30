#include <OneWire.h>

OneWire  ds(2);  // on pin 10 (a 4.7K resistor is necessary)
byte data[12];
byte addr[8];

float fTempRead()
{
  byte present = 0;
  float celsius;

  // the first ROM byte indicates which chip
  if(addr[0] != 0x28)
  {
    Serial.println("NOT is DS18B20");
    return 0;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(750);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( int i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time

  celsius = (float)raw / 16.0;
  return celsius;
}

void setup(void) {
  Serial.begin(9600);
  while(!ds.search(addr))
  {
    ds.reset_search();
    delay(250);
  }
}

void loop(void) {

  Serial.print("  Temperature = ");
  Serial.println(fTempRead());
  delay(1000);

}


