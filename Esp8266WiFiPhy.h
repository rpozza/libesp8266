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

#ifndef ESP8266_WIFIPHY
#define ESP8266_WIFIPHY

#include "Esp8266BaseAT.h"

#define WIFI_MODE_STA				1
#define WIFI_MODE_AP				2
#define WIFI_MODE_STA_AND_AP		3

#define WIFI_ENC_OPEN				0
#define WIFI_ENC_WPA_PSK			2
#define WIFI_ENC_WPA2_PSK			3
#define WIFI_ENC_WPA_WPA2_PSK		4

#define WIFI_MAX_CONNECTIONS		4

#define WIFI_BROADCAST_SSID			0
#define WIFI_HIDDEN_SSID			1


class StationAccessPoint {
public:
	StationAccessPoint();
	StationAccessPoint(string ssid, string password, string bssidmac);
	StationAccessPoint(string ssid, string bssidmac, int channel, int rssi);
	virtual ~StationAccessPoint();
	string m_ssid;
	string m_password;
	string m_bssidmac;
	int m_channel;
	int m_rssi;
};

class SoftAccessPoint {
public:
	SoftAccessPoint();
	SoftAccessPoint(string ssid, string password, int channel, int encryption, int max_conn, int ssid_bcast);
	virtual ~SoftAccessPoint();
	string m_ssid;
	string m_password;
	int m_channel;
	int m_encryption;
	int m_maxconnections;
	int m_ssidbroadcast;
};

class Esp8266WiFiPhy : public Esp8266BaseAT {
public:
	Esp8266WiFiPhy();
	virtual ~Esp8266WiFiPhy();

	StationAccessPoint m_stationAP;
	SoftAccessPoint m_softAP;

	string m_softAPInterfaceMac;
	string m_stationAPInterfaceMac;

	int8_t GetWiFiMode(bool flashStored=false);
	void SetWiFiMode(uint8_t mode=WIFI_MODE_STA_AND_AP, bool flashStore=false);

	StationAccessPoint GetConnectedStationAP(bool flashStored=false);
	bool ConnectToStationAP (StationAccessPoint sap, bool flashStore=false);
	void ListStationAP(StationAccessPoint sap = StationAccessPoint("","",""));
	void QuitStationAP(void);
	void AutoConnectToStationAtBoot(bool enable=false);
	string GetStationAPMac(bool flashStored=false);
	void SetStationAPMac(string mac_addr, bool flashStore=false);

	SoftAccessPoint GetConfigurationSoftAP(bool flashStored=false);
	void ConfigureSoftAP(SoftAccessPoint sap, bool flashStore=false);
	string GetSoftAPMac(bool flashStored=false);
	void SetSoftAPMac(string mac_addr, bool flashStore=false);

protected:
	string StringEscape (string input);
	string StringUnescape (string input);
	string GetStringBetweenTokens (char starttoken, char endtoken, string input, size_t &from);

	bool DoubleWaitFor(string toCompareOK, string toCompareERR);
};

#endif
