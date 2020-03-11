import pygame
from math import ceil
from HaxballOnline.CirclePhysical import CirclePhysical

class GameEngine(object):
    "object containing Game's data"
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

    posts = []       # list containing posts
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((self.screen_w, self.screen_h))
        self.fps_clock = pygame.time.Clock()
        self.timer = 0
        self.sectors = [ [ [] for j in range(ceil(self.screen_h/self.sector_size))] for i in range(ceil(self.screen_w/self.sector_size))] # 2D array containing arrays, to store object in the secotrs and optimise collisions

    def draw_background(self):
        self.screen.fill(self.back_color)
        pygame.draw.rect(self.screen, self.pitch_color, ((self.screen_w - self.pitch_w)/2,(self.screen_h - self.pitch_h)/2, self.pitch_w, self.pitch_h))

    def clock_tick(self):
        return self.fps_clock.tick(self.fps)

    def new_member(self, member):
        self.members.append(member)

    def new_ball(self, ball):
        self.balls.append(ball)

    def new_post(self,post):
        self.posts.append(post)

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
                pygame.draw.circle(self.screen, (0,0,255), (int(obj.p.x), int(obj.p.y)), obj.hitbox, 1)
            for obj in self.balls:
                pygame.draw.circle(self.screen, (0,0,255), (int(obj.p.x), int(obj.p.y)), obj.hitbox, 1)
            for obj in self.posts:
                pygame.draw.circle(self.screen, (0,0,255), (int(obj.p.x), int(obj.p.y)), obj.hitbox, 1)

        # check collisions and redraw all members
        for obj in self.members:
            obj.collide()
            pygame.draw.circle(self.screen, (0,0,0), (int(obj.p.x), int(obj.p.y)), obj.size)
            pygame.draw.circle(self.screen, obj.color, (int(obj.p.x), int(obj.p.y)), obj.size-2)

        for obj in self.balls:
            pygame.draw.circle(self.screen, (0,0,0), (int(obj.p.x), int(obj.p.y)), obj.size)
            pygame.draw.circle(self.screen, obj.color, (int(obj.p.x), int(obj.p.y)), obj.size-2)

        for obj in self.posts:
            pygame.draw.circle(self.screen, (0, 0, 0), (int(obj.p.x), int(obj.p.y)), obj.size)
            pygame.draw.circle(self.screen, obj.color, (int(obj.p.x), int(obj.p.y)), obj.size - 2)


    def check_collision(self, obj):
        # check collision with pitch walls

            # Left wall
            if obj.p.x < int(obj.size + (self.screen_w - self.pitch_w)/2):
                obj.p.x = int(obj.size + (self.screen_w - self.pitch_w)/2)
                obj.v.x *= -self.wall_bounce

            # Right wall
            if obj.p.x > int(self.pitch_w + ((self.screen_w - self.pitch_w)/2) - obj.size):
                obj.p.x = int(self.pitch_w + ((self.screen_w - self.pitch_w)/2) - obj.size)
                obj.v.x *= -self.wall_bounce

            # Top wall
            if obj.p.y < int(obj.size + (self.screen_h - self.pitch_h)/2):
                obj.p.y = int(obj.size + (self.screen_h - self.pitch_h)/2)
                obj.v.y *= -self.wall_bounce

            # Bottom wall
            if obj.p.y > int(self.pitch_h + ((self.screen_h - self.pitch_h)/2) - obj.size):
                obj.p.y = int(self.pitch_h + ((self.screen_h - self.pitch_h)/2) - obj.size)
                obj.v.y *= -self.wall_bounce