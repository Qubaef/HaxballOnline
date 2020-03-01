import pygame
import math
from Ball import Ball
from CirclePhysical import CirclePhysical

class Player(CirclePhysical):
    """Object containing player's mathods"""

    def __init__(self, game, px, py, number, color):
        super().__init__(game, px, py, number, 1, 15, color)

    # check if circle collides with any other circle in nearest sectors
    def collide(self):
        nearby = super().get_nearby()

        for circ in nearby:
            if circ != self:
                dist = (self.p.x - circ.p.x)**2 + (circ.p.y - self.p.y)**2
                if dist <= (self.size + circ.size)**2:
                    dist = math.sqrt(dist)

                    if dist == 0:
                        dist = 1

                    overlap = (dist - self.size - circ.size)/2

                    super().from_sector_remove()
                    self.p -= overlap * (circ.weight/self.weight) * (self.p - circ.p)/dist
                    self.game.check_collision(self)
                    super().to_sector_add()

                    circ.from_sector_remove()
                    circ.p += overlap * (self.weight/circ.weight) * (self.p - circ.p)/dist
                    circ.game.check_collision(circ)
                    circ.to_sector_add()

                    #if circ is a ball, transfer velocity
                    #if isinstance(circ, Ball):

                    # zderzenie gracza z piłką to zderzenie sprężyste dynamiczne (następuje zmiana prędkości obu obiektów)
                    # poniżej implementacja wzoru z wikipedidd

                    # normal vector
                    self.v,circ.v = CirclePhysical.collision_calculator(self.v,circ.v, self.weight , circ.weight,self.p,circ.p)
                    circ.v = circ.v * self.ball_control
                    # check if ball velocity is not bigger than max allowed velocity
                    if circ.v.magnitude() > circ.v_max:
                        circ.v = circ.v.normalize() * circ.v_max

    def kick(self, pos):
        # check if ball is in hitbox range
        for ball in self.game.balls:
            dist = (self.p.x - ball.p.x)**2 + (ball.p.y - self.p.y)**2
            if dist <= (self.hitbox + ball.hitbox)**2:
                # kick ball to given pos
                ball.v = (pos - ball.p).normalize() * (pos - ball.p).length()/12

    def mode_ball_control(self):
        # turn down ball_control to reduce the bounce
        self.ball_control = 0.1
        self.v_max = 2 / math.pow(self.weight, 2/3)

    def mode_normal(self):
        # bring back normal mode
        self.ball_control = 1.0
        self.v_max = 6 / math.pow(self.weight, 2/3)
