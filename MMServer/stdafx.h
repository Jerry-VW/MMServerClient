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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include <string>

#include "Commander.h"
#include "Coordinator.h"

using namespace std;

// TODO: reference additional headers your program requires here
