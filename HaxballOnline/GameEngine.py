import pygame
import pygame.gfxdraw
from math import ceil
from CirclePhysical import CirclePhysical
from Goal import Goal

from Collision import Collision


class GameEngine( object ):
    #object containing Game's data

    screen_w = 1100
    screen_h = int(screen_w / 1.57)

    pitch_w = int(screen_w * 0.8)
    pitch_h = int(pitch_w / 1.57)

    back_color = (164, 143, 91)
    pitch_color_1 = (113, 152, 63)
    pitch_color_2 = (134, 185, 80)

    border_width = 2
    border_color = (174,202,137)

    fps = 60
    test_mode = False

    wall_bounce = 1.0

    sector_size = 50
    
    balls = []       # list containing balls
    players = []    # list containing players

    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((self.screen_w, self.screen_h))
        self.fps_clock = pygame.time.Clock()
        self.timer = 0

        # 2D array containing arrays, to store object in the secotrs and optimise collisions
        self.sectors = [ [ [] for j in range(ceil(self.screen_h / self.sector_size))] for i in range(ceil(self.screen_w / self.sector_size))] 

        # create goals
        screen_margin = (self.screen_h - self.pitch_h) / 2
        self.goal_left = Goal(self, self.pitch_color_1, (self.screen_w - self.pitch_w) / 2, screen_margin + self.pitch_h * 6 / 16, screen_margin + self.pitch_h * 10 / 16, 50, -1)
        self.goal_right = Goal(self, self.pitch_color_2, self.pitch_w + (self.screen_w - self.pitch_w) / 2, screen_margin + self.pitch_h * 6 / 16, screen_margin + self.pitch_h * 10 / 16, 50, 0)


    def draw_background(self):
        # draw backgroud
        self.screen.fill(self.back_color)

        
        # draw pitch border
        pygame.draw.rect(self.screen, self.border_color, \
            ((self.screen_w - self.pitch_w) / 2 - self.border_width, \
            (self.screen_h - self.pitch_h) / 2 - self.border_width, \
            self.pitch_w + self.border_width * 2 - 1, \
            self.pitch_h + self.border_width * 2 - 1), \
            self.border_width)

        # draw pitch stripes
        for i in range(0, 10):
            if i % 2:
                pygame.draw.rect(self.screen, self.pitch_color_1, \
                   ((self.screen_w - self.pitch_w) / 2 + i * self.pitch_w / 10, \
                   (self.screen_h - self.pitch_h) / 2, self.pitch_w / 10, \
                   self.pitch_h))
            else:
                pygame.draw.rect(self.screen, self.pitch_color_2, \
                    ((self.screen_w - self.pitch_w) / 2 + i * self.pitch_w / 10, \
                    (self.screen_h - self.pitch_h) / 2, self.pitch_w / 10, \
                    self.pitch_h))
        
        ### draw pitch lines

        # middle line
        pygame.draw.rect(self.screen, self.border_color, \
            ((self.screen_w / 2) - self.border_width + 1, \
            (self.screen_h - self.pitch_h) / 2, \
            self.border_width * 2, self.pitch_h))

        # circle
        pygame.gfxdraw.aacircle(self.screen, \
           int(self.screen_w / 2), \
           int(self.screen_h / 2), \
           int(self.pitch_w / 8), self.border_color)

        pygame.gfxdraw.aacircle(self.screen, \
           int(self.screen_w / 2), \
           int(self.screen_h / 2), \
           int(self.pitch_w / 8 - 1), self.border_color)

        pygame.gfxdraw.aacircle(self.screen, \
           int(self.screen_w / 2), \
           int(self.screen_h / 2), \
           int(self.pitch_w / 8 - 2), self.border_color)

        # dot
        pygame.gfxdraw.filled_circle(self.screen, \
           int(self.screen_w / 2), \
           int(self.screen_h / 2), \
           self.border_width * 3, self.border_color)

        

        # draw goals and borders
        pygame.draw.rect(self.screen, self.border_color, (self.goal_left.get_px() - self.border_width,self.goal_left.get_py() - self.border_width, self.goal_left.get_width() + self.border_width - 1, self.goal_left.get_height() + self.border_width * 2 - 1), self.border_width)
        pygame.draw.rect(self.screen, self.goal_left.color, (self.goal_left.get_px(), self.goal_left.get_py(), self.goal_left.get_width(), self.goal_left.get_height()))
        
        pygame.draw.rect(self.screen, self.border_color, (self.goal_right.get_px(),self.goal_right.get_py() - self.border_width, self.goal_right.get_width() + self.border_width - 1, self.goal_right.get_height() + self.border_width * 2 - 1), self.border_width)
        pygame.draw.rect(self.screen, self.goal_right.color, (self.goal_right.get_px(), self.goal_right.get_py(), self.goal_right.get_width(), self.goal_right.get_height()))

        # draw posts
        pygame.gfxdraw.filled_circle(self.screen, int(self.goal_left.post_up.p.x), int(self.goal_left.post_up.p.y), self.goal_left.post_up.size, self.goal_left.post_up.color)
        pygame.gfxdraw.aacircle(self.screen, int(self.goal_left.post_up.p.x), int(self.goal_left.post_up.p.y), self.goal_left.post_up.size, self.goal_left.post_up.color)
        pygame.gfxdraw.filled_circle(self.screen, int(self.goal_left.post_down.p.x), int(self.goal_left.post_down.p.y), self.goal_left.post_down.size, self.goal_left.post_down.color)
        pygame.gfxdraw.aacircle(self.screen, int(self.goal_left.post_down.p.x), int(self.goal_left.post_down.p.y), self.goal_left.post_down.size, self.goal_left.post_down.color)
        
        pygame.gfxdraw.filled_circle(self.screen, int(self.goal_right.post_up.p.x), int(self.goal_right.post_up.p.y), self.goal_right.post_up.size, self.goal_right.post_up.color)
        pygame.gfxdraw.aacircle(self.screen, int(self.goal_right.post_up.p.x), int(self.goal_right.post_up.p.y), self.goal_right.post_up.size, self.goal_right.post_up.color)
        pygame.gfxdraw.filled_circle(self.screen, int(self.goal_right.post_down.p.x), int(self.goal_right.post_down.p.y), self.goal_right.post_down.size, self.goal_right.post_down.color)
        pygame.gfxdraw.aacircle(self.screen, int(self.goal_right.post_down.p.x), int(self.goal_right.post_down.p.y), self.goal_right.post_down.size, self.goal_right.post_down.color)
    
        
    def clock_tick(self):
        return self.fps_clock.tick(self.fps)

    def new_player(self, player):
        self.players.append(player)

    def new_ball(self, ball):
        self.balls.append(ball)

    def redraw(self):
        # draw static background
        self.draw_background()

        # update positions and redraw players
        self.update()

        # redraw screen
        pygame.display.update()

        # dt is time since last tick
        self.timer += self.clock_tick()

    def update(self):

        # update positions
        for obj in self.players:
            obj.update()
        for obj in self.balls:
            obj.update()

        ### if test mode is on, draw additional markers on screen
        if self.test_mode:

            # draw sectors around ball and players
            for obj in self.players:
                sector_num = int(obj.size * 4 / self.sector_size)
                for i in range(int(obj.p.x / self.sector_size) - sector_num, int(obj.p.x / self.sector_size) + sector_num + 1):
                    for j in range(int(obj.p.y / self.sector_size) - sector_num, int(obj.p.y / self.sector_size) + sector_num + 1):
                        pygame.draw.rect(self.screen, (0,255 - ((i + j) % 2) * 50, 0), (i * self.sector_size, j * self.sector_size, int(self.sector_size), int(self.sector_size)))
            for obj in self.balls:
                sector_num = int(obj.size * 4 / self.sector_size)
                for i in range(int(obj.p.x / self.sector_size) - sector_num, int(obj.p.x / self.sector_size) + sector_num + 1):
                    for j in range(int(obj.p.y / self.sector_size) - sector_num, int(obj.p.y / self.sector_size) + sector_num + 1):
                        pygame.draw.rect(self.screen, (0,255 - ((i + j) % 2) * 50, 0), (int(obj.p.x / self.sector_size) * self.sector_size, int(obj.p.y / self.sector_size) * self.sector_size, int(self.sector_size), int(self.sector_size)))
            
            # draw hitboxes
            for obj in self.players:
                pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.hitbox, (0,0,255))
            for obj in self.balls:
                pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.hitbox, (0,0,255))

        # check collisions and redraw all players
        for obj in self.players:
            Collision.collide(obj)
            pygame.gfxdraw.filled_circle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, obj.color)
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, (0,0,0))
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size - 1, (0,0,0))

        # check collisions with posts
        # TODO: move it to goal verification function
        Collision.collide(self.goal_left.post_up)
        Collision.collide(self.goal_left.post_down)
        Collision.collide(self.goal_right.post_up)
        Collision.collide(self.goal_right.post_down)

        # check collisions and redraw balls
        for obj in self.balls:
            pygame.gfxdraw.filled_circle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, obj.color)
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, (0,0,0))
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size - 1, (0,0,0))

    # fix objects position, to prevent walls collisions
    def walls_collision(self, obj):
        Collision.walls_collision(obj, self)