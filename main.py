from window import Window
from servo import Servo
import threading
import pygame
import time
import math
import sys
import socket
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
    FPS = 5

    window = Window(WIDTH, HEIGHT)
    window.open()
    
    #adding events to controll the arm via keyboard
    window.add_key_event((pygame.K_w, forward), (pygame.K_s, back), (pygame.K_a, left), (pygame.K_d, right), (pygame.K_SPACE, up), (pygame.K_u, down))
    clock = pygame.time.Clock()

    while running:
        window.update(arm.get_current_angles())
        arm.move_to_position()
        clock.tick(FPS)

running = True
thread = threading.Thread(target=run_window)
#thread.start()

#TODO: get the information from the esp and process it

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('192.168.4.1', 1337)
socket.connect(server_address)

while True:
    data = socket.recvfrom(1024)
    if data:
        content, _ = data
        content = str(content)
        if ";" in content:
            try:
                content = content.replace('"', '')
                content = content.replace(char(39), '')
                content = content.split(';')
                x, y, z, f1, f2, f3, f4, f5 = [float(v) for v in content]
                arm.x = x;
                arm.y = y;
                arm.z = z;
                print("X: " + str(x) + " Y: " + str(y) + " Z:" + str(z))
                arm.move_to_position()
            except Exception:
                print("error")

socket.close()
