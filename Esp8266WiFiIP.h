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

#ifndef ESP8266_WIFIIP
#define ESP8266_WIFIIP

#include <map>
#include "Esp8266WiFiPhy.h"

#define WIFI_DHCP_DISABLE 			0
#define WIFI_DHCP_ENABLE 			1

#define WIFI_DHCP_SOFTAP 			0
#define WIFI_DHCP_STATION 			1
#define WIFI_DHCP_BOTH	 			2

typedef map<string,string> APDevicesList;

class StaticIP{
public:
	StaticIP();
	StaticIP(string address, string gway, string nmask);
	virtual ~StaticIP();
	string m_IPAddress;
	string m_IPGateway;
	string m_IPNetmask;
};

class DHCPServer{
public:
	DHCPServer();
	DHCPServer(uint32_t lease, string startIP, string endIP);
	virtual ~DHCPServer();
	uint32_t m_lease_minutes; //between 1 and 2880
	string m_IPStart;
	string m_IPEnd;
};

class Esp8266WiFiIP : public Esp8266WiFiPhy {
public:
	Esp8266WiFiIP();
	virtual ~Esp8266WiFiIP();

	StaticIP m_myStationAPStaticIP;
	StaticIP m_mySoftAPStaticIP;

//protected:
	void DHCPEnable(uint8_t mode, uint8_t enable, bool flashStore=false);
	int8_t GetDHCPStatus(bool flashStored=false);

	StaticIP GetStaticIPStationAP(bool flashStored=false);
	void SetStaticIPStationAP(StaticIP statAP, bool flashStore=false);
	StaticIP GetStaticIPSoftAP(bool flashStored=false);
	void SetStaticIPSoftAP(StaticIP softAP, bool flashStore=false);

	DHCPServer GetDHCPRange(bool flashStored=false);
	void SetDHCPRange(DHCPServer servConf, bool def_range=false, bool flashStore=false);
	APDevicesList GetSoftAPConnectedDevices(void);

};

#endif
