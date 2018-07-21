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

#include "stdafx.h"

Coordinator::Coordinator(SOCKET inClientSocket) {
	clientSocket = inClientSocket;
	commander.simInit();
	commander.mouseInit();
}

Coordinator::Coordinator(int portNumber) {
	//// Initialize Winsock
	//iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//if (iResult != 0) {
	//	printf("WSAStartup failed: %d\n", iResult);
	//	exit(1);
	//}

	//ZeroMemory(&hints, sizeof(hints));
	//hints.ai_family = AF_INET;
	//hints.ai_socktype = SOCK_STREAM;
	//hints.ai_protocol = IPPROTO_TCP;
	//hints.ai_flags = AI_PASSIVE;

	//// Resolve the local address and port to be used by the server
	//iResult = getaddrinfo(NULL, std::to_string(portNumber).c_str(), &hints, &result);
	//if (iResult != 0) {
	//	printf("getaddrinfo failed: %d\n", iResult);
	//	WSACleanup();
	//	exit(1);
	//}

	//// Make sure we got at least one address back
	//if (result == NULL) {
	//	printf("Unable to resolve interface\n");
	//}

	//// Create a SOCKET for the server to listen for client connections
	//listenSocket =
	//	socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	//if (listenSocket == INVALID_SOCKET) {
	//	printf("Error at socket(): %ld\n", WSAGetLastError());
	//	freeaddrinfo(result);
	//	exit(1);
	//}

	//// ======== BIND ========
	//// Setup the TCP listening socket
	//iResult = bind(listenSocket, result->ai_addr, result->ai_addrlen);
	//if (iResult == SOCKET_ERROR) {
	//	printf("bind failed with error: %d\n", WSAGetLastError());
	//	freeaddrinfo(result);
	//	closesocket(listenSocket);
	//	WSACleanup();
	//	exit(1);
	//}
	//freeaddrinfo(result);
	//
	//// ======== LISTEN ON A SOCKET ========
	//if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
	//	printf("Listen failed with error: %ld\n", WSAGetLastError());
	//	closesocket(listenSocket);
	//	WSACleanup();
	//	exit(1);
	//}
}

Coordinator::~Coordinator() {
	// ======== SHUTDOWN SOCKET ========
	// shutdown the send half of connection since no more data will be sent
	iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		exit(1);
	}

	// cleanup
	closesocket(clientSocket);
	WSACleanup();
}

// When the client connection has been accepted,
// a server application would normally pass the accepted client socket
// (the ClientSocket variable in the above sample code) to a worker thread
// or an I/O completion port and continue accepting additional connections.
bool Coordinator::acceptConnection() {
	// Accept a client socket
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return false;
	} else {
		return true;
	}
}

bool Coordinator::getConnectionStatus() {
	return isClientConnected;
}

// Receive messages came from client over socket
string Coordinator::receiveMessage() {
	iResult = recv(clientSocket, receiveBuffer, defaultBuffLength, 0);

	if (iResult == SOCKET_ERROR) {
		printf("recv failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		exit(1);
	}
	printf("%d bytes received: %s\n", iResult, receiveBuffer);
	return receiveBuffer;
}

int Coordinator::sendMessage(string mMessage) {
	iResult = send(clientSocket, mMessage.c_str(), defaultBuffLength, 0);

	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		exit(1);
	}
	printf("%d bytes sent: %s\n", iResult, mMessage.c_str());
	return iResult;
}

// Core serving routine
int Coordinator::serve() {
	// Receive command
	((string)receiveBuffer).clear();
	iResult = recv(clientSocket, receiveBuffer, defaultBuffLength, 0);
	// Error processing
	if (iResult == SOCKET_ERROR) {
		printf("recv failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	} else if (iResult == 0) {
		// Client connection is closed
		iResult = shutdown(clientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("Socket shutdown failed: %d\n", WSAGetLastError());
			return -1;
		}
		return 0;
	}
	// Command processing
	printf("%d bytes received: %s\n", iResult, receiveBuffer);
	userCommand = (string)receiveBuffer;

	// Command -> move
	if (!userCommand.compare(0, 4, "move")) {
		commander.moveBlock(1);
		printf("Command-> move\n");
		iResult = send(clientSocket, "Done. 1 block forward.\n", 24, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> back
	if (!userCommand.compare(0, 4, "back")) {
		commander.turnUTurn();
		commander.moveBlock(1);
		commander.turnUTurn();
		printf("Command-> back\n");
		iResult = send(clientSocket, "Done. 1 block backward.\n", 25, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> half
	if (!userCommand.compare(0, 4, "half")) {
		commander.moveHalf();
		printf("Command-> moveHalf\n");
		iResult = send(clientSocket, "Done. Half block forward\n", 26, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> left
	if (!userCommand.compare(0, 4, "left")) {
		commander.turnLeft(90);
		printf("Command-> left\n");
		iResult = send(clientSocket, "Done. 90 left.\n", 16, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> right
	if (!userCommand.compare(0, 5, "right")) {
		commander.turnRight(90);
		printf("Command-> right\n");
		iResult = send(clientSocket, "Done. 90 right.\n", 17, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> uTurn
	if (!userCommand.compare(0, 5, "uTurn")) {
		commander.turnUTurn();
		printf("Command-> uTurn\n");
		iResult = send(clientSocket, "Done. U-Turn.\n", 15, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}

	// Command -> senseLeft
	if (!userCommand.compare(0, 9, "senseLeft")) {
		printf("Command-> senseLeft\n");
		iResult = send(clientSocket, (to_string(commander.senseLeftWall()) + "\n").c_str(), 2, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> senseFront
	if (!userCommand.compare(0, 10, "senseFront")) {
		printf("Command-> senseFront\n");
		iResult = send(clientSocket, (to_string(commander.senseFrontWall()) + "\n").c_str(), 2, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> senseRight
	if (!userCommand.compare(0, 10, "senseRight")) {
		printf("Command-> senseRight\n");
		iResult = send(clientSocket, (to_string(commander.senseRightWall()) + "\n").c_str(), 2, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}
	// Command -> senseGoal
	if (!userCommand.compare(0, 9, "senseGoal")) {
		printf("Command-> senseGoal\n");
		iResult = send(clientSocket, (to_string(commander.senseGoal()) + "\n").c_str(), 2, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}


	// Command -> reset
	if (!userCommand.compare(0, 5, "reset")) {
		commander.mouseInit();
		printf("Command-> reset\n");
		iResult = send(clientSocket, "Reset done.\n", 23, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return iResult;
	}

	// Command -> shutdownServer
	if (!userCommand.compare(0, 14, "shutdownServer")) {
		printf("Command-> shutdownServer\n");
		iResult = send(clientSocket, "Shutting down server.\n", 23, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			return -1;
		}
		return -2;
	}

	// Unknown command
	printf("Command-> unknown\n");
	iResult = send(clientSocket, "Unknown command, try again.\n", 29, 0);
	if (iResult == SOCKET_ERROR) {
		printf("Send failed: %d\n", WSAGetLastError());
		return -1;
	}
	return iResult;
}
// END OF FILE