import math

import pygame
from Post import Post

class Collision(object):

    # check if circle collides with any other circle in nearest sectors
    @staticmethod
    def collide(circ1):

        nearby = circ1.get_nearby()

        for circ2 in nearby:
            if circ2 != circ1:
                dist = (circ1.p.x - circ2.p.x) ** 2 + (circ2.p.y - circ1.p.y) ** 2
                if dist <= (circ1.size + circ2.size) ** 2:
                    if isinstance(circ2, Post):
                        circ2.v = (0, 0)
                    dist = math.sqrt(dist)

                    overlap = (dist - circ1.size - circ2.size) / 2

                    circ1.from_sector_remove()
                    if dist != 0:
                        p_new = circ1.p - overlap * (circ2.weight / circ1.weight) * (circ1.p - circ2.p) / dist
                        circ1.set_p(p_new.x, p_new.y)
                    else:
                        p_new = circ1.p - overlap * (circ2.weight / circ1.weight) * circ1.p.normalize()
                        circ1.set_p(p_new.x, p_new.y)
                    circ1.game.check_collision(circ1)
                    circ1.to_sector_add()

                    circ2.from_sector_remove()
                    if dist != 0:
                        p_new = circ2.p + overlap * (circ1.weight / circ2.weight) * (circ1.p - circ2.p) / dist
                        circ2.set_p(p_new.x, p_new.y)
                    else:
                        p_new = circ2.p + overlap * (circ1.weight / circ2.weight) * circ1.p.normalize()
                        circ2.set_p(p_new.x, p_new.y)
                    if not isinstance(circ2,Post):
                        circ2.game.check_collision(circ2)
                    circ2.to_sector_add()

                    # zderzenie gracza z piłką to zderzenie sprężyste dynamiczne (następuje zmiana prędkości obu obiektów)
                    # poniżej implementacja wzoru z wikipedidd

                    # normal vector
                    circ1.v, circ2.v = Collision.collision_calculator(circ1.v, circ2.v, circ1.weight, circ2.weight, circ1.p, circ2.p)
                    circ2.v = circ2.v * circ1.ball_control
                    # check if ball velocity is not bigger than max allowed velocity
                    if circ2.v.magnitude() > circ2.v_max:
                        circ2.v = circ2.v.normalize() * circ2.v_max

    @staticmethod
    def collision_calculator(v1, v2, m1, m2, x1, x2) -> pygame.math.Vector2:
        mass = 2 * m1 / (m1 + m2)
        v11 = v1 - (mass * (v1 - v2).dot(x1 - x2) / pow((x1 - x2).length(), 2)) * (x1 - x2)
        v22 = v2 - (mass * (v2 - v1).dot(x2 - x1) / pow((x2 - x1).length(), 2)) * (x2 - x1)
        return v11, v22
