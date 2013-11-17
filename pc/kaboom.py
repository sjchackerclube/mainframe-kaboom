#-------------------------------------------------------------------------------
# Name:         kaboom
# Purpose:      Remote control for the mainframe-kaboom circuit
#
# Author:      aporto
#
# Created:     17/11/2013
# Copyright:   (c) aporto 2013
# Licence:     GPLv3
#-------------------------------------------------------------------------------

import threading, serial

class KaboomPlayer():
    def __init__(self):
        self.status = 0
        self.wiring = [0, 0, 0, 0, 0, 0]
        self.step = 0

class Kaboom(threading.Thread):
    def __init__(self):
        #import serial
        from time import sleep

        self.PLAYER_STATUS_WAITING = 0
        self.PLAYER_STATUS_PLAYING = 1
        self.PLAYER_STATUS_LOST = 2
        self.PLAYER_STATUS_WON = 3
        self.PLAYER_STATUS_NOT_READY = 4

        self.__sleep = sleep
        self.__serialPort = serial.Serial(timeout = 0.1)
        self.__connected = False;
        self.__end = 0
        self.__sequence = '-1-2-4'

        self.__receiving = 0    # receiving_none
        self.__statusString = ''

        self.players = []
        for i in range(6):
            self.players.append(KaboomPlayer())

        threading.Thread.__init__(self)

    def __del__(self):
        self.disconnect()

    def connect(self, port):
        if self.__connected:
            return False
        try:
            self.__serialPort.port = port
            self.__serialPort.open()
            self.start()
            self.__connected = True
        except:
            self.__connected = False
        return self.__connected

    def disconnect(self):
        if self.__connected:
            self.__close()
            self.__serialPort.close()
            self.__connected = False

    def start_game(self, sequence="-1-2-4"):
        if self.__connected:
            self.__sequence = sequence
            #self.__serialPort.write("*A*")  # set sequence
            self.__serialPort.write("*B*" + sequence+ "|\n")  # start new game

    def get_status(self):
        if self.__connected:
            for player in self.players:
                player.status = 0
                #print "zera"
                player.wiring = [0, 0, 0, 0, 0, 0]
                player.step = 0

            timeout = 10
            #print "status"
            self.__serialPort.write("*C*\n")  # get status
            self.__sleep(0.5)

            while timeout > 0:
                self.__sleep(0.01)
                timeout = timeout - 1
                if self.__receiving == 2:    # received status string
                    break
            if self.__receiving == 2:    # received status string
                self.__receiving = 0
                return True
            else:
                self.__receiving = 0
                return False
        else:
            return False

    def __process_status_string(self):
        #print "get status"
        #print self.__statusString
        self.__statusString = self.__statusString.lstrip('*')
        strPlayers = self.__statusString.split('|')
        self.__statusString = ''

        #print len(strPlayers)
        if len(strPlayers) != 7:
            return

        i = 0
        for player in self.players:
            #print i, "=", strPlayers[i]
            txt = strPlayers[i].split(':')
            player.step = int("0x" + txt[0], 16)
            player.status = int("0x" + txt[1], 16)
            wiring = int("0x" + txt[2], 16)
            #print " "
            #print i,"=", wiring, "=", txt[2],
            for bit in range(6):
                #print bit, "(", (wiring & (1 << bit)) > 0, ") ",
                if wiring & (1 << bit) > 0:
                    player.wiring[bit] = 1
                else:
                    player.wiring[bit] = 0
            i = i + 1

    def __close(self):
        #print "---> Closing"
        self.__end = 1
        while (self.__end == 1):
            self.__sleep(0.1)
        #print "---> Closed"

    def run(self):
        while(self.__end == 0):
            #print "Teste"
            if self.__connected == False:
                self.__sleep(0.1)
                #print "--"
                continue
            c = self.__serialPort.read()
            if c == '':
                #print "++"
                #self.__sleep(0.1)
                continue
            #print c,
            if c == '*':
                self.__receiving = 1    # receiving status string
                #print "Start status"
                self.__statusString = ""
            elif self.__receiving == 1:
                self.__statusString += c
                #print ":",
                if len(self.__statusString) == 55:
                    #print "end status"
                    self.__receiving = 0
                    self.__process_status_string()
                    self.__receiving = 2    # received status string
        #print "--->Closed"
        self.__end = 2

if __name__ == '__main__':
    from time import sleep

    k = Kaboom()
    print k.connect(13)
    sleep(3) # Give some time to Arduino start the serial port

    print k.get_status()
    for item in k.players:
        print item.status,", ", item.wiring, ", ", item.step
    #sleep(2)
    #k.disconnect()
    print "fim"

