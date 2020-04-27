import threading
import struct
import socket
import time
import GameEngine


class TransferManager( object ):
    def __init__(self, nickname, ip):
        self.client_nickname = nickname
        self.client_number = -1
        self.ready_to_play = False
        self.s = 0
        self.ip = ip
        self.game_running = False

        # data to init game
        self.init_pack_recived = False
        self.players_number = 0
        self.players_numbers = []
        self.players_nicknames = []
        self.players_teams = []

        self.game_pack_recived = False
        self.game_pack = 0

        # data to send
        self.command = 64
        self.mouse_x = 0
        self.mouse_y = 0

    def initConnection(self):

        # initialize socket
        
        HOST = self.ip
        PORT = 8080 
        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # send nickname to server and receive number
            self.s.connect((HOST, PORT))
            
            self.s.setblocking(True)

            print('Established connection ')

            nickname = self.client_nickname + chr(0)
            self.s.sendall(bytes(nickname, encoding='utf-8'))
            
            dataSize = 8
            data = self.s.recv(dataSize)
            self.client_number = int(struct.unpack('d',data)[0])
            print('Nickname: ' + str(self.client_nickname) + ' Number: ' + str(self.client_number))
        except:
            return -1

        # hold connection in new thread
        communicator = threading.Thread(target=self.communicate, args =())
        communicator.deamon = True
        communicator.start()

        return 1


    def communicate(self):
        # keep sending and receiving readyToPlay to the server to keep connection
        data_size = 256
        sizeof_int = 4
        sizeof_double = 8 


        # send and receive readyToPlay, until init pack was recived
        while(True):
            while(True):
                ready_flag = struct.pack('?', self.ready_to_play)
                self.s.sendall(ready_flag)

                data = self.s.recv(data_size)
                if(len(data) != 1):
                    # init pack received
                    self.ready_to_play = False  
                    break


            # get init's pack info
            data_unpacked = struct.unpack('i',data)
            self.players_number = data_unpacked[0]

            # get init data of players
            for i in range(self.players_number):
                self.s.sendall(data)
                data = self.s.recv(data_size)

                # unpack team and player's number
                data_unpacked = struct.unpack_from('ii',data)

                self.players_teams.append(data_unpacked[0])
                self.players_numbers.append(data_unpacked[1])

                # trim data and get player's nickname
                data = data[2 * sizeof_int:]
                sizeof_nickname = len(data)
                data_unpacked = struct.unpack_from(str(sizeof_nickname) + 's',data)

                # convert bytes to string, dropping last character ('\0' char is not needed in python)
                self.players_nicknames.append(data_unpacked[0].decode("utf-8")[:-1])


            # give info to GameController that data was sent and game needs to be initalized
            self.init_pack_recived = True


             # send and receive readyToPlay, until game pack was recived
            while(True):
                ready_flag = struct.pack('?', self.ready_to_play)
                self.s.sendall(ready_flag)

                data = self.s.recv(data_size)
                if(len(data) != 1):
                    # game pack received
                    self.ready_to_play = False  
                    break


            # receive and analyse game pack, send client's input
            while(True):
                # analyse game pack
                game_data_size = 5 * sizeof_double + 2 * sizeof_double + sizeof_double * self.players_number * 5
                data_unpacked = struct.unpack(self.unpack_format(game_data_size), data)
            
                self.game_pack = data_unpacked
                self.game_pack_recived = True

                # send client's input
            
                clientInput = bytes([self.command]) + struct.pack('dd', *[self.mouse_x, self.mouse_y])
                self.s.sendall(clientInput)

                # receive new game pack
                data = self.s.recv(game_data_size)
                if len(data) == 1:
                    self.resetTransferData()
                    break
   

    def unpack_format(self, data_size):
        format = ""
        for i in range(int(data_size / 8)):
            format += 'd'
        return format


    def resetTransferData(self):
        self.init_pack_recived = False
        self.game_pack_recived = False
        self.game_running = False

        # data to init game
        self.init_pack_recived = False
        self.players_number = 0
        self.players_numbers = []
        self.players_nicknames = []
        self.players_teams = []

        self.game_pack_recived = False
        self.game_pack = 0

        # data to send
        self.command = 64
        self.mouse_x = 0
        self.mouse_y = 0
