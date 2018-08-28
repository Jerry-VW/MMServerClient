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

class Commander {
public:
	Commander();
	~Commander();

	int simInit();
	void mouseInit();

	void moveBlock(int);
	void moveHalf();
	void turnLeft(int);
	void turnRight(int);
	void turnUTurn();

	int senseFrontWall();
	int senseLeftWall();
	int senseRightWall();
	int senseGoal();

private:
	HINSTANCE hDLL;
	const typedef void (CALLBACK* dllVoidVoid)(void); // void fun(void)
	const typedef void (CALLBACK* dllVoidInt)(short); // void fun(short)
	const typedef short (CALLBACK* dllIntVoid)(void); // short fun(void)
};
