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

#include "Esp8266BaseAT.h"

Esp8266BaseAT::Esp8266BaseAT() {
	// needed after reboot
	wait_ms(300);
	FactoryReset();
	// needed after restore
	wait_ms(700);
	Echo(false);
	wait_ms(100); // down to 1ms works if more speed is needed
	UARTConfigure();
	/* Tested for 921600 8N01 */
	wait_ms(100); // down to 1ms works if more speed is needed
	SetRFPower(50);
	wait_ms(100); // down to 1ms works if more speed is needed
}

Esp8266BaseAT::~Esp8266BaseAT() {

}

bool
Esp8266BaseAT::Echo(bool enable){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	if (enable){
		ATCommand = "ATE1";
	}
	else{
		ATCommand = "ATE0";
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if (!WaitFor("OK")){
		printf("Error: Missed OK reply - ATE\r\n");
		testprintf("Ended!\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

void
Esp8266BaseAT::FactoryReset(void){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+RESTORE\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if (!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+RESTORE\r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

void
Esp8266BaseAT::RestartModule(void){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+RST\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if (!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+RST\r\n");
		return;
	}
	// needed to set default speed after reboot
	UARTUpdate();
	testprintf("Ended!\r\n");
}

void
Esp8266BaseAT::DeepSleep(uint32_t interval_ms){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+GSLP=";
	// data
	ATCommand += IntegerToString((int)interval_ms);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if (!WaitFor("r")){
		printf("Error: Missed OK reply - AT+GSLP\r\n");
		return;
	}
	testprintf("Ended!\r\n");
}


int8_t
Esp8266BaseAT::GetStationSleepMode(void){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+SLEEP\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());

	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+SLEEP\?\r\n");
		return -1;
	}

	// parsing
	char ch = m_lastATreply.at(m_lastATreply.find(":") + 1);
	dbgprintf("Sleep Mode: %c\r\n", ch);
	testprintf("Ended!\r\n");
	return (int8_t) (ch - '0');
}

void
Esp8266BaseAT::SetStationSleepMode(uint8_t sleepmode){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+SLEEP=";
	// data
	ATCommand += IntegerToString((int) sleepmode);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+SLEEP=\r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

void
Esp8266BaseAT::UARTConfigure(uint32_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity, uint8_t flowctrl, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+UART_";
	if (flashStore){
		ATCommand += "DEF=";
	}
	else {
		ATCommand += "CUR=";
	}

	// parameters
	ATCommand += IntegerToString((int)baudrate) + ",";
	ATCommand += IntegerToString((int)databits) + ",";
	ATCommand += IntegerToString((int)stopbits) + ",";
	ATCommand += IntegerToString((int)parity) + ",";
	ATCommand += IntegerToString((int)flowctrl) + "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if (!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+UART\r\n");
		return;
	}
	// update real UART
	UARTUpdate(baudrate, databits, stopbits, parity, flowctrl);
	testprintf("Ended!\r\n");
}

void
Esp8266BaseAT::SetRFPower(uint32_t txpower){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+RFPOWER=";
	// data
	ATCommand += IntegerToString((int) txpower);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if (!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+RFPOWER\r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

bool
Esp8266BaseAT::WaitFor(string toCompare){
	char ch;
	string receivedS;

	Timer t;
	t.start();
	do{
		// if there's anything in the queue picks it up
		if (m_UART->readable()){
			//store into received string
			receivedS += m_UART->getc();
		}
		//until the string is found or the timeout expires
		if (t.read_ms() > TIMEOUT_MS){
			t.stop();
			printf("Timeout expired!! .. %s \r\n", receivedS.c_str());
			return false;
		}
	} while (receivedS.find(toCompare) == std::string::npos);
	dbgprintf("REPLY:\r\n%s\r\n", receivedS.c_str());
	testprintf("%s\r\n", receivedS.c_str());
	m_lastATreply = receivedS;
	return true;
}

string
Esp8266BaseAT::IntegerToString(int input){
	char sprintBuffer[SPRINTFBUFFERLEN];
	sprintf(sprintBuffer,"%d",input);
	return string(sprintBuffer);
}

int
Esp8266BaseAT::StringToInteger(string input){
	int ret;
	sscanf(input.c_str(), "%d", &ret);
	return ret;
}
