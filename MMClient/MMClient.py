
#MMClient
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

# Example of how to use MMAgent module.
# step1: Execute the maze mms.exe
# step2: Start the server MicroMouseServer.exe
# step3: Run this .py file
# Usable method:
# METHOD            : DESCRIPTION
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

import MMAgent # import MMAgent module
agent = MMAgent.Agent('localhost', 23333) # instantiate a agent class
while not agent.isExitPending:
    command = input('Command:')
    if command == 'connect':
        agent.connect() # give command to micromouse by calling agent's methods
        continue
    if command == 'disconnect':
        agent.disconnect()
        continue
    if command == 'shutdownServer':
        agent.shutdownServer()
        continue
    if command == 'exit':
        agent.exit()
        continue
    if command == 'reset':
        agent.reset()
        continue
    if command == 'move':
        agent.move()
        continue
    if command == 'half':
        agent.half()
        continue
    if command == 'back':
        agent.back()
        continue
    if command == 'left':
        agent.left()
        continue
    if command == 'right':
        agent.right()
        continue
    if command == 'uTurn':
        agent.uTurn()
        continue
    if command == 'sLeft':
        print(agent.senseLeft())
        continue
    if command == 'sFront':
        print(agent.senseFront())
        continue
    if command == 'sRight':
        print(agent.senseRight())
        continue
    if command == 'sGoal':
        print(agent.senseGoal())
        continue
