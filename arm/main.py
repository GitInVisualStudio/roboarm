from window import Window
import threading
import pygame
import time
import math
import arm

#sclae for the keyboard input
SCALE = .3

def right():
    arm.y -= SCALE 

def left():
    arm.y += SCALE

def forward():
    arm.x += SCALE

def back():
    arm.x -= SCALE

def up():
    arm.z += SCALE

def down():
    arm.z -= SCALE

def run_window():
    WIDTH, HEIGHT = 720, 480
    FPS = 10

    window = Window(WIDTH, HEIGHT)
    window.open()
    
    #adding events to controll the arm via keyboard
    window.add_key_event((pygame.K_w, forward), (pygame.K_s, back), (pygame.K_a, left), (pygame.K_d, right), (pygame.K_SPACE, up), (pygame.K_u, down))
    clock = pygame.time.Clock()

    while running:
        window.update(arm.get_current_angles())
        #just testing to see if gradient descent works...
        arm.move_to_position()
        clock.tick(FPS)

running = True
thread = threading.Thread(target=run_window)
thread.start()

#TODO: get the information from the esp and process it
#arm.close()
