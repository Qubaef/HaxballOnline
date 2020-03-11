import pygame
import sys
import math
import random

from pygame.locals import *
from Post import Post
from Player import Player
from GameEngine import GameEngine
from Ball import Ball

game = GameEngine()
screenw_add = (game.screen_w-game.pitch_w)/2
screenh_add = (game.screen_h-game.pitch_h)/2
ball = Ball(game, 500, 300, 0)
player = Player(game, 400, 300, 1, (0, 0, 255))

game.new_ball(ball)
game.new_player(player)
# create static bots
bots = []
bots_number = 5
for i in range(0,bots_number):
   bots.append(Player(game, game.screen_w * random.uniform(0,1), game.screen_h * random.uniform(0,1), i + 2, (255, 0, 0)))
   game.new_player(bots[i])

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

    if player_move.length() > 0:
        # make player move with equal speed in all directions
        player_move = player_move.normalize()

    player.velocity_add(player_move)

    #for i in range (0,bots_number):
    #    if game.timer > 100 * i:
    #        dir = (ball.p - bots[i].p).normalize()
    #        bots[i].set_move((bots[i].v_max * random.uniform(0,1) * dir.x,(bots[i].v_max * random.uniform(0,1) * dir.y)), (-1, -1))
    #
    #if game.timer > 100 * bots_number:
    #    game.timer = 0

    game.redraw()

