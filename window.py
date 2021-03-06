import arm
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
        #clear the current screen
        self.screen.fill((0, 0, 0))

        #TODO: render the arm

        alpha, beta, gamma, delta = angles
        #translation
        _x, _y = 200, 200
        length = 50
        #angles are negative because the top starts at 0
        x, y = _x + math.cos(-alpha) * length, _y + math.sin(-alpha) * length
        
        pygame.draw.line(self.screen, (255, 0, 0), (_x, _y), (x, y))

        x2, y2 = x + math.cos(-beta) * length, y + math.sin(-beta) * length
        pygame.draw.line(self.screen, (0, 255, 0), (x, y), (x2, y2))

        x3, y3 = x2 + math.cos(-gamma) * length, y2 + math.sin(-gamma) * length
        pygame.draw.line(self.screen, (0, 0, 255), (x2, y2), (x3, y3))
        
        #TODO: render the target position
        x = _x + math.sqrt(arm.x**2 + arm.y**2) * length
        y = _y - arm.z * length #opposite direction
        pygame.draw.rect(self.screen, (0, 255, 255), (x, y, 5, 5))

        pygame.display.update()
