from window import Window
import threading
import pygame
import time
import math
import arm

x, y, z = 0, 0, 0

def get_angles(*position):
    """
    returns the angles needed for the arm to move to the position
    positon is a 3d vector x, y, z
    """

    x, y, z = position

    length = math.sqrt(x**2 + y**2 + z**2) #distance to the position

    #cant devide by 0 => return 0 when length is 0
    if length == 0:
        return 0, 0, 0
    
    #normalization of the position
    x /= length
    y /= length
    z /= length
    
    if length > 2: #limits the length the maximal length of the arm, one joint equals 1
        length = 2

    if y < 0: #the arm cant turn back
        y = 0
    
    angle = math.acos(length / 2) #rotation of the arms
    angle_beta = math.pi - angle #beta is the angle between the arms
    angle_alpha = angle + math.asin(z) #absolute rotation

    return angle_alpha, angle_beta, math.atan2(y, x)

def right():
    y -= 1

def left():
    y += 1

def forward():
    x += 1

def back():
    x -= 1

def up():
    z += 1

def down():
    z -= 1

def run_window():
    WIDTH, HEIGHT = 720, 480
    FPS = 10

    window = Window(WIDTH, HEIGHT)
    window.open()
    
    #adding events to controll the arm via keyboard
    #window.add_key_event((pygame.K_w, forward), (pygame.K_s, back), (pygame.K_a, left), (pygame.K_d, right), (pygame.K_SPACE, up), (pygame.K_u, down))
    clock = pygame.time.Clock()

    while running:
        window.update(arm.get_angles())
        clock.tick(FPS)

running = True
thread = threading.Thread(target=run_window)
thread.start()

#TODO: get the information from the esp and process it
arm.close()
