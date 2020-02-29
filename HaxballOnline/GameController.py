import pygame
import sys
import math
import random

from pygame.locals import *

from Player import Player
from GameEngine import GameEngine
from Ball import Ball

game = GameEngine()
ball = Ball(game, 500, 300, 0)
player = Player(game, 400, 300, 1, 1.0)

game.new_ball(ball)
game.new_member(player)

# create static bots (buggy sometimes)
for i in range(2,20):
   game.new_member(Player(game, game.pitch_w * random.uniform(0,1), game.pitch_h * random.uniform(0,1), i + 2, 1))

done = False

# main loop of the game
while not done:
    # draw static background
    game.draw_background()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_t:
                if game.test_mode:
                    game.test_mode = False
                else:
                    game.test_mode = True
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_SPACE:
                player.mode_normal()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            p = pygame.mouse.get_pos()
            player.kick(p)

        if done != True:
            player_move = pygame.math.Vector2(0,0)
            input = pygame.key.get_pressed()
            if input[K_w]:
                player_move += (0,-1)
            if input[K_s]:
                player_move += (0, 1)
            if input[K_d]:
                player_move += (1,0)
            if input[K_a]:
                player_move += (-1,0)
            if input[K_r]:
                # set ball v and p values
                ball.set_move((15,15), (0,0))
            if input[K_SPACE]:
                # turn on better ball control
                player.mode_ball_control()
            if input[K_ESCAPE]:
                done = True
    
    # move player depending on keyboard input
    player.velocity_add(player_move)

    # update positions and redraw members
    game.update()

    # redraw screen
    pygame.display.update()
    game.clock_tick()

