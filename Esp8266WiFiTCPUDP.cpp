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

#include "Esp8266WiFiTCPUDP.h"

Esp8266WiFiTCPUDP::Esp8266WiFiTCPUDP(){
	m_multipleConnMode = false; // boot values, safe to assume not changing?
	m_showremote = false;
}
Esp8266WiFiTCPUDP::~Esp8266WiFiTCPUDP(){
}

void
Esp8266WiFiTCPUDP::UpdateListActiveConnections(uint8_t &clientserver, uint32_t &localPort, uint32_t &remotePort, string &remoteIP, string &conntype, uint8_t &linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTATUS\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPSTATUS\r\n");
		return;
	}
	size_t from = 0;
	while (m_lastATreply.find('+',from) != std::string::npos){
		from = m_lastATreply.find('+', from);
		linkID = (uint8_t) StringToInteger(GetStringBetweenTokens(':',',',m_lastATreply, from));
		conntype = StringUnescape(GetStringBetweenTokens(',',',',m_lastATreply, from));
		remoteIP = StringUnescape(GetStringBetweenTokens(',',',',m_lastATreply, from));
		remotePort = (uint32_t) StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
		localPort = (uint32_t) StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
		clientserver = GetStringBetweenTokens(',','\r',m_lastATreply, from).at(0) - '0';
	}
	testprintf("Ended!\r\n");
}

void
Esp8266WiFiTCPUDP::GetLocalIPAddress(APDevicesList &softAP, APDevicesList &stationAP){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIFSR\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIFSR\r\n");
		return;
	}

	size_t from = 0;
	string APIP = "";
	string APMAC = "";
	string STAIP = "";
	string STAMAC = "";
	string lineToRead;
	while (m_lastATreply.find('+',from) != std::string::npos){
		lineToRead = GetStringBetweenTokens(':',',',m_lastATreply, from);
		if (lineToRead.find("APIP") != std::string::npos){
			APIP = StringUnescape(GetStringBetweenTokens(',','\r',m_lastATreply, from));
		}
		if (lineToRead.find("APMAC") != std::string::npos){
			APMAC = StringUnescape(GetStringBetweenTokens(',','\r',m_lastATreply, from));
		}
		if (lineToRead.find("STAIP") != std::string::npos){
			STAIP = StringUnescape(GetStringBetweenTokens(',','\r',m_lastATreply, from));
		}
		if (lineToRead.find("STAMAC") != std::string::npos){
			STAMAC = StringUnescape(GetStringBetweenTokens(',','\r',m_lastATreply, from));
		}
	}
	softAP.clear();
	softAP.insert(pair<string,string>(APMAC,APIP));
	stationAP.clear();
	stationAP.insert(pair<string,string>(STAMAC,STAIP));
	testprintf("Ended!\r\n");
}

int8_t
Esp8266WiFiTCPUDP::GetMultipleConnectionsStatus(void){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPMUX\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIFSR\r\n");
		return -1;
	}
	// parsing
	char ch = m_lastATreply.at(m_lastATreply.find(":") + 1);
	dbgprintf("Multiple Connections Status: %c\r\n", ch);
	testprintf("Ended!\r\n");
	return (int8_t) (ch - '0');
}

void
Esp8266WiFiTCPUDP::EnableMultipleConnections(bool multipleEnabled){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPMUX=";
	if (multipleEnabled){
		ATCommand += "1";
	}
	else {
		ATCommand += "0";
	}
	// tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIFSR\r\n");
		return;
	}
	m_multipleConnMode = multipleEnabled;
	testprintf("Ended!\r\n");
}

bool
Esp8266WiFiTCPUDP::TCPClientConnectTo(string IPremote, uint32_t portRemote, uint32_t keepaliveSecs, uint8_t linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTART=";
	if (m_multipleConnMode){
		ATCommand += IntegerToString((int) linkID) + ",";
	}
	ATCommand += StringEscape("TCP") + ",";
	ATCommand += StringEscape(IPremote) + ",";
	ATCommand += IntegerToString((int)portRemote) + ",";
	ATCommand += IntegerToString((int)keepaliveSecs) + "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPSTART\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

bool
Esp8266WiFiTCPUDP::TCPServerBind(uint32_t portLocal, bool remove){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	if (!m_multipleConnMode) {
		printf("Error: Multiple Mode needed AT+CIPMUX=1\r\n");
		return false;
	}
	// header
	ATCommand = "AT+CIPSERVER=";
	if (remove){
		ATCommand += "0";
	}
	else {
		ATCommand += "1";
	}
	if (portLocal != 0){
		ATCommand += "," + IntegerToString((int) portLocal);
	}
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPSERVER\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

int
Esp8266WiFiTCPUDP::GetTCPServerTimeout(void){
	string ATCommand;
	int timeout_s;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTO";
	ATCommand += "\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPSTO\r\n");
		return -1;
	}
	size_t from = 0;
	timeout_s = StringToInteger(GetStringBetweenTokens(':','\r',m_lastATreply, from));
	testprintf("Ended!\r\n");
	return timeout_s;
}

bool
Esp8266WiFiTCPUDP::SetTCPServerTimeout(uint32_t timeout_s){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTO";
	ATCommand += "=" + IntegerToString((int) timeout_s);
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPSTO\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

bool
Esp8266WiFiTCPUDP::UDPClientServerBind(string IPremote, uint32_t portRemote, uint32_t portLocal, uint8_t mode, uint8_t linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTART=";
	if (m_multipleConnMode){
		ATCommand += IntegerToString((int) linkID) + ",";
	}
	ATCommand += StringEscape("UDP") + ",";
	ATCommand += StringEscape(IPremote) + ",";
	ATCommand += IntegerToString((int)portRemote) + ",";
	ATCommand += IntegerToString((int)portLocal) + ",";
	ATCommand += IntegerToString((int)mode) + "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPSTART\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

bool
Esp8266WiFiTCPUDP::UDPClientServerConnect(string IPremote, uint32_t portRemote, uint8_t linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTART=";
	if (m_multipleConnMode){
		ATCommand += IntegerToString((int) linkID) + ",";
	}
	ATCommand += StringEscape("UDP") + ",";
	ATCommand += StringEscape(IPremote) + ",";
	ATCommand += IntegerToString((int)portRemote) + "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPSTART\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}


bool
Esp8266WiFiTCPUDP::TCPSend(char * data, int bufflength, uint8_t linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSEND=";
	if (m_multipleConnMode){
		ATCommand += IntegerToString((int) linkID) + ",";
	}
	ATCommand += IntegerToString(bufflength) + "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	WaitFor(">");
	dbgprintf("DATA SENT:\r\n");
	for (int i=0; i < bufflength; i++){
		m_UART->putc(data[i]);
		dbgprintf("%c",data[i]);
	}
	dbgprintf("\r\n");
	if(!DoubleWaitFor("SEND OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPSEND\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

bool
Esp8266WiFiTCPUDP::UDPSendTo(char * data, int bufflength, string IPremote, uint32_t portRemote, uint8_t linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSEND=";
	if (m_multipleConnMode){
		ATCommand += IntegerToString((int) linkID) + ",";
	}
	ATCommand += IntegerToString(bufflength) + ",";
	if (!IPremote.empty()){
		ATCommand += StringEscape(IPremote) + ",";
		ATCommand += IntegerToString((int)portRemote);
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	WaitFor(">");
	dbgprintf("DATA SENT:\r\n");
	for (int i=0; i < bufflength; i++){
		m_UART->putc(data[i]);
		dbgprintf("%c",data[i]);
	}
	dbgprintf("\r\n");
	if(!DoubleWaitFor("SEND OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPSEND\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

bool
Esp8266WiFiTCPUDP::ConnectionClose(uint8_t linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPCLOSE";
	if (m_multipleConnMode){
		ATCommand += "=" + IntegerToString((int) linkID);
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPSEND\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
}

bool
Esp8266WiFiTCPUDP::Ping(string host){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+PING=";
	ATCommand += StringEscape(host);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+PING\r\n");
		return false;
	}
	testprintf("Ended!\r\n");
	return true;
	//optional: return time value i.e.	(reply "+1    OK")
}

bool
Esp8266WiFiTCPUDP::EnableShowRemoteInReceive(bool enable){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPDINFO=";
	if (enable){
		ATCommand += "1";
	}
	else{
		ATCommand += "0";
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPDINFO\r\n");
		return false;
	}
	m_showremote = enable;
	testprintf("Ended!\r\n");
	return true;
}

int
Esp8266WiFiTCPUDP::ReceiveFrom(string &IPRemote, uint32_t &portRemote, char * data, int &bufflength, uint8_t &linkID){
	string ATCommand;
	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	if (!WaitFor(":")){
		// cannot find token
		return -1;
	}
	size_t from = 0;
	size_t reception = m_lastATreply.find("+IPD", from);
	if (reception == std::string::npos){
		// spurious token
		return 0;
	}
	from = reception;

	if (m_multipleConnMode){
		linkID = StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
	}
	if (m_showremote){
		bufflength = StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
		IPRemote = GetStringBetweenTokens(',',',',m_lastATreply, from);
		portRemote = StringToInteger(GetStringBetweenTokens(',',':',m_lastATreply, from));
	}
	else {
		bufflength = StringToInteger(GetStringBetweenTokens(',',':',m_lastATreply, from));
	}
	dbgprintf("DATA RECEIVED:\r\n");
	for (int i=0; i<bufflength; i++){
		data[i] = m_UART->getc();
		dbgprintf("%c",data[i]);
	}
	dbgprintf("\r\n");
	testprintf("Ended!\r\n");
	return bufflength;
}


int
Esp8266WiFiTCPUDP::NonBlockingReceiveFrom(string &IPRemote, uint32_t &portRemote, char * data, int &bufflength, uint8_t &linkID){
	string ATCommand;
	char firstChar;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	if (!m_UART->readable()){
		// cannot find token, exit immediately
		return -3;
	}
	firstChar = m_UART->getc();
	if (firstChar != '+'){
		// spurious token, exit immediately
		return -2;
	}
	// starts to wait for a full message since an header was found!
	if (!WaitFor(":")){
		// spurious message
		return -1;
	}
	size_t from = 0;
	size_t reception = m_lastATreply.find("IPD", from);
	if (reception == std::string::npos){
		// spurious token
		return 0;
	}
	from = reception;

	if (m_multipleConnMode){
		linkID = StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
	}
	if (m_showremote){
		bufflength = StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
		IPRemote = GetStringBetweenTokens(',',',',m_lastATreply, from);
		portRemote = StringToInteger(GetStringBetweenTokens(',',':',m_lastATreply, from));
	}
	else {
		bufflength = StringToInteger(GetStringBetweenTokens(',',':',m_lastATreply, from));
	}
	dbgprintf("DATA RECEIVED:\r\n");
	for (int i=0; i<bufflength; i++){
		data[i] = m_UART->getc();
		dbgprintf("%c",data[i]);
	}
	dbgprintf("\r\n");
	testprintf("Ended!\r\n");
	return bufflength;
}

int
Esp8266WiFiTCPUDP::ReceiveFromLinkID(char * data, int &bufflength, uint8_t &linkID){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	if (!WaitFor(":")){
		// cannot find token, timeout error code -1
		return -1;
	}
	size_t from = 0;
	size_t reception = m_lastATreply.find("+IPD", from);
	if (reception == std::string::npos){
		// spurious token, error code 0
		return 0;
	}
	// parsing number of received bytes
	from = reception;
	if (m_multipleConnMode){
		linkID = StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
	}
	if (m_showremote){
		bufflength = StringToInteger(GetStringBetweenTokens(',',',',m_lastATreply, from));
		GetStringBetweenTokens(',',',',m_lastATreply, from); // just to advance pointer
		GetStringBetweenTokens(',',':',m_lastATreply, from); // just to advance pointer
	}
	else{
		bufflength = StringToInteger(GetStringBetweenTokens(',',':',m_lastATreply, from));
	}
	dbgprintf("DATA RECEIVED:\r\n");
	for (int i=0; i<bufflength; i++){
		data[i] = m_UART->getc();
		dbgprintf("%c",data[i]);
	}
	dbgprintf("\r\n");
	testprintf("Ended!\r\n");
	return bufflength;
}
