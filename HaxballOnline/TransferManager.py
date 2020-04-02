import threading
import socket
import time

class TransferManager( object ):
    def __init__(self, nickname):
        self.nickname = nickname
        self.number = -1
        self.readyToPlay = False
        self.s = 0

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


    def communicate(self):
        # keep sending and receiving readyToPlay to the server to keep connection
        while(True):
            readySignal = str(self.readyToPlay) + chr(0)
            self.s.sendall(bytes(readySignal, encoding='utf-8'))
            readySignal = self.s.recv(16)
            time.sleep(0.2)

