import threading
import socket
import time
import GameEngine
class TransferManager( object ):
    def __init__(self, nickname):
        self.nickname = nickname
        self.number = -1
        self.readyToPlay = False
        self.s = 0
        self.game = 0
    def initConnection(self):

        # initialize socket
        HOST = '127.0.0.1'
        PORT = 8080 
        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # send nickname to server and receive number
            self.s.connect((HOST, PORT))
        
            nickname = self.nickname + chr(0)
            self.s.sendall(bytes(nickname, encoding='utf-8'))
        
            data = self.s.recv(16)
            self.number = data[0]
            print(str(self.nickname) + str(self.number))
        except:
            return -1

        # hold connection in new thread
        communicator = threading.Thread(target=self.communicate, args =())
        communicator.deamon = True
        communicator.start()

        return 1

    def addGame(self, game:GameEngine):
        self.game=game


    def communicate(self):
        # keep sending and receiving readyToPlay to the server to keep connection
        while(True):
            readySignal = str(self.readyToPlay) + chr(0)
            self.s.sendall(bytes(readySignal, encoding='utf-8'))
            size=8
            dataSize = 5*size + 2*size +size*len(self.game.team_left.players)*5 + size*len(self.game.team_right.players)*5
            data = self.s.recv(dataSize)
            #TODO change bytes to double from C, convert to python double and serialzie
            time.sleep(0.2)

