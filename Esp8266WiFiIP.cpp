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

#include "Esp8266WiFiIP.h"

StaticIP::StaticIP(){
	m_IPAddress = "";
	m_IPGateway = "";
	m_IPNetmask = "";
}
StaticIP::StaticIP(string address, string gway, string nmask){
	m_IPAddress = address;
	m_IPGateway = gway;
	m_IPNetmask = nmask;
}
StaticIP::~StaticIP(){
}


DHCPServer::DHCPServer(){
	m_lease_minutes = 2880;
	m_IPStart = "";
	m_IPEnd = "";
}
DHCPServer::DHCPServer(uint32_t lease, string startIP, string endIP){
	if (lease > 2880) lease = 2880;
	if (lease < 1) lease = 1;
	m_lease_minutes = lease;
	m_IPStart = startIP;
	m_IPEnd = endIP;
}
DHCPServer::~DHCPServer(){

}

/*
 * WIFIIP CLASS
 */

Esp8266WiFiIP::Esp8266WiFiIP() {

}

Esp8266WiFiIP::~Esp8266WiFiIP() {

}

int8_t
Esp8266WiFiIP::GetDHCPStatus(bool flashStored){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWDHCP_";
	// store
	if (flashStored){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand +="\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CWDHCP\?\r\n");
		return -1;
	}
	// parsing
	char ch = m_lastATreply.at(m_lastATreply.find(":") + 1);
	dbgprintf("DHCP Status : %c\r\n", ch);
	testprintf("Ended!\r\n");
	return (int8_t) (ch - '0');
}

void
Esp8266WiFiIP::DHCPEnable(uint8_t mode, uint8_t enable, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWDHCP_";
	// store
	if (flashStore){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand += "=" + IntegerToString((int) mode);
	ATCommand += "," + IntegerToString((int) enable);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("\r\nError: Missed OK reply - AT+CWDHCP_xxx\r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

// ---- STATION AP ----

StaticIP
Esp8266WiFiIP::GetStaticIPStationAP(bool flashStored){
	string ATCommand;
	string ipAddr, gWay, nMask;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTA_";
	// store
	if (flashStored){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand +="\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPSTA\?\r\n");
		return StaticIP();
	}

	size_t from = 0;
	if (GetStringBetweenTokens(':',':',m_lastATreply, from).find("ip") != std::string::npos){
		ipAddr = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));
	}
	if (GetStringBetweenTokens(':',':',m_lastATreply, from).find("gateway") != std::string::npos){
		gWay = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));
	}
	if (GetStringBetweenTokens(':',':',m_lastATreply, from).find("netmask") != std::string::npos){
		nMask = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));
	}
	dbgprintf("Station's IP: <%s>,<%s>,<%s>\r\n", ipAddr.c_str(), gWay.c_str(), nMask.c_str());
	testprintf("Ended!\r\n");
	return StaticIP(ipAddr,gWay,nMask);
}

void
Esp8266WiFiIP::SetStaticIPStationAP(StaticIP statAP, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTA_";
	// store
	if (flashStore){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand += "=" + StringEscape(statAP.m_IPAddress);
	if (!(statAP.m_IPGateway.empty())) {
		ATCommand += "," + StringEscape(statAP.m_IPGateway);
		if (!(statAP.m_IPNetmask.empty())) {
			ATCommand += "," + StringEscape(statAP.m_IPNetmask);
		}
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPSTA_xxx\r\n");
		return;
	}
	m_myStationAPStaticIP = statAP;
	testprintf("Ended!\r\n");
}

// ---- SOFT AP ----

StaticIP
Esp8266WiFiIP::GetStaticIPSoftAP(bool flashStored){
	string ATCommand;
	string ipAddr, gWay, nMask;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPAP_";
	// store
	if (flashStored){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand +="\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPAP\?\r\n");
		return StaticIP();
	}

	size_t from = 0;
	if (GetStringBetweenTokens(':',':',m_lastATreply, from).find("ip") != std::string::npos){
		ipAddr = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));
	}
	if (GetStringBetweenTokens(':',':',m_lastATreply, from).find("gateway") != std::string::npos){
		gWay = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));
	}
	if (GetStringBetweenTokens(':',':',m_lastATreply, from).find("netmask") != std::string::npos){
		nMask = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));
	}
	dbgprintf("SoftAP's IP: <%s>,<%s>,<%s>\r\n", ipAddr.c_str(), gWay.c_str(), nMask.c_str());
	testprintf("Ended!\r\n");
	return StaticIP(ipAddr,gWay,nMask);
}

void
Esp8266WiFiIP::SetStaticIPSoftAP(StaticIP softAP, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPAP_";
	// store
	if (flashStore){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand += "=" + StringEscape(softAP.m_IPAddress);
	if (!(softAP.m_IPGateway.empty())) {
		ATCommand += "," + StringEscape(softAP.m_IPGateway);
		if (!(softAP.m_IPNetmask.empty())) {
			ATCommand += "," + StringEscape(softAP.m_IPNetmask);
		}
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CIPAP_xxx\r\n");
		return;
	}
	m_mySoftAPStaticIP = softAP;
	testprintf("Ended!\r\n");
}

DHCPServer
Esp8266WiFiIP::GetDHCPRange(bool flashStored){
	string ATCommand;
	uint32_t leasemins;
	string IPstart, IPend;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWDHCPS_";
	// store
	if (flashStored){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand +="\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CWDHCP\?\r\n");
		return DHCPServer();
	}

	size_t from = 0;
	leasemins = StringToInteger(GetStringBetweenTokens(':',',',m_lastATreply, from));
	IPstart = GetStringBetweenTokens(',',',',m_lastATreply, from); // NB: no unescaping necessary?
	IPend = GetStringBetweenTokens(',','\r',m_lastATreply, from); // NB: no unescaping necessary?

	dbgprintf("SoftAP's DHCP Range and Lease Minutes: <%s>,<%s>,<%d>\r\n", IPstart.c_str(), IPend.c_str(), (int) leasemins);
	testprintf("Ended!\r\n");
	return DHCPServer(leasemins,IPstart,IPend);
}

void
Esp8266WiFiIP::SetDHCPRange(DHCPServer servConf, bool def_range, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWDHCPS_";
	// store
	if (flashStore){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	if (def_range){
		ATCommand += "=0";
	}
	else {
		ATCommand += "=1," + IntegerToString((int) servConf.m_lease_minutes);
		ATCommand += "," + StringEscape(servConf.m_IPStart);
		ATCommand += "," + StringEscape(servConf.m_IPEnd);
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK", "ERROR")){
		printf("Error: Missed OK reply - AT+CWDHCP_xxx (check IP range is on the same subnet of the AP's allocated IP)\r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

APDevicesList
Esp8266WiFiIP::GetSoftAPConnectedDevices(void){
	string ATCommand;
	string ipadd, macadd;
	APDevicesList retval;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWLIF\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: AT+CWLIF! \r\n");
		retval.insert(pair<string,string>("",""));
		return retval;
	}

	size_t from = 0;
	size_t separator = 0;
	while (m_lastATreply.find(',',from) != std::string::npos){
		separator = m_lastATreply.find(',',from);
		ipadd = m_lastATreply.substr(from, (separator - from));
		from = separator + 1;
		separator = m_lastATreply.find('\r',from);
		macadd = m_lastATreply.substr(from, (separator - from));
		from = separator + 2;
		retval.insert(pair<string,string>(macadd,ipadd));
	}
	testprintf("Ended!\r\n");
	return retval;
}
