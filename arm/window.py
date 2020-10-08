import math
import pygame
import numpy as np

class Window:
     
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.events = []
        self.keys = []
        self.add_event((pygame.QUIT, exit))

    def open(self):
        self.screen = pygame.display.set_mode((self.width, self.height))
    
    def add_key_event(self, *arg):
        for term, action in arg:
            self.keys.append((term, action))    

    def add_event(self, *arg):
        for term, action in arg:
            self.events.append((term, action))

    def __check_events(self):
        for event in pygame.event.get():
            for term, action in self.events:
                if event.type == term:
                    action()
            if event.type is not pygame.KEYDOWN:
                break
            for term, action in self.keys:
                if event.key == term:
                    action()

    def update(self, angles):
        self.__check_events()
        #TODO: render the arm

        alpha, beta, gamma, delta = angles
        #translation
        _x, _y = 100, 100
        length = 30
        x, y = _x + math.cos(alpha) * length, _y + math.sin(alpha) * length
        pygame.draw.line(self.screen, (0, 255, 0), (_x, _y), (x, y))

        pygame.display.update()
