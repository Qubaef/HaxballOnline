import pygame
from CirclePhysical import CirclePhysical

class Ball(CirclePhysical):
    """
    Class defining ball
    Ball is always at first place of a game.members[] list
    """

    def __init__(self, game, px, py, number):
        super().__init__(game, px, py, number, 0.2, 10, (255,255,255))



