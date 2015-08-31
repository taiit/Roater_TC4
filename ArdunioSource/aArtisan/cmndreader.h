// cmndreader.h
//----------------

// code that defines specific commands for aArtisan

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

// Version 1.10

#ifndef CMNDREADER_H
#define CMNDREADER_H

#include <cmndproc.h>
#include <PWM16.h>

#include "user.h"

// ----------------------- commands
#define READ_CMD "READ" // triggers the TC4 to output current temps on serial line
#define RF2000_CMD "RF2000" // legacy code
#define RC2000_CMD "RC2000" // legacy code
#define UNITS_CMD "UNITS" // changes units, F or C
#define CHAN_CMD "CHAN" // maps logical channels to physical channels
#define OT1_CMD "OT1" // 0 to 100 percent output on SSR drive OT1
#define OT2_CMD "OT2" // 0 to 100 percent output on SSR drive OT2
#define IO3_CMD "IO3" // 0 to 100 percent PWM 5V output on IO3
#define DIGITAL_WRITE_CMD "DWRITE" // turn digital pin LOW or HIGH
#define ANALOG_WRITE_CMD "AWRITE" // write a value 0 to 255 to PWM pin
#define IO3 3 // use DIO3 for PWM output
// [Vo Huu Tai 31/8/2015 ]  ADD
#define RELAY1_CMD "RL1"
#define RELAY2_CMD "RL2"
// [Vo Huu Tai 31/8/2015 ]  END ADD

// forward declarations
class dwriteCmnd;
class awriteCmnd;
class readCmnd;
class chanCmnd;
class ot1Cmnd;
class ot2Cmnd;
class io3Cmnd;
class unitsCmnd;
class rf2000Cmnd;
class rc2000Cmnd;
// [Vo Huu Tai 31/8/2015 ]  ADD
class relay1Cmd;
class relay2Cmd;
// [Vo Huu Tai 31/8/2015 ]  END ADD

// external declarations of class objects
extern readCmnd reader;
extern awriteCmnd awriter;
extern dwriteCmnd dwriter;
extern chanCmnd chan;
extern ot1Cmnd ot1;
extern ot2Cmnd ot2;
extern io3Cmnd io3;
extern rf2000Cmnd rf2000;
extern rc2000Cmnd rc2000;
extern unitsCmnd units;
// [Vo Huu Tai 31/8/2015 ]  ADD
extern relay1Cmd relay1;
extern relay2Cmd relay2;
// [Vo Huu Tai 31/8/2015 ]  END ADD
// extern declarations for functions, variables in the main program
extern PWM16 ssr;
extern int levelOT1;
extern int levelOT2;
extern void logger();
extern boolean Cscale;
extern uint8_t actv[NC];

// class declarations for commands

class dwriteCmnd : public CmndBase {
  public:
    dwriteCmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class awriteCmnd : public CmndBase {
  public:
    awriteCmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class readCmnd : public CmndBase {
    public:
    readCmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class chanCmnd : public CmndBase {
  public:
    chanCmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class ot1Cmnd : public CmndBase {
  public:
    ot1Cmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class ot2Cmnd : public CmndBase {
  public:
    ot2Cmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class io3Cmnd : public CmndBase {
  public:
    io3Cmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class unitsCmnd : public CmndBase {
  public:
    unitsCmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class rf2000Cmnd : public CmndBase {
  public:
    rf2000Cmnd();
    virtual boolean doCommand( CmndParser* pars );
};

class rc2000Cmnd : public CmndBase {
  public:
    rc2000Cmnd();
    virtual boolean doCommand( CmndParser* pars );
};
// [Vo Huu Tai 31/8/2015 ]  Add
class relay1Cmd : public CmndBase {
	private:
		bool bIsFirst = false;
	public:
		relay1Cmd();
		virtual boolean doCommand( CmndParser* pars );
};
class relay2Cmd : public CmndBase {
	private:
		bool bIsFirst = false;
	public:
		relay2Cmd();
		virtual boolean doCommand( CmndParser* pars );
};
#endif

