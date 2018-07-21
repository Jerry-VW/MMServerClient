/*
MMServer
Enable remote control of your virtual micromouse through network communication.
Copyright(C) 2018  WENQIAN WANG

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")



class Coordinator {
public:
	Coordinator(int);
	Coordinator(SOCKET);
	~Coordinator();
	bool acceptConnection();
	bool getConnectionStatus();
	std::string receiveMessage();
	int sendMessage(std::string);

	int serve();

private:
	const std::string defaultPort;
	const static int defaultBuffLength = 512;
	char receiveBuffer[defaultBuffLength];
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
	bool isClientConnected = false;
	Commander commander = Commander();
	std::string userCommand;
};
// END OF FILE