// aArtisan.ino
// ------------

// Written to support the Artisan roasting scope //http://code.google.com/p/artisan/

// *** BSD License ***
// ------------------------------------------------------------------------------------------
// Copyright (c) 2011, MLG Properties, LLC
// All rights reserved.
//
// Contributor:  Jim Gallt
//
// Redistribution and use in source and binary forms, with or without modification, are 
// permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice, this list of 
//   conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice, this list 
//   of conditions and the following disclaimer in the documentation and/or other materials 
//   provided with the distribution.
//
//   Neither the name of the copyright holder(s) nor the names of its contributors may be 
//   used to endorse or promote products derived from this software without specific prior 
//   written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS 
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ------------------------------------------------------------------------------------------

#define BANNER_ARTISAN "aARTISAN V2.00"

// Revision history:
// 20110408 Created.
// 20110409 Reversed the BT and ET values in the output stream.
//          Shortened the banner display time to avoid timing issues with Artisan
//          Echo all commands to the LCD
// 20110413 Added support for Artisan 0.4.1
// 20110414 Reduced filtering levels on BT, ET
//          Improved robustness of checkSerial() for stops/starts by Artisan
//          Revised command format to include newline character for Artisan 0.4.x
// 20110528 New command language added (major revision)
//          Use READ command to poll the device for up to 4 temperature channels
//          Change temperature scale using UNITS command
//          Map physical channels on ADC to logical channels using the CHAN command
//          Select SSR output duty cycle with OT1 and OT2 commands
//          Select PWM logic level output on I/O3 using IO3 command
//          Directly control digital pins using DPIN command (WARNING -- this might not be smart)
//          Directly control analog pins using APIN command (WARNING -- this might not be smart)
// 20110601 Major rewrite to use cmndproc.h library
//          RF2000 and RC2000 set channel mapping to 1200
// 20110602 Added ACKS_ON to control verbose output
// ----------- Version 1.10
// 20111011 Added support for type J and type T thermocouples
//          Better error checking on EEPROM reads
// ----------- Version 2.00 (for Arduino 1.0)
// 20120126 Created .ino source file.
//          Serial output now fixed at one decimal place
//          Better echo display of serial commands.
//          Raw ADC conversions are filtered before converting to temperatures.

// this library included with the arduino distribution
#include <Wire.h>

// The user.h file contains user-definable and some other global compiler options
// It must be located in the same folder as aArtisan.pde
#include "user.h"

// command processor declarations -- must be in same folder as aArtisan
#include "cmndreader.h"

#ifdef MEMORY_CHK
// debugging memory problems
#include "MemoryFree.h"
#endif

// these "contributed" libraries must be installed in your sketchbook's arduino/libraries folder
#include <cmndproc.h> // for command interpreter
#include <thermocouple.h> // type K, type J, and type T thermocouple support
#include <cADC.h> // MCP3424
#include <PWM16.h> // for SSR output
#ifdef LCD
#include <cLCD.h> // required only if LCD is used
#endif

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:

// ------------------------ other compile directives
#define MIN_DELAY 300   // ms between ADC samples (tested OK at 270)
#define DP 1  // decimal places for output on serial port
#define D_MULT 0.001 // multiplier to convert temperatures from int to float
#define DELIM "; ," // command line parameter delimiters

#ifdef EEPROM_ARTISAN // optional code if EEPROM flag is active
#include <mcEEPROM.h>
mcEEPROM eeprom;
calBlock caldata;
#endif

float AT; // ambient temp
float TemperatureData[NC];  // final output values referenced to physical channels 0-3
uint8_t actv[NC];  // identifies channel status, 0 = inactive, n = physical channel + 1
#ifdef CELSIUS // only affects startup conditions
boolean Cscale = true;
#else
boolean Cscale = false;
#endif

int levelOT1, levelOT2;  // parameters to control output levels
uint32_t lcd_count; // for echo display of serial commands

// class objects
cADC adc( A_ADC ); // MCP3424
ambSensor amb( A_AMB ); // MCP9800
filterRC fT[NC]; // filter for logged ET, BT
PWM16 ssr;  // object for SSR output on OT1, OT2
CmndInterp ci( DELIM ); // command interpreter object

// ---------------------------------- LCD interface definition
#ifdef LCD
// LCD output strings
char st1[6],st2[6];
#ifdef LCDAPTER
  #define BACKLIGHT lcd.backlight();
  cLCD lcd; // I2C LCD interface
#else // parallel interface, standard LiquidCrystal
  #define BACKLIGHT ;
  #define RS 2
  #define ENABLE 4
  #define D4 7
  #define D5 8
  #define D6 12
  #define D7 13
  LiquidCrystal lcd( RS, ENABLE, D4, D5, D6, D7 ); // standard 4-bit parallel interface
#endif
#endif
// --------------------------------------------- end LCD interface
// [Vo Huu Tai 9/9/2015 ]  ADD NEW
#include <OneWire.h> //Ds18b20
#include <max6675.h> //MAX6675
int thermoSO = 15;	//SO <=> PC1 <=> Pin15 on ardunio uno
int thermoCS = 17;	//CS <=> PC3 <=> Pin17 on ardunio uno
int thermoCLK = 16; //CLK <=> PC2 <=> Pin16 on ardunio uno
byte addr[8];
OneWire  ds(7);  // on pin 7 (a 4.7K resistor is necessary)
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
// [Vo Huu Tai 9/9/2015 ]  END ADD
// ------------- wrapper for the command interpreter's serial line reader
void checkSerial() {
  const char* result = ci.checkSerial();
  if( result != NULL ) { // some things we might want to do after a command is executed
    #ifdef LCD
    lcd.setCursor( 0, 1 ); // echo all commands to the LCD
    lcd.print( "         ");
    lcd.setCursor( 0, 1 ); // echo all commands to the LCD
    lcd.print( result );
    lcd_count = millis();
    #endif
    #ifdef MEMORY_CHK
    Serial.print("# freeMemory()=");
    Serial.println(freeMemory());
    #endif
  }
}

// ----------------------------------
void checkStatus( uint32_t ms ) { // this is an active delay loop
  uint32_t tod = millis();
  while( millis() < tod + ms ) {
    checkSerial();
    // add future code here to detect LCDapter button presses, etc. ?
  }
}

// ----------------------------------------------------
float convertUnits ( float t ) {
  if( Cscale ) return F_TO_C( t );
  else return t;
}

// ------------------------------------------------------------------
void logger()
{
// print ambient
  Serial.print( convertUnits( AT ), DP );
// print active channels
  for( uint8_t jj = 0; jj < NC; ++jj ) {
    uint8_t k = actv[jj];
    if( k > 0 ) {
      --k;
      Serial.print(",");
      Serial.print( convertUnits( TemperatureData[k] ), 2 );
    }
  }
  Serial.println();
}

// --------------------------------------------------------------------------
void get_samples() // this function talks to the amb sensor and ADC via I2C
{
  int32_t v;
  TC_TYPE tc;
  float tempF;
//  int32_t itemp;
  float uiTemp;
  
  uint16_t dly = amb.getConvTime(); // use delay based on slowest conversion
  uint16_t dADC = adc.getConvTime();
  dly = dly > dADC ? dly : dADC;
 
  for( uint8_t jj = 0; jj < NC; jj++ ) { // one-shot conversions on both chips
    uint8_t k = actv[jj]; // map logical channels to physical ADC channels
    if( k > 0 ) {
      --k;
      adc.nextConversion( k ); // start ADC conversion on physical channel k
      amb.nextConversion(); // start ambient sensor conversion
      checkStatus( dly ); // give the chips time to perform the conversions
      amb.readSensor(); // retrieve value from ambient temp register
      v = adc.readuV(); // retrieve microvolt sample from MCP3424
      tempF = tc.Temp_F( 0.001 * v, amb.getAmbF() ); // convert uV to Celsius
      // filter on direct ADC readings, not computed temperatures
      v = fT[k].doFilter( v << 10 );  // multiply by 1024 to create some resolution for filter
      v >>= 10; 
      AT = amb.getAmbF();
      //T[k] = tc.Temp_F( 0.001 * v, AT ); // convert uV to Fahrenheit;
	  // [Vo Huu Tai 27/8/2015 ]  Simulation sensor data
	  //T[k] =  random(300); //(int)fTempRead();
	  // [Vo Huu Tai 1/9/2015 ]  Demo
	  uiTemp = thermocouple.fReadCelsius();//(int)fTempRead(); 
	  if(uiTemp < 70){
		  VAN_GAS_ON;
	  }
	  if(uiTemp > 90){
		  VAN_GAS_OFF;
	  }
	  TemperatureData[k] = uiTemp;
    }
  }
};

#ifdef LCD
// --------------------------------------------
void updateLCD() {
  
 // AT
  int it01 = round( convertUnits( AT ) );
  if( it01 > 999 ) 
    it01 = 999;
  else
    if( it01 < -999 ) it01 = -999;
  sprintf( st1, "%4d", it01 );
  lcd.setCursor( 0, 0 );
  lcd.print("AMB:");
  lcd.print(st1);

  // display the first 2 active channels encountered, normally BT and ET
  uint8_t jj,j;
  uint8_t k;
  for( jj = 0, j = 0; jj < NC && j < 2; ++jj ) {
    k = actv[jj];
    if( k != 0 ) {
      ++j;
      it01 = round( convertUnits( TemperatureData[k-1] ) );
      if( it01 > 999 ) 
        it01 = 999;
      else
        if( it01 < -999 ) it01 = -999;
      sprintf( st1, "%4d", it01 );
      if( j == 1 ) {
        lcd.setCursor( 9, 0 );
        lcd.print("T1:");
      }
      else {
        lcd.setCursor( 9, 1 );
        lcd.print( "T2:" );
      }
      lcd.print(st1);  
    }
  }
  if( millis() - lcd_count >= 100 ) { // display for 100 ms only
    lcd.setCursor( 0, 1 );
    lcd.print( "         " );
    lcd_count = 0;
  }
}
#endif

float fTempRead()
{
	byte data[12];
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
	return (C_TO_F(celsius));
}

boolean bHaveGas(){
	if(analogRead(GAS_PIN) > 800)return true;
	else return false;
}
void vBeep(unsigned int iMiliSeconds){
	BEEP_ON;
	while(iMiliSeconds--){
		delay(1); //<=> _delay_ms
	}
	BEEP_OFF;
}
void vPWM1Control(uint8_t percent)
{
	OCR1A = (100-percent)*20000/100;
}
void vPWM2Control(uint8_t percent)
{
	OCR1B = (100-percent)*20000/100;
}
// ------------------------------------------------------------------------
// MAIN
//
void setup()
{
  delay(100);
  Wire.begin(); 
  Serial.begin(BAUD);
  amb.init( AMB_FILTER );  // initialize ambient temp filtering

#ifdef LCD
  lcd.begin(16, 2);
  BACKLIGHT;
  lcd.setCursor( 0, 0 );
  lcd.print( BANNER_ARTISAN ); // display version banner
#endif // LCD

#ifdef MEMORY_CHK
  Serial.print("# freeMemory()=");
  Serial.println(freeMemory());
#endif

  adc.setCal( CAL_GAIN, UV_OFFSET );
  amb.setOffset( AMB_OFFSET );

#ifdef EEPROM_ARTISAN
  // read calibration and identification data from eeprom
  if( readCalBlock( eeprom, caldata ) ) {
    adc.setCal( caldata.cal_gain, caldata.cal_offset );
    amb.setOffset( caldata.K_offset );
  }
#endif

  // initialize filters on all channels
  fT[0].init( ET_FILTER ); // digital filtering on ET
  fT[1].init( BT_FILTER ); // digital filtering on BT
  fT[2].init( ET_FILTER);
  fT[3].init( ET_FILTER);
  
  // set up output variables
  ssr.Setup( TIME_BASE );
  levelOT1 = levelOT2 = 0;
  
  // initialize the active channels to default values
  actv[0] = 1;  // ET on TC1
  actv[1] = 2;  // BT on TC2
  actv[2] = 0; // default inactive
  actv[3] = 0;

// add active commands to the linked list in the command interpreter object
  ci.addCommand( &dwriter );
  ci.addCommand( &awriter );
  ci.addCommand( &units );
  ci.addCommand( &chan );
  ci.addCommand( &io3 );
  ci.addCommand( &ot2 );
  ci.addCommand( &ot1 );
  ci.addCommand( &rf2000 );
  ci.addCommand( &rc2000 );
  ci.addCommand( &reader );
// [Vo Huu Tai 31/8/2015 ]  Add new command for test relay
  ci.addCommand( &relay1 );
  ci.addCommand( &relay2 );
// [Vo Huu Tai 31/8/2015 ]  End add new command for test relay
#ifdef LCD
  delay( 500 );
  lcd.clear();
#endif

// [Vo Huu Tai 31/8/2015 ]  ADD
 randomSeed(analogRead(0)); //for random temperature 
 MTC_PIN_MODE_INNT;
 MCT_PIN_DEFAULT_SETTING; //turn off all MOSFET
 PWM_TIMER_1_INIT;
  #if 0
  while(!ds.search(addr)) //search ds18b20
  {
	  ds.reset_search();
	  delay(250);
  }
  #endif
// [Vo Huu Tai 31/8/2015 ]  END ADD
}

// -----------------------------------------------------------------
void loop()
{	
	get_samples();
	#ifdef LCD
	updateLCD();
	#endif
	checkSerial();  // Has a command been received?
	// [Vo Huu Tai 1/9/2015 ]  DEMO GAS SENSOR
	 if(bHaveGas()){
		 IGNITION_ON;
		 vBeep(100);
	 }  
	 else{
		 IGNITION_OFF;
	 }
	 
	/*
		The MAX6675 has a maximum conversion time of 0.22 seconds.
		If the device is sampled faster than this it will continue 
		to output the temperature that was first read without any 
		indication that it is being read too fast - 
		I found a delay of at least 250ms between readings the device worked well. 
		Serial.print("Data: ");
		Serial.println( thermocouple.fReadCelsius(), 2 );
		_delay_ms(250);
		
		Test gas sensor
		Serial.println(analogRead(GAS_PIN));
		delay(100); // Print value every 100 msec.
	*/	
	
	//vPWM1Control(100);
	//vPWM2Control(1);
}
