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
#include <Wire.h>
#define TC_TYPE typeK  // use one of these 3 options; comment out the other 2
//#define TC_TYPE typeJ
//#define TC_TYPE typeT

#define EEPROM_ARTISAN // comment this line out if no calibration information stored in 64K EEPROM
//#define LCD // if output on an LCD screen is desired
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
#define MOSFET_GAS_PIN			2
#define MOSFET_IGNITION_PIN		3
#define MOSFET_MOTOR_1			4 
#define MOSFET_MOTOR_2			5
#define MOSFET_PWM_2_PIN		9
#define MOSFET_PWM_2_PIN		10
#define GAS_PIN					14
#define BEEP_PIN				6
#define LED_STATUS_PIN			8
#define OC1A_PIN				9
#define OC1B_PIN				10		
//Turn on and off MOSFET macro
#define VAN_GAS_ON			digitalWrite(MOSFET_GAS_PIN, LOW)
#define VAN_GAS_OFF			digitalWrite(MOSFET_GAS_PIN, HIGH)
//
#define IGNITION_ON			digitalWrite(MOSFET_IGNITION_PIN, LOW)
#define IGNITION_OFF			digitalWrite(MOSFET_IGNITION_PIN, HIGH)
//
#define MOTOR_1_ON			digitalWrite(MOSFET_MOTOR_1, LOW)
#define MOTOR_1_OFF			digitalWrite(MOSFET_MOTOR_1, HIGH)
//
#define MOTOR_2_ON			digitalWrite(MOSFET_MOTOR_2, LOW)
#define MOTOR_2_OFF			digitalWrite(MOSFET_MOTOR_2, HIGH)
//
#define BEEP_ON				digitalWrite(BEEP_PIN, HIGH)
#define BEEP_OFF			digitalWrite(BEEP_PIN, LOW)

#define MTC_PIN_MODE_INNT			pinMode(MOSFET_GAS_PIN, OUTPUT);\
									pinMode(MOSFET_IGNITION_PIN, OUTPUT);\
									pinMode(MOSFET_MOTOR_1, OUTPUT);\
									pinMode(MOSFET_MOTOR_2, OUTPUT);\
									pinMode(BEEP_PIN,OUTPUT)

#define PWM_TIMER_1_INIT			pinMode(OC1A_PIN,OUTPUT);\
									pinMode(OC1B_PIN,OUTPUT);\
									TCCR1A = _BV(COM1A1) | _BV(COM1B1)	| _BV(WGM11);\
									TCCR1B = _BV(WGM12)  | _BV(WGM13)	| _BV(CS12);\
									ICR1 = 30000;\
									OCR1A = 15000;\
									OCR1B = 15000

#define MCT_PIN_DEFAULT_SETTING		VAN_GAS_OFF;\
									IGNITION_OFF;\
									MOTOR_1_OFF;\
									MOTOR_2_OFF;\
									BEEP_OFF
														
// [Vo Huu Tai 31/8/2015 ]  END ADD

// *************************************************************************************

#endif
