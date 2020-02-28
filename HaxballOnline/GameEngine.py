import pygame
from CirclePhysical import CirclePhysical

class GameEngine(object):
    "object containing Game's data"
    screen_w = 1100
    screen_h = int(screen_w / 1.57)
    pitch_w = int(screen_w * 0.8)
    pitch_h = int(pitch_w / 1.57)
    back_color = (164, 143, 91)
    pitch_color = (0, 160, 20)
    fps = 60
    wall_bounce = 1.0
    members = [] # list containing ball and all players
    test_mode = False

    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((self.screen_w, self.screen_h))
        self.fps_clock = pygame.time.Clock()

    def draw_background(self):
        self.screen.fill(self.back_color)
        pygame.draw.rect(self.screen, self.pitch_color, ((self.screen_w - self.pitch_w)/2,(self.screen_h - self.pitch_h)/2, self.pitch_w, self.pitch_h))

    def clock_tick(self):
        self.fps_clock.tick(self.fps)

    def new_member(self, member):
        self.members.append(member)

    def update(self):

        for obj in self.members:
            # update positions and redraw members on the screen
            obj.update()
            self.check_collision()
            pygame.draw.circle(self.screen, (0,0,0), (int(obj.p.x), int(obj.p.y)), obj.size)
            pygame.draw.circle(self.screen, obj.color, (int(obj.p.x), int(obj.p.y)), obj.size-2)
            if self.test_mode:
                pygame.draw.circle(self.screen, (0,0,255), (int(obj.p.x), int(obj.p.y)), obj.hitbox, 1)


    def check_collision(self):
        # check collision with pitch walls

        for obj in self.members:
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