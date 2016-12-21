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

#include "Esp8266Driver.h"

Esp8266Driver::Esp8266Driver() {
	// Initialize Reset Pin
	ResetInit();
	// Perform reset: L->H transition
	ResetL();
	wait_ms(100);
	ResetH();
	wait_ms(100);
	// setup UART callback and baudrate
	UARTInit(DEBUG_BAUDRATE);
}

Esp8266Driver::~Esp8266Driver() {

}

void
Esp8266Driver::UART_TX(string data) {
	m_UART->puts(data.c_str());
}

void
Esp8266Driver::UARTUpdate(uint32_t baudr, uint8_t datab, uint8_t stopb, uint8_t par, uint8_t flctrl) {
	dbgprintf("Updating UART parameters: %d,%d,%d,%d,%d\r\n", (int)baudr,(int)datab,(int)stopb,(int)par,(int)flctrl);
	m_UART->baud(baudr);
	m_UART->format(datab, static_cast<mbed::SerialBase::Parity> (par),stopb);
#if defined(TARGET_ARCH_PRO)
	//NB: these are actually N.C.
	m_UART->set_flow_control(static_cast<mbed::SerialBase::Flow> (flctrl), P0_10, P0_11);
#endif
}

void
Esp8266Driver::UARTInit(int baudrate) {
	dbgprintf("Initializing UART P4_28,P4_29 in IRQ Mode: %d baud\r\n", baudrate);
	m_UART->baud(baudrate);
}

void
Esp8266Driver::ResetInit(void) {
	#if defined(TARGET_ARCH_PRO)
		dbgprintf("Initializing Reset GPIO: P0_4\r\n");
		m_reset = new DigitalOut (P0_4);
	#endif
}

void
Esp8266Driver::ResetL(void) {
	dbgprintf("Reset GND\r\n");
	m_reset->write(0);
}

void
Esp8266Driver::ResetH(void) {
	dbgprintf("Reset VDD\r\n");
	m_reset->write(1);
}

#if defined(TARGET_ARCH_PRO)
MODSERIAL * Esp8266Driver::m_UART = new MODSERIAL(P4_28, P4_29,CIRCULAR_BUFFERSIZE);
#endif
