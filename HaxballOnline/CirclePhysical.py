import pygame
from abc import ABC

class CirclePhysical(ABC):
    """description of class"""

    def __init__(self, game, px, py, number, weight, size, color):
        self.game = game
        self.number = number
        self.weight = weight
        self.size = size                # size used for drawing and collision detection
        self.hitbox = size + 7          # hitbox used for kicking the ball
        self.v_max = 6/self.weight
        self.friction = self.weight * 0.2
        self.color = color
        self.p = pygame.math.Vector2(px,py)
        self.v = pygame.math.Vector2(0,0)
        self.a = pygame.math.Vector2(0,0)
        self.ball_control = 1
        self.sector = (px)

    def velocity_add(self, velocity):
        self.v += velocity

    def update(self):
        # remove element from current sector
        if self in self.game.sectors[int(self.p.x/self.game.sector_size)][int(self.p.y/self.game.sector_size)]:
            self.game.sectors[int(self.p.x/self.game.sector_size)][int(self.p.y/self.game.sector_size)].remove(self)

        self.a = -self.v * self.friction
        self.v += self.a
        self.p.x += self.v.x
        self.p.y += self.v.y

        # check if velocity is not bigger than max allowed velocity
        if self.v.magnitude() > self.v_max:
            self.v = self.v.normalize() * self.v_max

        # fix object's position whith wall collsion detection
        self.game.check_collision(self)

        # move element to right sector
        self.game.sectors[int(self.p.x/self.game.sector_size)][int(self.p.y/self.game.sector_size)].append(self)


    def set_move(self, v, p):
        # set given veloctity and position
        self.p.x = p[0]
        self.p.y = p[1]

        self.v.x = v[0]
        self.v.y = v[1]

    def get_nearby(self):
        # return list of objects located in nearby sectors

        # get number of nearby sectors (depends of object size)
        sector_num = int(self.size*4/self.game.sector_size)

        # iterate through sectors and gather all circles
        objects = []
        for i in range(int(self.p.x/self.game.sector_size) - sector_num, int(self.p.x/self.game.sector_size) + sector_num + 1):
            for j in range(int(self.p.y/self.game.sector_size) - sector_num, int(self.p.y/self.game.sector_size) + sector_num + 1):
                if i >= 0 and j >= 0 and i < len(self.game.sectors) and j < len(self.game.sectors[0]):
                    objects += self.game.sectors[i][j]

        return objects

                    





