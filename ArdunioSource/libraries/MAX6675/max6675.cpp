/*
 * max6675.cpp
 *
 * Created: 9/9/2015 5:10:40 PM
 *  Author: Vo Huu Tai
 */ 

#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif
#include <util/delay.h>
#include <stdlib.h>
#include "max6675.h"

MAX6675::MAX6675(int8_t sck, int8_t cs, int8_t so) {
  cSCK = sck;
  cCS = cs;
  cSO = so;

  //define pin modes
  pinMode(cCS, OUTPUT);
  pinMode(cSCK, OUTPUT); 
  pinMode(cSO, INPUT);

  digitalWrite(cCS, HIGH);
}
double MAX6675::fReadCelsius(void) {

  uint16_t uiRetValue;

  digitalWrite(cCS, LOW);
  _delay_ms(1);

  uiRetValue = ucSpiRead();
  uiRetValue <<= 8;
  uiRetValue |= ucSpiRead();

  digitalWrite(cCS, HIGH);

  if (uiRetValue & 0x4) {
    // uh oh, no thermocouple attached!
    return NAN; 
    //return -100;
  }
   /*
	Bit[15]:	Dummy sign (always equals 0)
	Bit[14:3]:	Temperature data: bit 14 is MSB
	Bit[2]:		default is low, goes high if the thermocouple input is open
	Bit[1]:		Device ID, is 0
	Bit[0]:		is three-state
	 
  */
  uiRetValue >>= 3;
  
  return uiRetValue*0.25;
}

double MAX6675::fReadFahrenheit(void) {
  return fReadCelsius() * 9.0/5.0 + 32;
}

uint8_t MAX6675::ucSpiRead(void) { 
  int8_t index;
  uint8_t ucRetData = 0;

  for (index = 7; index >= 0; index--)
  {
    digitalWrite(cSCK, LOW);
    _delay_ms(1);
    if (digitalRead(cSO)) {
      //set the bit to 0 no matter what
      ucRetData |= (1 << index);
    }

    digitalWrite(cSCK, HIGH);
    _delay_ms(1);
  }

  return ucRetData;
}
