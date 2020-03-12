import pygame
import pygame.gfxdraw
from math import ceil
from CirclePhysical import CirclePhysical
from Goal import Goal

from Collision import Collision


class GameEngine(object):
    #object containing Game's data

    screen_w = 1100
    screen_h = int(screen_w / 1.57)

    pitch_w = int(screen_w * 0.8)
    pitch_h = int(pitch_w / 1.57)

    back_color = (164, 143, 91)
    pitch_color = (0, 160, 20)

    fps = 60
    test_mode = False

    wall_bounce = 1.0

    sector_size = 50
    
    balls =[]       # list containing balls
    members = []    # list containing players

    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((self.screen_w, self.screen_h))
        self.fps_clock = pygame.time.Clock()
        self.timer = 0

        # 2D array containing arrays, to store object in the secotrs and optimise collisions
        self.sectors = [ [ [] for j in range(ceil(self.screen_h/self.sector_size))] for i in range(ceil(self.screen_w/self.sector_size))] 

        # create goals
        screen_margin = (self.screen_h-self.pitch_h)/2
        self.goal_left = Goal(self, self.pitch_color ,(self.screen_w-self.pitch_w)/2, screen_margin + self.pitch_h*6/16, screen_margin + self.pitch_h*10/16, 50, -1)
        self.goal_right = Goal(self, self.pitch_color, self.pitch_w + (self.screen_w-self.pitch_w)/2, screen_margin + self.pitch_h*6/16, screen_margin + self.pitch_h*10/16, 50, 0)


    def draw_background(self):
        # draw backgroud
        self.screen.fill(self.back_color)

        # draw pitch
        pygame.draw.rect(self.screen, self.pitch_color, ((self.screen_w - self.pitch_w)/2,(self.screen_h - self.pitch_h)/2, self.pitch_w, self.pitch_h))
        
        # draw goals
        pygame.draw.rect(self.screen, self.goal_left.color, (self.goal_left.get_px(), self.goal_left.get_py(), self.goal_left.get_width(), self.goal_left.get_height()))
        pygame.draw.rect(self.screen, self.goal_right.color, (self.goal_right.get_px(), self.goal_right.get_py(), self.goal_right.get_width(), self.goal_right.get_height()))

        # draw poles
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

    def new_player(self, member):
        self.members.append(member)

    def new_ball(self, ball):
        self.balls.append(ball)

    def redraw(self):
        # update positions and redraw members
        self.update()

        # redraw screen
        pygame.display.update()

        # dt is time since last tick
        self.timer += self.clock_tick()

    def update(self):

        # update positions
        for obj in self.members:
            obj.update()
        for obj in self.balls:
            obj.update()
        if self.test_mode:
            # if test mode is on, draw additional markers on screen
            for obj in self.members:
                sector_num = int(obj.size * 4/ self.sector_size)
                for i in range(int(obj.p.x / self.sector_size) - sector_num, int(obj.p.x / self.sector_size) + sector_num + 1):
                    for j in range(int(obj.p.y / self.sector_size) - sector_num, int(obj.p.y / self.sector_size) + sector_num + 1):
                        pygame.draw.rect(self.screen, (0,255 - ((i + j) % 2) * 50, 0), (i * self.sector_size, j * self.sector_size, int(self.sector_size), int(self.sector_size)))
            for obj in self.balls:
                sector_num = int(obj.size * 4 / self.sector_size)
                for i in range(int(obj.p.x / self.sector_size) - sector_num, int(obj.p.x / self.sector_size) + sector_num + 1):
                    for j in range(int(obj.p.y / self.sector_size) - sector_num, int(obj.p.y / self.sector_size) + sector_num + 1):
                        pygame.draw.rect(self.screen, (0,255 - ((i + j) % 2) * 50, 0), (int(obj.p.x / self.sector_size)*self.sector_size, int(obj.p.y / self.sector_size) * self.sector_size, int(self.sector_size), int(self.sector_size)))

            for obj in self.members:
                pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.hitbox, (0,0,255))
            for obj in self.balls:
                pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.hitbox, (0,0,255))

        # check collisions and redraw all members
        for obj in self.members:
            Collision.collide(obj)
            pygame.gfxdraw.filled_circle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, obj.color)
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, (0,0,0))
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size-1, (0,0,0))

        Collision.collide(self.goal_left.post_up)
        Collision.collide(self.goal_left.post_down)
        Collision.collide(self.goal_right.post_up)
        Collision.collide(self.goal_right.post_down)

        for obj in self.balls:
            pygame.gfxdraw.filled_circle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, obj.color)
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size, (0,0,0))
            pygame.gfxdraw.aacircle(self.screen, int(obj.p.x), int(obj.p.y), obj.size-1, (0,0,0))



    def check_collision(self, obj):
        # check collision with pitch walls

            # Left wall
            if obj.p.x < int(obj.size + (self.screen_w - self.pitch_w)/2):
                if obj.p.y < self.goal_left.post_down.p.y and obj.p.y > self.goal_left.post_up.p.y:
                    if obj.p.x < self.goal_left.x - self.goal_left.width + obj.size:
                        obj.set_p(self.goal_left.x - self.goal_left.width + obj.size, obj.p.y)
                        obj.v *= 0
                else:
                    obj.set_p(int(obj.size + (self.screen_w - self.pitch_w)/2), obj.p.y)
                    obj.v.x *= -self.wall_bounce

            # Right wall
            if obj.p.x > int(self.pitch_w + ((self.screen_w - self.pitch_w)/2) - obj.size):
                if obj.p.y < self.goal_right.post_down.p.y and obj.p.y > self.goal_right.post_up.p.y:
                    if obj.p.x > self.goal_right.x + self.goal_right.width - obj.size:
                        obj.set_p(self.goal_right.x + self.goal_right.width - obj.size, obj.p.y)
                        obj.v *= 0
                else: 
                    obj.set_p(int(self.pitch_w + ((self.screen_w - self.pitch_w)/2) - obj.size), obj.p.y)
                    obj.v.x *= -self.wall_bounce

            # Top wall
            if obj.p.y < int(obj.size + (self.screen_h - self.pitch_h)/2):
                obj.set_p(obj.p.x, int(obj.size + (self.screen_h - self.pitch_h)/2))
                obj.v.y *= -self.wall_bounce

            # Bottom wall
            if obj.p.y > int(self.pitch_h + ((self.screen_h - self.pitch_h)/2) - obj.size):
                obj.set_p(obj.p.x, int(self.pitch_h + ((self.screen_h - self.pitch_h)/2) - obj.size))
                obj.v.y *= -self.wall_bounce