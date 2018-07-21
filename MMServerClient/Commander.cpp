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


Commander::Commander() {
	hDLL = LoadLibrary(L"vbmms.dll");
	if (hDLL == NULL) {
		printf("lib load failed.\n");
	} else {
		printf("Dll loaded.\n");
	}
}

Commander::~Commander() {
	FreeLibrary(hDLL);
}

// Initialize simulation.
// return 1: OK otherwise error
int Commander::simInit() {
	return ((dllIntVoid)GetProcAddress(hDLL, "SimInit"))();
}

// Initialize mouse location to start
void Commander::mouseInit() {
	((dllVoidVoid)GetProcAddress(hDLL, "MouseInit"))();
}

// Go along diretion by "amount" blocks
void Commander::moveBlock(int amount) {
	((dllVoidInt)GetProcAddress(hDLL, "MoveBlock"))((short)amount);
}

// Go along direction by half a block
void Commander::moveHalf() {
	((dllVoidVoid)GetProcAddress(hDLL, "MoveHalf"))();
}

// Turn left by "degree" degrees. 45/90/135
void Commander::turnLeft(int degree) {
	((dllVoidInt)GetProcAddress(hDLL, "TurnLeft"))((short)degree);
}

// Turn right by "degree" degrees. 45/90/135
void Commander::turnRight(int degree) {
	((dllVoidInt)GetProcAddress(hDLL, "TurnRight"))((short)degree);
}

// Do a U-turn
void Commander::turnUTurn() {
	((dllVoidVoid)GetProcAddress(hDLL, "Turn180"))();
}

// Front sensor
// return 1: wall / 0: no wall
int Commander::senseFrontWall() {
	return ((dllIntVoid)GetProcAddress(hDLL, "InputFW"))();
}

// Left sensor
// return 1: wall / 0: no wall
int Commander::senseLeftWall() {
	return ((dllIntVoid)GetProcAddress(hDLL, "InputLW"))();
}

// Right sensor
// return 1: wall / 0: no wall
int Commander::senseRightWall() {
	return ((dllIntVoid)GetProcAddress(hDLL, "InputRW"))();
}

// Have we got there?
// return 1: yes / 0: no
int Commander::senseGoal() {
	return ((dllIntVoid)GetProcAddress(hDLL, "InputGL"))();
}
// END OF FILE