import pygame
import math
from abc import ABC

class CirclePhysical(ABC):

    def __init__(self, game, px, py, number, weight, size, color):
        self.game = game
        self.number = number
        self.weight = weight
        self.size = size                # size used for drawing and collision detection
        self.hitbox = int(size * 3/2)          # hitbox used for kicking the ball
        self.v_max = 6 / math.pow(self.weight, 2/3)     # maximum velocity is non-linear, cause ball was be too fast
        self.friction = self.weight * 0.2
        self.color = color
        self.p = pygame.math.Vector2(px,py)
        self.v = pygame.math.Vector2(0,0)
        self.a = pygame.math.Vector2(0,0)
        self.ball_control = 1
        self.sector = (px)

    # methods

    def velocity_add(self, velocity):
        self.v += velocity

    def update(self):

        self.from_sector_remove()

        self.a = -self.v * self.friction
        self.v += self.a
        self.p += self.v

        # check if velocity is not bigger than max allowed velocity
        if self.v.magnitude() > self.v_max:
            self.v = self.v.normalize() * self.v_max

        # fix object's position whith wall collsion detection
        self.game.check_collision(self)

        self.to_sector_add()


    def from_sector_remove(self):
        # remove element from currently occupied sector
        if self in self.game.sectors[int(self.p.x/self.game.sector_size)][int(self.p.y/self.game.sector_size)]:
            self.game.sectors[int(self.p.x/self.game.sector_size)][int(self.p.y/self.game.sector_size)].remove(self)

    def to_sector_add(self):
        # add element to right sector
        self.game.sectors[int(self.p.x/self.game.sector_size)][int(self.p.y/self.game.sector_size)].append(self)


    def set_move(self, v, p):
        # set given veloctity and position
        # if given pos is less than 0, dont change it
        if p[0] >= 0 and p[1] >= 0:
            self.p.x = p[0]
            self.p.y = p[1]

        self.v.x = v[0]
        self.v.y = v[1]

    def set_p(self, px, py):
        # set p vector
        self.p.x = px
        self.p.y = py

    def get_nearby(self):
        # return list of objects located in nearby sectors

        # get number of nearby sectors (depends of object size)
        sector_num = int(self.size * 4 / self.game.sector_size)

        # iterate through sectors and gather all circles
        objects = []
        for i in range(int(self.p.x/self.game.sector_size) - sector_num, int(self.p.x/self.game.sector_size) + sector_num + 1):
            for j in range(int(self.p.y/self.game.sector_size) - sector_num, int(self.p.y/self.game.sector_size) + sector_num + 1):
                if i >= 0 and j >= 0 and i < len(self.game.sectors) and j < len(self.game.sectors[0]):
                    objects += self.game.sectors[i][j]

        return objects

    # check if circle collides with any other circle in nearest sectors
    def collide(self):
        nearby = self.get_nearby()

        for circ in nearby:
            if circ != self:
                dist = (self.p.x - circ.p.x)**2 + (circ.p.y - self.p.y)**2
                if dist <= (self.size + circ.size)**2:
                    dist = math.sqrt(dist)

                    overlap = (dist - self.size - circ.size)/2

                    self.from_sector_remove()
                    if dist != 0:
                        p_new = self.p - overlap * (circ.weight/self.weight) * (self.p - circ.p)/dist
                        self.set_p(p_new.x, p_new.y)
                    else:
                        p_new = self.p - overlap * (circ.weight/self.weight) * self.p.normalize()
                        self.set_p(p_new.x, p_new.y)
                    self.game.check_collision(self)
                    self.to_sector_add()

                    circ.from_sector_remove()
                    if dist != 0:
                        p_new = circ.p + overlap * (self.weight/circ.weight) * (self.p - circ.p)/dist
                        circ.set_p(p_new.x, p_new.y)
                    else:
                        p_new = circ.p + overlap * (self.weight/circ.weight) * self.p.normalize()
                        circ.set_p(p_new.x, p_new.y)

                    circ.game.check_collision(circ)
                    circ.to_sector_add()

                    # zderzenie gracza z piłką to zderzenie sprężyste dynamiczne (następuje zmiana prędkości obu obiektów)
                    # poniżej implementacja wzoru z wikipedidd

                    # normal vector
                    self.v,circ.v = self.collision_calculator(self.v,circ.v, self.weight , circ.weight,self.p,circ.p)
                    circ.v = circ.v * self.ball_control
                    # check if ball velocity is not bigger than max allowed velocity
                    if circ.v.magnitude() > circ.v_max:
                        circ.v = circ.v.normalize() * circ.v_max

    @staticmethod
    def collision_calculator(v1, v2, m1, m2, x1, x2) -> pygame.math.Vector2:
        mass = 2 * m1 / (m1 + m2)
        v11= v1 - (mass * (v1 - v2).dot(x1 - x2) / pow((x1 - x2).length(), 2)) * (x1 - x2)
        v22= v2 - (mass * (v2 - v1).dot(x2 - x1) / pow((x2 - x1).length(), 2)) * (x2 - x1)
        return v11,v22
                    





