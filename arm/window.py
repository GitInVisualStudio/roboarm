import math
import pygame
import numpy as np

class Window:
     
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.events = []
        self.add_event((pygame.QUIT, exit))
        self.camera = np.array([-10, 0, 0])
        self.x = 0
        self.y = 0
        self.z = 10

    def open(self):
        self.screen = pygame.display.set_mode((self.width, self.height))

    def add_event(self, *arg):
        for term, action in arg:
            self.events.append((term, action))

    def __check_events(self):
        for event in pygame.event.get():
            if event.type is not pygame.KEYDOWN:
                break
            if event.key == pygame.K_w:
                self.x += 1
            if event.key == pygame.K_s:
                self.x -= 1
            if event.key == pygame.K_a:
                self.y += 1
            if event.key == pygame.K_d:
                self.y -= 1
            for term, action in self.events:
                if event.key == term:
                    action()
    
    def get_point(self, *position):
        position = np.array(position)
        length = math.sqrt((position * position).sum())
        if length == 0:
            return 0, 0
        #NOTE: the fov is 90deg to simplify things, thats why we dont need a clip matrix
        return ((position - self.camera) * length / position[2] + self.camera)[:2]

    def update(self, angles):
        self.__check_events()
        #TODO: render the arm
        x, y = self.get_point(self.x, self.y, self.z)
        self.screen.fill((0, 0, 0))
        pygame.draw.rect(self.screen, (0, 255, 0), (x, y, 2, 2))
        pygame.display.update()
