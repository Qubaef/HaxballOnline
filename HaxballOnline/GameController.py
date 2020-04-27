import pygame
import sys
import math

from pygame.locals import *
from Post import Post
from Player import Player
from GameEngine import GameEngine
from Ball import Ball
from TransferManager import TransferManager


 # get user's input and return it as data ready to send to the server
def manageInputs():

    localCommand = 0
    command = 64
    mouse_x = 0
    mouse_y = 0

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            localCommand = -1
        elif event.type == pygame.KEYDOWN and event.key == pygame.K_t:
            localCommand = 1
        elif event.type == pygame.MOUSEBUTTONDOWN:
            command += 2
            mouse_x = pygame.mouse.get_pos()[0]
            mouse_y = pygame.mouse.get_pos()[1]
    
    if localCommand != -1:
        player_move = pygame.math.Vector2(0,0)
        input = pygame.key.get_pressed()
        if input[K_w]:
            command += 16
        if input[K_s]:
            command += 32
        if input[K_d]:
            command += 8
        if input[K_a]:
            command += 4
        if input[K_SPACE]:
            command += 1
        if input[K_ESCAPE]:
            localCommand = -1
    
    return localCommand, command, mouse_x, mouse_y


##### START OF THE GAME
### Main Menu

# get user's nick
nickname = input('Enter nickname: ')
ip = input('Enter server\'s ip adress: ')

if ip == '':
    ip = '127.0.0.1'

# init connection with the server
transfer_manager = TransferManager(nickname, ip)
if transfer_manager.initConnection() == -1:
    sys.exit()

done = False

while(not done):
     # ask user if he is ready to start the game
    input('Ready?: ')

    transfer_manager.ready_to_play = True

    # wait for init pack to be sent
    while(transfer_manager.init_pack_recived == False):
        continue

    # intialize new game based on data recived in init packs
    game = GameEngine()
    ball = Ball(game, 500, 300, 0)
    game.new_ball(ball)

    for i in range(len(transfer_manager.players_numbers)):
        game.new_player(Player(game, 0, 0, transfer_manager.players_numbers[i], (0, 0, 255), transfer_manager.players_nicknames[i]), transfer_manager.players_teams[i])

        # if player is client's player, set his border to different color
        if transfer_manager.players_numbers[i] == transfer_manager.client_number:
            game.players[i].border_color = (255,255,0)

    # set flag read
    transfer_manager.ready_to_play = True

    # main loop of the game
    while(True):

        localCommand, transfer_manager.command, transfer_manager.mouse_x, transfer_manager.mouse_y = manageInputs()
    
        # deserialize game
        if transfer_manager.game_pack_recived == True:
            game.deserialize(transfer_manager.game_pack)
            transfer_manager.game_pack_recived = False

        if localCommand == -1:
            # if localCommand was -1, exit the game
            done = True
            break
        elif localCommand == 1:
            # if localCommand was 1, switch display mode
            if game.test_mode:
                game.test_mode = False
            else:
                game.test_mode = True

        game.redraw()

        # if game was finished, break the loop
        if transfer_manager.game_running == False:
            break

    game.reset()
    del game