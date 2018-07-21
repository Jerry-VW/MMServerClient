#MMClient MMAgent module
#Enable remote control of your virtual micromouse by network communication.
#Copyright(C) 2018  WENQIAN WANG

#This program is free software : you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.If not, see <http://www.gnu.org/licenses/>.

# Python 3.7

# COMMAND           : DESCRIPTION
# ----------------------------------------
# connect           : connect to MMServer
# disconnect        : disconnect from MMServer
# shutdownServer    : shutdown remote server and exit
# exit              : exit MMClient without shutting down server
# reset             : reset mouse to start position
# move              : move one block forward
# half              : move half block forward
# back              : move one block backward
# left              : turn left by 90 degrees
# right             : turn right by 90 degrees
# uTurn             : turn 180 degrees
# senseLeft         : sense if there is a wall on the left (1:yes/0:no)
# senseFront        : sense if there is a wall in front (1:yes/0:no)
# senseRight        : sense if there is a wall on the right (1:yes/0:no)
# senseGoal         : sense if mouse has arrived goal (1:yes/0:no)

from socket import socket
from socket import AF_INET, SOCK_STREAM

class Agent(object):

    def __init__(self, host, port):
        super(Agent, self).__init__()
        self.client = socket(AF_INET, SOCK_STREAM)
        self.isExitPending = False
        self.isConnected = False
        self.bufferSize = 512
        self.host = host
        self.port = port
        print('Ready to connect to {h} @ {p}'.format(h=host, p=str(port)))

    def connect(self) -> int:
        if not self.isConnected:
            self.client = socket(AF_INET, SOCK_STREAM)
            self.client.connect((self.host, self.port))
            self.client.sendall('{:\0^10}'.format('\0').encode())
            self.client.recv(self.bufferSize)
        self.isConnected = True
        return 0

    def disconnect(self) -> int:
        if self.isConnected:
            self.client.sendall('{:\0^10}'.format('\0').encode())
            self.client.recv(self.bufferSize)
        self.client.close()
        self.isConnected = False
        return 0

    def shutdownServer(self) -> int:
        if self.isConnected:
            self.client.sendall('shutdownServer'.encode())
            self.client.recv(self.bufferSize)
        self.isConnected = False
        return 0

    def exit(self) -> int:
        self.disconnect()
        self.isExitPending = True
        return 0

    def reset(self) -> int:
        if self.isConnected:
            self.client.sendall('reset'.encode())
            self.client.recv(self.bufferSize)
            return 0
        return -1

    def move(self) -> int:
        if self.isConnected:
            self.client.sendall('move'.encode())
            self.client.recv(self.bufferSize)
            return 0
        return -1

    def half(self) -> int:
        if self.isConnected:
            self.client.sendall('half'.encode())
            self.client.recv(self.bufferSize)
            return 0
        return -1

    def back(self) -> int:
        if self.isConnected:
            self.client.sendall('back'.encode())
            self.client.recv(self.bufferSize)
            return 0
        return -1

    def left(self) -> int:
        if self.isConnected:
            self.client.sendall('left'.encode())
            self.client.recv(self.bufferSize)
            return 0
        return -1

    def right(self) -> int:
        if self.isConnected:
            self.client.sendall('right'.encode())
            self.client.recv(self.bufferSize)
            return 0
        return -1

    def uTurn(self) -> int:
        if self.isConnected:
            self.client.sendall('uTurn'.encode())
            self.client.recv(self.bufferSize)
            return 0
        return -1

    def senseLeft(self) -> int:
        if self.isConnected:
            self.client.sendall('senseLeft'.encode())
            return int(self.client.recv(self.bufferSize).decode())
        return -1

    def senseFront(self) -> int:
        if self.isConnected:
            self.client.sendall('senseFront'.encode())
            return int(self.client.recv(self.bufferSize).decode())
        return -1

    def senseRight(self) -> int:
        if self.isConnected:
            self.client.sendall('senseRight'.encode())
            return int(self.client.recv(self.bufferSize).decode())
        return -1

    def senseGoal(self) -> int:
        if self.isConnected:
            self.client.sendall('senseGoal'.encode())
            return int(self.client.recv(self.bufferSize).decode())
        return -1
