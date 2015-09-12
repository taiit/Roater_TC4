/*
 * max6675.h
 *
 * Created: 9/9/2015 5:08:18 PM
 *  Author: Vo Huu Tai
 */ 
#ifndef MAX6675_H_
#define MAX6675_H_
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MAX6675 {
	public:
	MAX6675(int8_t SCLK, int8_t CS, int8_t MISO);

	double fReadCelsius(void);
	double fReadFahrenheit(void);
	private:
	int8_t cSCK, cSO, cCS;
	uint8_t ucSpiRead(void);
};
#endif /* MAX6675_H_ */

