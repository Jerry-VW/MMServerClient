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

// MMServer.cpp : Main entry point for the console application.
//

#include "stdafx.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEBUG

SOCKET listenSocket = INVALID_SOCKET;

void initializeProcess(int portNumber) {
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL, hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, std::to_string(portNumber).c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	// Make sure we got at least one address back
	if (result == NULL) {
		printf("Unable to resolve interface\n");
		WSACleanup();
		exit(1);
	}

	// Create a SOCKET for the server to listen for client connections
	listenSocket =
		socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		exit(1);
	}
	int enable = 1;
	if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int)) < 0) {
		printf("setsockopt(SO_REUSEADDR) failed");
		WSACleanup();
		exit(1);
	}

	// ======== BIND ========
	// Setup the TCP listening socket
	iResult = bind(listenSocket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		exit(1);
	}
	freeaddrinfo(result);

	// ======== LISTEN ON A SOCKET ========
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		exit(1);
	}
}

// Server thread that processes R/W with client
DWORD WINAPI serverThread(LPVOID lpParam) {
	int returnValue = 1;
	printf("New client connected.\n");
	Coordinator coordinator = Coordinator((SOCKET)lpParam);
	while (returnValue > 0) {
		returnValue = coordinator.serve();
	}
	printf("Client disconnected.\n");
	if (returnValue == -2) {
		// Server shutdown routine flag
		closesocket((SOCKET)lpParam);
		WSACleanup();
	}
	ExitThread(0); // SHOULD NOT BE return but ExitThread()
}

int main() {
	SetConsoleOutputCP(65001); // Set console code page to UTF-8

#ifdef DEBUG
	WCHAR logBuffer[MAX_PATH];

	printf("MicroMouseServer  Copyright (C) 2018  WENQIAN WANG\nThis program comes with ABSOLUTELY NO WARRANTY\n");
	if (!GetCurrentDirectory(MAX_PATH, logBuffer)) {
		// error fetching
		printf("error fetching current directory, exit.\n");
		return 1;
	}
	printf("Current directory is: %ls\n", logBuffer);

	//if (!SetCurrentDirectory(L"..\\Release\\")) {
	//	// error redirecting
	//	printf("error changing directory, exit.\n");
	//	return 1;
	//}
	//GetCurrentDirectory(MAX_PATH, logBuffer);
	//printf("Changed to: %ls\n", logBuffer);

#endif // DEBUG

	string userCommand;
	SOCKET clientSocket = INVALID_SOCKET;

	initializeProcess(23333);

	// Serve process
	//TODO: Error processing 2nd accept code:10093 WSANOTINITIALISED
	printf("Server initialized. Waiting for clients.\n");

	while (true) {
		// Accept a client socket
		clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			break;
		}
		else {
			// Established a connection, kick off new thread with client socket info
			CreateThread(NULL, 0, serverThread, (LPVOID)clientSocket, 0, NULL);
		}
	}

	WSACleanup();
	printf("Server shutdown. Goodbye.\n");
	return 0;
}

// END OF FILE
