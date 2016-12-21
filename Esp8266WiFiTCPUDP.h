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

#ifndef ESP8266_WIFITCPUDP
#define ESP8266_WIFITCPUDP

#include "Esp8266WiFiIP.h"

#define UDP_PEER_NOCHANGE 			0
#define UDP_PEER_CHANGE_ONCE 		1
#define UDP_PEER_CHANGE_MULTIPLE 	2

class Esp8266WiFiTCPUDP : public Esp8266WiFiIP {
public:
	Esp8266WiFiTCPUDP();
	virtual ~Esp8266WiFiTCPUDP();

	void UpdateListActiveConnections(uint8_t &clientserver, uint32_t &localPort, uint32_t &remotePort,
									 string &remoteIP, string &conntype, uint8_t &linkID);
	void GetLocalIPAddress(APDevicesList &softAP, APDevicesList &stationAP);

	int8_t GetMultipleConnectionsStatus(void);
	void EnableMultipleConnections(bool multipleEnabled=true);

	bool TCPClientConnectTo(string IPremote, uint32_t portRemote, uint32_t keepaliveSecs=0, uint8_t linkID=0);

	bool ConnectionClose(uint8_t linkID=5);
	bool Ping(string host);

	bool TCPServerBind(uint32_t portLocal, bool remove=false);
	int GetTCPServerTimeout(void);
	bool SetTCPServerTimeout(uint32_t timeout_s);

	bool TCPSend(char * data, int bufflength, uint8_t linkID=0);
	int ReceiveFromLinkID(char * data, int &bufflength, uint8_t &linkID);

	//OK

	bool EnableShowRemoteInReceive(bool enable=true);
	bool UDPClientServerBind(string IPremote, uint32_t portRemote, uint32_t portLocal,
							 uint8_t mode=UDP_PEER_CHANGE_MULTIPLE, uint8_t linkID=0);
	bool UDPClientServerConnect(string IPremote, uint32_t portRemote, uint8_t linkID=0);
	bool UDPSendTo(char * data, int bufflength, string IPremote, uint32_t portRemote, uint8_t linkID=0);
    int ReceiveFrom(string &IPRemote, uint32_t &portRemote, char * data, int &bufflength, uint8_t &linkID);
    int NonBlockingReceiveFrom(string &IPRemote, uint32_t &portRemote, char * data, int &bufflength, uint8_t &linkID);

	bool m_multipleConnMode;
	bool m_showremote;
};

#endif
