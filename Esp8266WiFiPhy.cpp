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

#include "Esp8266WiFiPhy.h"

/*
 * STATION ACCESS POINT DATA STRUCTURE AND INITIALIZATION
 */

StationAccessPoint::StationAccessPoint() {
	m_ssid = "e/d\\u,ro\"am";
	m_password = "pass1234";
	m_bssidmac = ""; //"ca:7d:19:d8:a6:44";
	m_channel = -1;
	m_rssi = 0;
}
StationAccessPoint::StationAccessPoint(string ssid, string password, string bssidmac) {
	m_ssid = ssid;
	m_password = password;
	m_bssidmac = bssidmac;
	m_channel = -1;
	m_rssi = 0;
}
StationAccessPoint::StationAccessPoint(string ssid, string bssidmac, int channel, int rssi) {
	m_ssid = ssid;
	m_bssidmac = bssidmac;
	m_channel = channel;
	m_rssi = rssi;
}
StationAccessPoint::~StationAccessPoint() {
}

/*
 * SOFT ACCESS POINT DATA STRUCTURE AND INITIALIZATION
 */
SoftAccessPoint::SoftAccessPoint() {
	m_ssid = "DeskEgg02";
	m_password = "password";
	m_channel = 11;
	m_encryption = WIFI_ENC_WPA2_PSK;
	m_maxconnections = WIFI_MAX_CONNECTIONS;
	m_ssidbroadcast = WIFI_BROADCAST_SSID;
}
SoftAccessPoint::SoftAccessPoint(string ssid, string password, int channel, int encryption, int max_conn, int ssid_bcast) {
	m_ssid = ssid;
	m_password = password;
	m_channel = channel;
	m_encryption = encryption;
	m_maxconnections = max_conn;
	m_ssidbroadcast = ssid_bcast;
}
SoftAccessPoint::~SoftAccessPoint() {
}

/*
 * WIFIPHY CLASS
 */

Esp8266WiFiPhy::Esp8266WiFiPhy() : m_stationAP(), m_softAP() {

}

Esp8266WiFiPhy::~Esp8266WiFiPhy() {

}

int8_t
Esp8266WiFiPhy::GetWiFiMode(bool flashStored){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWMODE_";
	// stored?
	if (flashStored){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	//tail
	ATCommand += "\?\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());

	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CWMODE\?\r\n");
		return -1;
	}

	// parsing
	char ch = m_lastATreply.at(m_lastATreply.find(":") + 1);
	dbgprintf("WiFi Mode: %c\r\n", ch);
	testprintf("Ended!\r\n");
	return (int8_t) (ch - '0');
}

void
Esp8266WiFiPhy::SetWiFiMode(uint8_t mode, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWMODE_";
	// store
	if (flashStore){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand += "=" + IntegerToString((int)mode);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: Missed OK reply - AT+CWMODE_xxx=\r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

// ---- STATION MODE ----

StationAccessPoint
Esp8266WiFiPhy::GetConnectedStationAP(bool flashStored){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWJAP_";
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
		printf("Error: Missed OK reply - AT+CWJAP\?\r\n");
		return StationAccessPoint("","","");
	}

	size_t from = 0;
	string ssid = StringUnescape(GetStringBetweenTokens(':',',',m_lastATreply, from));
	string bssid = StringUnescape(GetStringBetweenTokens(',',',',m_lastATreply, from));
	string channel = GetStringBetweenTokens(',',',',m_lastATreply, from);
	string rssi = GetStringBetweenTokens(',','\r',m_lastATreply, from);

	dbgprintf("AccessPoint: <%s>,<%s>,<%s>,<%s>\r\n", ssid.c_str(), bssid.c_str(), channel.c_str(), rssi.c_str());
	testprintf("Ended!\r\n");
	return StationAccessPoint(ssid,bssid,StringToInteger(channel),StringToInteger(rssi));
}

bool
Esp8266WiFiPhy::ConnectToStationAP (StationAccessPoint sap, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWJAP_";
	if (flashStore){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}

	ATCommand += "=" + StringEscape(sap.m_ssid) + ",";
	ATCommand += StringEscape(sap.m_password);
	if (!(sap.m_bssidmac.empty())) {
		ATCommand += "," + StringEscape(sap.m_bssidmac);
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());

	if(!DoubleWaitFor("OK","FAIL")){
		printf("Error: +CWJAP: Error Code (1<=>4)==(TIMEOUT,WRONG PASSWORD,AP NOT FOUND,FAIL)\r\n");
		return false;
	}
	// assign to internal variable
	m_stationAP = sap;
	testprintf("Ended!\r\n");
	return true;
}

void
Esp8266WiFiPhy::ListStationAP(StationAccessPoint sap){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWLAP";
	if (!sap.m_ssid.empty()){
		ATCommand += "=" + StringEscape(sap.m_ssid);
		if (!sap.m_bssidmac.empty()){
			ATCommand += "," + StringEscape(sap.m_bssidmac);
			if (sap.m_channel != -1){
				ATCommand += "," + IntegerToString(sap.m_channel); // not to be escaped, since number!
			}
		}
	}
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CWLAP\r\n");
		return;
	}
	dbgprintf("+CWLAP:(<ECN>,<SSID>,<RSSI>,<MAC>,<CH>,<FREQ OFFSET>,<FREQ CALIB>)\r\n");
	testprintf("Ended!\r\n");
}

void
Esp8266WiFiPhy::QuitStationAP(void){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWQAP\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!WaitFor("OK")){
		printf("Error: AT+CWQAP! \r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

void
Esp8266WiFiPhy::AutoConnectToStationAtBoot(bool enable){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWAUTOCONN=";
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
	if(!WaitFor("OK")){
		printf("Error: AT+CWAUTOCONN! \r\n");
		return;
	}
	testprintf("Ended!\r\n");
}

string
Esp8266WiFiPhy::GetStationAPMac(bool flashStored){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTAMAC_";
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
		printf("Error: Missed OK reply - AT+CIPSTAMAC\?\r\n");
		return "";
	}
	size_t from = 0;
	string stamac = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));

	dbgprintf("StationAP MAC ADDRESS: <%s>\r\n", stamac.c_str());
	testprintf("Ended!\r\n");
	return stamac;
}

void
Esp8266WiFiPhy::SetStationAPMac(string mac_addr, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPSTAMAC_";
	// store
	if (flashStore){
		// NB: DISABLED: WARNING!!! THIS CHANGES THE DEVICE PHYSICAL MAC ADDRESS!!
		// ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand += "=" + StringEscape(mac_addr);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPSTAMAC_xxx\r\n");
		return;
	}
	m_stationAPInterfaceMac = mac_addr;
	testprintf("Ended!\r\n");
}

// ---- SOFT AP ----

SoftAccessPoint
Esp8266WiFiPhy::GetConfigurationSoftAP(bool flashStored){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWSAP_";
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
		printf("Error: Missed OK reply - AT+CWSAP\?\r\n");
		return SoftAccessPoint("","",-1,-1,-1,-1);
	}

	size_t from = 0;
	string ssid = StringUnescape(GetStringBetweenTokens(':',',',m_lastATreply, from));
	string password = StringUnescape(GetStringBetweenTokens(',',',',m_lastATreply, from));
	string channel = GetStringBetweenTokens(',',',',m_lastATreply, from);
	string encryption = GetStringBetweenTokens(',',',',m_lastATreply, from);
	string max_conn = GetStringBetweenTokens(',',',',m_lastATreply, from);
	string ssid_bcast = GetStringBetweenTokens(',','\r',m_lastATreply, from);

	dbgprintf("AccessPoint: <%s>,<%s>,<%s>,<%s>,<%s>,<%s>\r\n", ssid.c_str(), password.c_str(), channel.c_str(), encryption.c_str(), max_conn.c_str(), ssid_bcast.c_str());
	testprintf("Ended!\r\n");
	return SoftAccessPoint(ssid,password,StringToInteger(channel),StringToInteger(encryption),StringToInteger(max_conn),StringToInteger(ssid_bcast));

}

void
Esp8266WiFiPhy::ConfigureSoftAP(SoftAccessPoint sap, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CWSAP_";
	// store
	if (flashStore){
		ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand += "=" + StringEscape(sap.m_ssid);
	ATCommand += "," + StringEscape(sap.m_password);
	ATCommand += "," + IntegerToString(sap.m_channel);
	ATCommand += "," + IntegerToString(sap.m_encryption);
	ATCommand += "," + IntegerToString(sap.m_maxconnections);
	ATCommand += "," + IntegerToString(sap.m_ssidbroadcast);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CWSAP_xxx\r\n");
		return;
	}
	// assign to internal variable
	m_softAP = sap;
	testprintf("Ended!\r\n");
}

string
Esp8266WiFiPhy::GetSoftAPMac(bool flashStored){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPAPMAC_";
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
		printf("Error: Missed OK reply - AT+CIPAPMAC\?\r\n");
		return "";
	}
	size_t from = 0;
	string stamac = StringUnescape(GetStringBetweenTokens(':','\r',m_lastATreply, from));

	dbgprintf("SoftAP MAC ADDRESS: <%s>\r\n", stamac.c_str());
	testprintf("Ended!\r\n");
	return stamac;
}

void
Esp8266WiFiPhy::SetSoftAPMac(string mac_addr, bool flashStore){
	string ATCommand;

	testprintf("\r\nEntering %s ...", __PRETTY_FUNCTION__);
	// header
	ATCommand = "AT+CIPAPMAC_";
	// store
	if (flashStore){
		// NB: DISABLED: WARNING!!! THIS CHANGES THE DEVICE PHYSICAL MAC ADDRESS!!
		// ATCommand += "DEF";
	}
	else {
		ATCommand += "CUR";
	}
	ATCommand += "=" + StringEscape(mac_addr);
	//tail
	ATCommand += "\r\n";

	// resets the buffer from any spurious previous output and send
	m_UART->rxBufferFlush();
	UART_TX(ATCommand);
	dbgprintf("SEND:\r\n%s",ATCommand.c_str());
	if(!DoubleWaitFor("OK","ERROR")){
		printf("Error: Missed OK reply - AT+CIPAPMAC_xxx\r\n");
		return;
	}
	m_softAPInterfaceMac = mac_addr;
	testprintf("Ended!\r\n");
}

string
Esp8266WiFiPhy::StringEscape (string input)
{
	string ret = "\"";
	for (std::string::iterator it=input.begin(); it!=input.end(); ++it){
		if (*it == '\\' || *it == ',' || *it == '"'){
			ret += '\\'; // escaped character
		}
		ret += (*it);
	}
	ret += "\"";
	return ret;
}

string
Esp8266WiFiPhy::StringUnescape (string input)
{
	string ret = "";
	std::string::iterator it2;
	for (std::string::iterator it=input.begin(); it!=input.end(); ++it){
		// second pointer
		it2 = it;
		advance(it2,1);
		if (*it == '\\'){ // escaped character
			if (*it2 == '\\' || *it2 == ',' || *it2 == '"'){
				it = it2; // skip it
			}
		}
		ret += (*it);
	}
	return ret.substr(1,ret.size()-2);
}

string
Esp8266WiFiPhy::GetStringBetweenTokens (char starttoken, char endtoken, string input, size_t &from)
{
	string ret = "";

	size_t first = input.find(starttoken, from) + 1;
	size_t displacement = input.find(endtoken,first) - first;
	ret = input.substr(first,displacement);

	// make sure it advances
	from = first;

	return ret;
}

bool
Esp8266WiFiPhy::DoubleWaitFor(string toCompareOK, string toCompareERR){
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

		if (receivedS.find(toCompareERR) != std::string::npos){
			printf("Erroneous Reply!! .. %s \r\n", receivedS.c_str());
			return false;

		}
	} while (receivedS.find(toCompareOK) == std::string::npos);
	dbgprintf("REPLY:\r\n%s\r\n", receivedS.c_str());
	testprintf("%s\r\n", receivedS.c_str());
	m_lastATreply = receivedS;
	return true;
}
