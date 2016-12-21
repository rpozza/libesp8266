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

#ifndef ESP8266_DRIVER
#define ESP8266_DRIVER

#include "mbed.h"
#include "MODSERIAL.h"
#include <string>

//#define DEBUG
//#define TEST
#define CIRCULAR_BUFFERSIZE 1024
#define DEBUG_BAUDRATE 115200

#ifdef DEBUG
#define dbgprintf(...) printf (__VA_ARGS__)
#else
#define dbgprintf(...) (void)0
#endif

#ifdef TEST
#define testprintf(...) printf (__VA_ARGS__)
#else
#define testprintf(...) (void)0
#endif



class Esp8266Driver
{
public:
	Esp8266Driver();
	virtual ~Esp8266Driver();

	void UART_TX(string data);
	static MODSERIAL * m_UART;

protected:
	void UARTUpdate(uint32_t baudrate=DEBUG_BAUDRATE, uint8_t databits=8, uint8_t stopbits=1, uint8_t parity=0, uint8_t flowctrl=0);

private:
	void UARTInit(int baudrate);
	void ResetInit(void);
	void ResetL(void);
	void ResetH(void);

	DigitalOut *m_reset;
};

#endif
