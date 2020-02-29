import pygame
import math
from Ball import Ball
from CirclePhysical import CirclePhysical

class Player(CirclePhysical):
    """Object containing player's mathods"""

    def __init__(self, game, px, py, number, weight):
        super().__init__(game, px, py, number, weight, 15, (255,0,0))

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
                    self.p -= overlap * (circ.weight/self.weight) * (self.p - circ.p)/dist
                    circ.p += overlap * (self.weight/circ.weight) * (self.p - circ.p)/dist

                    #if circ is a ball, transfer velocity
                    if isinstance(circ, Ball):

                        # zderzenie gracza z piłką to zderzenie sprężyste dynamiczne (następuje zmiana prędkości obu obiektów)
                        # poniżej implementacja wzoru w internetu

                        # normal vector
                        n = pygame.math.Vector2(0,0)
                        n = (self.p - circ.p)/dist

                        # tangent
                        t = pygame.math.Vector2(-n.y, n.x)

                        # dot product tangent
                        dpTan1 = self.v.x * t.x + self.v.y * t.y
                        dpTan2 = circ.v.x * t.x + circ.v.y * t.y

                        # dot product normal
                        dpNorm1 = self.v.x * n.x + self.v.y * n.y
                        dpNorm2 = circ.v.x * n.x + circ.v.y * n.y

                        # momentum
                        m1 = (dpNorm1 * (self.weight - circ.weight) + 2 * circ.weight * dpNorm2) / (self.weight + circ.weight)
                        m2 = (dpNorm2 * (circ.weight - self.weight) + 2 * self.weight * dpNorm1) / (self.weight + circ.weight)

                        # final velocity
                        self.v = (t * dpTan1 + n * m1) * self.ball_control
                        circ.v = (t * dpTan2 + n * m2) * self.ball_control

    def kick(self, pos):
        # check if ball is in hitbox range
        ball = self.game.members[0]
        dist = (self.p.x - ball.p.x)**2 + (ball.p.y - self.p.y)**2
        if dist <= (self.hitbox + ball.hitbox)**2:
            # kick ball to given pos
            ball.v = (pos - ball.p).normalize() * (pos - ball.p).length()/12

    def mode_ball_control(self):
        # turn down ball_control to reduce the bounce
        self.ball_control = 0.1
        self.v_max = 2/self.weight

    def mode_normal(self):
        # bring back normal mode
        self.ball_control = 1.0
        self.v_max = 6/self.weight




