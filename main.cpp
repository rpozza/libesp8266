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
 * Authors:
 * Riccardo Pozza <r.pozza@surrey.ac.uk>
 */

#include "Esp8266WiFiTCPUDP.h"

int main(void) {
	//NB: comment out protected: in classes before trying test programs!

//	//----- FIRST TEST PROGRAM - LOWER LAYER AT COMMANDS TESTING ------
//	printf("----------------------- FIRST TEST PROGRAM - LOWER LAYER AT COMMANDS TESTING ---------------------------\r\n");
//	Esp8266BaseAT *wifi = new Esp8266BaseAT();
//
//	wifi->SetStationSleepMode(DISABLE_SLEEP);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetStationSleepMode());
//	wifi->SetStationSleepMode(LIGHT_SLEEP);
//	dbgprintf("MODE SET TO : %d\r\m", (int) wifi->GetStationSleepMode());
//	wifi->SetStationSleepMode();
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetStationSleepMode());
//
//	wifi->GetStationSleepMode();
//
//	wifi->RestartModule();
//	wait_ms(1000);
//
//	uint32_t sleepTime=2000;
//	dbgprintf("Non Blocking - Sleeping for %d ms\r\n", (int) sleepTime);
//	wifi->DeepSleep(sleepTime);
//	// not sure working (check return string and connection XPD_DCDC with EXT_RSTB)
//	printf("-------------------- END OF FIRST TEST PROGRAM - LOWER LAYER AT COMMANDS TESTING  ----------------------\r\n");

//	//----- SECOND TEST PROGRAM - INFRASTRUCTURE MODE TESTING ------
//	printf("------------------------ SECOND TEST PROGRAM - INFRASTRUCTURE MODE TESTING -----------------------------\r\n");
//	Esp8266WiFiPhy *wifi = new Esp8266WiFiPhy();
//
//	wifi->SetWiFiMode(WIFI_MODE_STA);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetWiFiMode();
//
//	wifi->SetWiFiMode();
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetWiFiMode();
//
//	StationAccessPoint mobileGateway("MGTestAP", "8f268e44","");
//	wifi->ConnectToStationAP(mobileGateway);
//	dbgprintf("Connected to AccessPoint: <%s>\r\n", wifi->m_stationAP.m_ssid.c_str());
//
//	mobileGateway = wifi->GetConnectedStationAP();
//	dbgprintf("Connected to AccessPoint: <%s>\r\n", mobileGateway.m_ssid.c_str());
//
//	dbgprintf("Listing all AccessPoints NB \r\n");
//	//wifi->ListStationAP(); TODO::increase rx buffer or decrease speed for this command to avoid overwriting!
//	dbgprintf("Looking for specific mobile AP\r\n");
//	wifi->ListStationAP(mobileGateway);
//
//	dbgprintf("Quit the connected AP\r\n");
//	wifi->QuitStationAP();
//
//	dbgprintf("Disable AutoConnect To Station at Boot\r\n");
//	wifi->AutoConnectToStationAtBoot(false);
//
//	dbgprintf("Retrieve Station AP Interface MAC Address: <%s>\r\n", wifi->GetStationAPMac().c_str());
//	wifi->GetStationAPMac();
//	wifi->SetStationAPMac("18:fe:35:98:d3:7b");
//	dbgprintf("Retrieve Station AP Interface MAC Address: <%s>\r\n", wifi->GetStationAPMac().c_str());
//	printf("--------------------- END OF SECOND TEST PROGRAM - INFRASTRUCTURE MODE TESTING -------------------------\r\n");

//	//----- THIRD TEST PROGRAM - SOFT ACCESS POINT MODE TESTING ------
//	printf("----------------------- THIRD TEST PROGRAM - SOFT ACCESS POINT MODE TESTING  ---------------------------\r\n");
//	Esp8266WiFiPhy *wifi = new Esp8266WiFiPhy();
//
//	wifi->SetWiFiMode(WIFI_MODE_STA_AND_AP);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetWiFiMode();
//
//	SoftAccessPoint iotDeviceAP;
//	wifi->ConfigureSoftAP(iotDeviceAP);
//	dbgprintf("Configured as SoftAccessPoint: <%s>\r\n", wifi->m_softAP.m_ssid.c_str());
//
//	iotDeviceAP = wifi->GetConfigurationSoftAP();
//	dbgprintf("Retrieved SoftAccessPoint Configuration: <%s>\r\n", wifi->m_softAP.m_ssid.c_str());
//
//	dbgprintf("Retrieve Soft AP Interface MAC Address: <%s>\r\n", wifi->GetSoftAPMac().c_str());
//	wifi->GetSoftAPMac();
//
//	wifi->SetSoftAPMac("18:fe:35:98:d3:7b");
//	dbgprintf("Retrieve Station AP Interface MAC Address: <%s>\r\n", wifi->GetSoftAPMac().c_str());
//
//	printf("-------------------- END OF THIRD TEST PROGRAM - SOFT ACCESS POINT MODE TESTING  -----------------------\r\n");

//	//----- FOURTH TEST PROGRAM - INFRASTRUCTURE MODE IP TEST ------
//	printf("------------------------- FOURTH TEST PROGRAM - INFRASTRUCTURE MODE IP TEST ----------------------------\r\n");
//
//	Esp8266WiFiIP *wifi = new Esp8266WiFiIP();
//
//	wifi->SetWiFiMode(WIFI_MODE_STA);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetDHCPStatus();
//
//	wifi->DHCPEnable(WIFI_DHCP_STATION, WIFI_DHCP_ENABLE);
//
//	StationAccessPoint mobileGateway("MGTestAP", "8f268e44","");
//	wifi->ConnectToStationAP(mobileGateway);
//	dbgprintf("Connected to AccessPoint: <%s>\r\n", wifi->m_stationAP.m_ssid.c_str());
//
//	StaticIP localAddr;
//	localAddr = wifi->GetStaticIPStationAP();
//	dbgprintf("Connected to AccessPoint: <%s>\r\n", localAddr.m_IPAddress.c_str());
//
//	StaticIP newAddr("192.168.43.80","192.168.43.1","255.255.255.0");
//	wifi->SetStaticIPStationAP(newAddr);
//
//	localAddr = wifi->GetStaticIPStationAP();
//	dbgprintf("New Address Assigned Statically: <%s>\r\n", localAddr.m_IPAddress.c_str());
//
//	wifi->GetDHCPStatus();
//	printf("--------------------- END OF FOURTH TEST PROGRAM - INFRASTRUCTURE MODE IP TEST -------------------------\r\n");

//	//----- FIFTH TEST PROGRAM - SOFT ACCESS POINT MODE IP TEST ------
//	printf("------------------------ FIFTH TEST PROGRAM - SOFT ACCESS POINT MODE IP TEST ---------------------------\r\n");
//
//	Esp8266WiFiIP *wifi = new Esp8266WiFiIP();
//
//	wifi->SetWiFiMode(WIFI_MODE_AP);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetWiFiMode();
//
//	wifi->GetDHCPStatus();
//	wifi->DHCPEnable(WIFI_DHCP_SOFTAP, WIFI_DHCP_ENABLE);
//
//	SoftAccessPoint iotDeviceAP;
//	wifi->ConfigureSoftAP(iotDeviceAP);
//	dbgprintf("Configured as SoftAccessPoint: <%s>\r\n", wifi->m_softAP.m_ssid.c_str());
//
//	wifi->GetDHCPRange();
//	DHCPServer ipRanges(5000,"192.168.4.10","192.168.4.20");
//	wifi->SetDHCPRange(ipRanges);
//
//	wifi->GetDHCPRange();
//
//	APDevicesList connectedToAP;
//	dbgprintf("------------------- WiFi List IP addresses ----------------------\r\n");
//	for (int k=0;k<5;k++){
//		connectedToAP = wifi->GetSoftAPConnectedDevices();
//		for (APDevicesList::iterator it = connectedToAP.begin(); it != connectedToAP.end(); ++it){
//			dbgprintf("MAC and associated IP: <%s>,<%s>\r\n", it->first.c_str(), it->second.c_str());
//		}
//		wait_ms(5000);
//	}
//	printf("--------------------- END OF FIFTH TEST PROGRAM - SOFT ACCESS POINT MODE IP TEST -----------------------\r\n");

//	//----- SIXTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST ------
//	printf("---------------------- SIXTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST ------------------------\r\n");
//	Esp8266WiFiTCPUDP *wifi = new Esp8266WiFiTCPUDP();
//
//	wifi->SetWiFiMode(WIFI_MODE_STA_AND_AP);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetWiFiMode();
//
//	wifi->GetDHCPStatus();
//	wifi->DHCPEnable(WIFI_DHCP_BOTH, WIFI_DHCP_ENABLE);
//
//	SoftAccessPoint iotDeviceAP;
//	wifi->ConfigureSoftAP(iotDeviceAP);
//	dbgprintf("Configured as SoftAccessPoint: <%s>\r\n", wifi->m_softAP.m_ssid.c_str());
//
//	wifi->GetDHCPRange();
//	DHCPServer ipRanges(5000,"192.168.4.10","192.168.4.20");
//	wifi->SetDHCPRange(ipRanges);
//
//	wifi->GetDHCPRange();
//
//	APDevicesList connectedToAP, mySoftAP, myStationAP;
//	dbgprintf("------------------- WiFi List IP addresses ----------------------\r\n");
//	do {
//		printf("please connect to AP\r\n");
//		wait_ms(5000);
//		connectedToAP = wifi->GetSoftAPConnectedDevices();
//	}while (connectedToAP.empty());
//
//	// only one device connected for now
//	dbgprintf("Remote <MAC>,<IP>: <%s>,<%s>\r\n", connectedToAP.begin()->first.c_str(), connectedToAP.begin()->second.c_str());
//
//	printf("-------------------------------------- TCP AND UDP TEST PROGRAM ----------------------------------------\r\n");
//
//	wifi->GetLocalIPAddress(mySoftAP, myStationAP);
//
//	dbgprintf("SoftAP <MAC>,<IP>: <%s>,<%s>\r\n", mySoftAP.begin()->first.c_str(), mySoftAP.begin()->second.c_str());
//	dbgprintf("StationAP <MAC>,<IP>: <%s>,<%s>\r\n", myStationAP.begin()->first.c_str(), myStationAP.begin()->second.c_str());
//
//	printf("--------------------------------------- TCP CLIENT TEST PROGRAM ----------------------------------------\r\n");
//
//	wifi->GetMultipleConnectionsStatus();
//
//	// No linkID needed single single connection mode
//	wifi->TCPClientConnectTo(connectedToAP.begin()->second,23);
//
//	char something[1024] = "hello world\r\n";
//
//	wifi->TCPSend(something,strlen(something));
//
//	uint8_t link;
//	int number_bytes = 0;
//	bool find_something = false;
//	memset(something,0,1024);
//	do {
//		if (wifi->ReceiveFromLinkID(something,number_bytes,link) > 0){
//			dbgprintf("Received <%s>\r\n", something);
//			find_something = true;
//		}
//	}while (!find_something);
//
//	uint8_t cs, lid;
//	uint32_t lport, rport;
//	string remIP, conn;
//	wifi->UpdateListActiveConnections(cs,lport,rport,remIP,conn,lid);
//	dbgprintf("Opened %s (C=0/S=1) %d mode connection to %s : %d port via %d port, with LinkID %d\r\n", conn.c_str(),cs, remIP.c_str(), rport, lport, lid);
//
//	wifi->Ping(connectedToAP.begin()->second);
//	wifi->ConnectionClose();
//
//	printf("------------------- END OF SIXTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST --------------------\r\n");

//	//----- SEVENTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST ------
//	printf("--------------------- SEVENTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST -----------------------\r\n");
//	Esp8266WiFiTCPUDP *wifi = new Esp8266WiFiTCPUDP();
//
//	wifi->SetWiFiMode(WIFI_MODE_STA_AND_AP);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetWiFiMode();
//
//	wifi->GetDHCPStatus();
//	wifi->DHCPEnable(WIFI_DHCP_BOTH, WIFI_DHCP_ENABLE);
//
//	SoftAccessPoint iotDeviceAP;
//	wifi->ConfigureSoftAP(iotDeviceAP);
//	dbgprintf("Configured as SoftAccessPoint: <%s>\r\n", wifi->m_softAP.m_ssid.c_str());
//
//	wifi->GetDHCPRange();
//	DHCPServer ipRanges(5000,"192.168.4.10","192.168.4.20");
//	wifi->SetDHCPRange(ipRanges);
//
//	wifi->GetDHCPRange();
//
//	APDevicesList connectedToAP, mySoftAP, myStationAP;
//	dbgprintf("------------------- WiFi List IP addresses ----------------------\r\n");
//	do {
//		printf("please connect to AP\r\n");
//		wait_ms(5000);
//		connectedToAP = wifi->GetSoftAPConnectedDevices();
//	}while (connectedToAP.empty());
//
//	// only one device connected for now
//	dbgprintf("Remote <MAC>,<IP>: <%s>,<%s>\r\n", connectedToAP.begin()->first.c_str(), connectedToAP.begin()->second.c_str());
//
//	printf("-------------------------------------- TCP AND UDP TEST PROGRAM ----------------------------------------\r\n");
//
//	wifi->GetLocalIPAddress(mySoftAP, myStationAP);
//
//	dbgprintf("SoftAP <MAC>,<IP>: <%s>,<%s>\r\n", mySoftAP.begin()->first.c_str(), mySoftAP.begin()->second.c_str());
//	dbgprintf("StationAP <MAC>,<IP>: <%s>,<%s>\r\n", myStationAP.begin()->first.c_str(), myStationAP.begin()->second.c_str());
//
//	printf("--------------------------------------- TCP SERVER TEST PROGRAM ----------------------------------------\r\n");
//
//	wifi->EnableMultipleConnections();
//
//	wifi->TCPServerBind(23);
//
//	dbgprintf("Default Timeout: %d\r\n", wifi->GetTCPServerTimeout());
//
//	wifi->GetTCPServerTimeout();
//
//	wifi->SetTCPServerTimeout(120);
//
//	char something[1024] = "hello world\r\n";
//	uint8_t link=0;
//	do {
//		printf("please connect to TCP Server\r\n");
//		wait_ms(1000);
//	} while (!wifi->TCPSend(something,strlen(something),link));
//	//TODO rearrange to allow connection before sending
//
//	int number_bytes = 0;
//	bool find_something = false;
//	memset(something,0,1024);
//	do {
//		printf("please send a reply to TCP Server\r\n");
//		if (wifi->ReceiveFromLinkID(something,number_bytes,link) > 0){
//			dbgprintf("Received <%s>\r\n", something);
//			find_something = true;
//		}
//	}while (!find_something);
//
//	uint8_t cs, lid;
//	uint32_t lport, rport;
//	string remIP, conn;
//	wifi->UpdateListActiveConnections(cs,lport,rport,remIP,conn,lid);
//	dbgprintf("Accepted %s (C=0/S=1) %d mode connection from %s : %d port to %d port, with LinkID %d\r\n", conn.c_str(),cs, remIP.c_str(), rport, lport, lid);
//
//	wifi->Ping(connectedToAP.begin()->second);
//	wifi->ConnectionClose(link);
//
//	printf("------------------ END OF SEVENTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST -------------------\r\n");

//	//----- EIGHTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST ------
//	printf("--------------------- EIGHTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST -----------------------\r\n");
//	Esp8266WiFiTCPUDP *wifi = new Esp8266WiFiTCPUDP();
//
//	wifi->SetWiFiMode(WIFI_MODE_STA_AND_AP);
//	dbgprintf("MODE SET TO : %d\r\n", (int) wifi->GetWiFiMode());
//
//	wifi->GetWiFiMode();
//
//	wifi->GetDHCPStatus();
//	wifi->DHCPEnable(WIFI_DHCP_BOTH, WIFI_DHCP_ENABLE);
//
//	SoftAccessPoint iotDeviceAP;
//	wifi->ConfigureSoftAP(iotDeviceAP);
//	dbgprintf("Configured as SoftAccessPoint: <%s>\r\n", wifi->m_softAP.m_ssid.c_str());
//
//	wifi->GetDHCPRange();
//	DHCPServer ipRanges(5000,"192.168.4.10","192.168.4.20");
//	wifi->SetDHCPRange(ipRanges);
//
//	wifi->GetDHCPRange();
//
//	APDevicesList connectedToAP, mySoftAP, myStationAP;
//	dbgprintf("------------------- WiFi List IP addresses ----------------------\r\n");
//	do {
//		printf("please connect to AP\r\n");
//		wait_ms(5000);
//		connectedToAP = wifi->GetSoftAPConnectedDevices();
//	}while (connectedToAP.empty());
//
//	// only one device connected for now
//	dbgprintf("Remote <MAC>,<IP>: <%s>,<%s>\r\n", connectedToAP.begin()->first.c_str(), connectedToAP.begin()->second.c_str());
//
//	printf("-------------------------------------- TCP AND UDP TEST PROGRAM ----------------------------------------\r\n");
//
//	wifi->GetLocalIPAddress(mySoftAP, myStationAP);
//
//	dbgprintf("SoftAP <MAC>,<IP>: <%s>,<%s>\r\n", mySoftAP.begin()->first.c_str(), mySoftAP.begin()->second.c_str());
//	dbgprintf("StationAP <MAC>,<IP>: <%s>,<%s>\r\n", myStationAP.begin()->first.c_str(), myStationAP.begin()->second.c_str());
//
//	printf("--------------------------------------- UDP CLI/SER TEST PROGRAM ---------------------------------------\r\n");
//
//	wifi->EnableMultipleConnections();
//
//	wifi->EnableShowRemoteInReceive();
//
//	uint8_t link=4;
//	wifi->UDPClientServerBind(connectedToAP.begin()->second, 4023, 23, UDP_PEER_CHANGE_MULTIPLE, link);
//
//	char something[1024] = "hello world\r\n";
//
//	wifi->UDPSendTo(something, strlen(something), connectedToAP.begin()->second, 4023, link);
//
//	string anIP = "";
//	uint32_t aPort;
//	int number_bytes = 0;
//	bool find_something = false;
//	memset(something,0,1024);
//	do {
//		printf("Blocking I/O\r\n");
//		if (wifi->ReceiveFrom(anIP, aPort, something, number_bytes, link) > 0){
//			dbgprintf("Received <%s> from <%s>:<%d> on link <%d>\r\n", something, anIP.c_str(), aPort, link);
//			find_something = true;
//		}
//	}while (!find_something);
//
//	anIP = "";
//	aPort = 0;
//	number_bytes = 0;
//	find_something = false;
//	memset(something,0,1024);
//	do {
//		printf("Non Blocking I/O\r\n");
//		if (wifi->NonBlockingReceiveFrom(anIP, aPort, something, number_bytes, link) > 0){
//			dbgprintf("Received <%s> from <%s>:<%d> on link <%d>\r\n", something, anIP.c_str(), aPort, link);
//			find_something = true;
//		}
//	}while (!find_something);
//
//
//
//	uint8_t cs, lid;
//	uint32_t lport, rport;
//	string remIP, conn;
//	wifi->UpdateListActiveConnections(cs,lport,rport,remIP,conn,lid);
//	dbgprintf("Connected %s (C=0/S=1) %d mode connection to %s : %d port via %d port, with LinkID %d\r\n", conn.c_str(),cs, remIP.c_str(), rport, lport, lid);
//
//	wifi->ConnectionClose(link);
//	printf("------------------ END OF EIGHTH TEST PROGRAM - SOFT ACCESS POINT MODE TCP UDP TEST --------------------\r\n");
}
