
 #  The following class describes a single player during the game. 
 #  This class is a derived class and inherits from the "CirclePhysical" base class.
 
import pygame
import math
from Ball import Ball
from CirclePhysical import CirclePhysical

class Player( CirclePhysical ):
    # Object containing player's mathods

    def __init__(self, game, px, py, number, color, nickname):
        super().__init__(game, px, py, number, 1, 15, color)
        self.nickname = nickname

    def kick(self, pos):
        # check if ball is in hitbox range
        for ball in self.game.balls:
            dist = (self.p.x - ball.p.x) ** 2 + (ball.p.y - self.p.y) ** 2
            if dist <= (self.hitbox + ball.hitbox) ** 2:
                # kick ball to given pos
                ball.v = (pos - ball.p).normalize() * (pos - ball.p).length() / 12

    def mode_ball_control(self):
        # turn down ball_control to reduce the bounce
        self.ball_control = 0.1
        self.v_max = 2 / math.pow(self.weight, 2 / 3)

    def mode_normal(self):
        # bring back normal mode
        self.ball_control = 1.0
        self.v_max = 6 / math.pow(self.weight, 2 / 3)

