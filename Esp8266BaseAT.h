/*
 * Copyright [2016] [Riccardo Pozza]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author:
 * Riccardo Pozza <r.pozza@surrey.ac.uk>
 */

#ifndef ESP8266_BASEAT
#define ESP8266_BASEAT

#include "Esp8266Driver.h"

#define TIMEOUT_MS 					20000 // timeout in milliseconds
#define SPRINTFBUFFERLEN 			20

//Station Mode Sleep parameters
#define DISABLE_SLEEP  				0
#define LIGHT_SLEEP  				1
#define MODEM_SLEEP  				2

#define DEFAULT_TX_POWER			50 // range (0~82 * 0.25)dBm

class Esp8266BaseAT : public Esp8266Driver {
public:
	Esp8266BaseAT();
	virtual ~Esp8266BaseAT();

	/*
	 * Simple Casting Routines
	 */
	string IntegerToString(int input);
	int StringToInteger(string input);

protected:
	void Echo(bool enable);
	void FactoryReset(void);
	void RestartModule(void);
	void DeepSleep(uint32_t interval_ms);
	int8_t GetStationSleepMode(void);
	void SetStationSleepMode(uint8_t sleepmode=MODEM_SLEEP);
	void SetRFPower(uint32_t txpower=DEFAULT_TX_POWER);

	/* Input Parameters
	 * Baudrate   : from 110 to 115200*40
	 * Databits   : 5, 6, 7, 8 bits
	 * Stopbits   : 1<=>1, 2<=>1.5, 3<=>2 bits
	 * Parity     : 0<=>none, 1<=>odd, 2<=>even
	 * FlowCtrl   : 0<=>disable, 1<=>RTS, 2<=>CTS, 3<=>RTS+CTS
	 */
	void UARTConfigure(uint32_t baudrate=921600, uint8_t databits=8, uint8_t stopbits=1, uint8_t parity=0, uint8_t flowctrl=0, bool flashStore=false);

	/*
	 * I/O parsing handling routines and variables
	 */
	bool WaitFor(string toCompare);
	string m_lastATreply;
};

#endif
