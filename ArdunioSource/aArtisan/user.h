// user.h
//---------
// This file contains user definable compiler directives for aArtisan

// *************************************************************************************
// NOTE TO USERS: the following parameters should be
// be reviewed to suit your preferences and hardware setup.  
// First, load and edit this sketch in the Arduino IDE.
// Next compile the sketch and upload it to the Arduino.

// Version 1.10

#ifndef USER_H
#define USER_H

#define TC_TYPE typeK  // use one of these 3 options; comment out the other 2
//#define TC_TYPE typeJ
//#define TC_TYPE typeT

#define EEPROM_ARTISAN // comment this line out if no calibration information stored in 64K EEPROM
#define LCD // if output on an LCD screen is desired
#define LCDAPTER // if the I2C LCDapter board is to be used
//#define CELSIUS // controls only the initial conditions

#define BAUD 19200  // serial baud rate
#define BT_FILTER 10 // filtering level (percent) for BT
#define ET_FILTER 10 // filtering level (percent) for ET
#define AMB_FILTER 70 // 70% filtering on ambient sensor readings

// default values for systems without calibration values stored in EEPROM
#define CAL_GAIN 1.00 // you may substitute a known gain adjustment from calibration
#define UV_OFFSET 0 // you may substitute a known value for uV offset in ADC
#define AMB_OFFSET 0.0 // you may substitute a known value for amb temp offset (Celsius)

#define TIME_BASE pwmN1Hz // cycle time for PWM output to SSR's on OT1, OT2
#define NC 4 // maximum number of physical channels on the TC4

// Useful for debugging only -- leave inactive otherwise
//#define MEMORY_CHK

// This turns on the "# xxxxxxx\n" acknowledgments after commands
//#define ACKS_ON

// [Vo Huu Tai 31/8/2015 ]  ADD
#define RELAY_1_PIN				13 //Led, and relay 1
#define RELAY_2_PIN				12 //relay 2
#define RELAY_TEMPERATURE_PIN	11
#define RELAY_GAS_PIN			10
#define RELAY_TEMPERATURE_ON	digitalWrite(RELAY_TEMPERATURE_PIN, LOW)
#define RELAY_TEMPERATURE_OFF	digitalWrite(RELAY_TEMPERATURE_PIN, HIGH)
#define RELAY_GAS_ON			digitalWrite(RELAY_GAS_PIN, LOW)
#define RELAY_GAS_OFF			digitalWrite(RELAY_GAS_PIN, HIGH)
// [Vo Huu Tai 31/8/2015 ]  END ADD

// *************************************************************************************

#endif
