import pygame
from abc import ABC

class CirclePhysical(ABC):
    """description of class"""

    def __init__(self, game, px, py, number, weight, size, color):
        self.game = game
        self.number = number
        self.weight = weight
        self.size = size                # size used for drawing and collision detection
        self.hitbox = size + 6          # hitbox used for kicking the ball
        self.v_max = 6/self.weight
        self.friction = self.weight * 0.2
        self.color = color
        self.p = pygame.math.Vector2(px,py)
        self.v = pygame.math.Vector2(0,0)
        self.a = pygame.math.Vector2(0,0)
        self.ball_control = 1

    def velocity_add(self, velocity):
        self.v += velocity

    def update(self):
        # temp solution, cause this will be server side part

        self.a = -self.v * self.friction
        self.v += self.a
        self.p.x += self.v.x
        self.p.y += self.v.y

        # check if velocity is not bigger than max allowed velocity
        if self.v.magnitude() > self.v_max:
            self.v = self.v.normalize() * self.v_max

    def set_move(self, v, p):
        # set given veloctity and position
        self.p.x = p[0]
        self.p.y = p[1]

        self.v.x = v[0]
        self.v.y = v[1]

                    





