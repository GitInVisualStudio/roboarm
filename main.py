import RPi.GPIO as GPIO
import time
import math
import pygame
from servo import Servo

angle_alpha = 0
angle_beta = 0
alpha = Servo(pin=19) #first rotation
beta = Servo(pin=13) #second rotation
gamma = Servo(pin=6) #rotation on the x-y
grab = Servo(pin=26) 

DELAY = 1

def close():
    """
    release every servo and cleanup the connections
    """
    grab.close()
    alpha.close()
    gamma.close()
    beta.close()
    GPIO.cleanup()

def get_angles(*position):
    """
    moves the arm to the position
    positon is a 3d vector x, y, z
    """

    x, y, z = position

    length = math.sqrt(x**2 + y**2 + z**2)

    if length is 0:
        return

    x /= length
    y /= length
    z /= length
    
    if length > 2:
        length = 2

    if y < 0:
        y = 0
    
    angle = math.acos(length / 2) * 180 / math.pi
    angle_beta = 180 - angle * 2
    angle_alpha = math.asin(z) * 180 / math.pi
    #alpha.move(180 - angle - angle_alpha)
    """if angle + angle_alpha > 70:
        beta_angle += (angle + angle_alpha) - 70"""
    #beta.move(beta_angle)
    return angle + angle_alpha, angle_beta, math.atan2(y, x) * 180 / math.pi

pygame.init()

WIDTH, HEIGHT = 720, 480

screen = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()
FPS = 30

running = True

x, y, z = 1, 0, 0

while running:
    clock.tick(FPS)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            close()
            exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_w:
                x += 1
            if event.key == pygame.K_s:
                x -= 1
            if event.key == pygame.K_a:
                y += 1
            if event.key == pygame.K_d:
                y -= 1
            if event.key == pygame.K_u:
                z += 1
            if event.key == pygame.K_i:
                z -= 1
            scale = 0.1
            
            a, b, g = get_angles(x * scale, y * scale, z * scale) #returns the angles for alpha, beta and gamma

            alpha.move(180 - a) #alpha is placed in the wrong way, so we need to change the direction
            if a > 70:
                b += a - 70 #because beta influences alpha in the real world we need to consider this
            beta.move(b)
            gamma.move(g)

            screen.fill((0, 0, 0))
            length = 100
            offset = 200

            #alpha is turned 90 deg because the view is flipped
            angle = (a + 90) * math.pi / 180
            x1, y1 = offset + math.sin(angle) * length, offset + math.cos(angle) * length
            pygame.draw.line(screen, (255, 0, 0), (offset, offset), (x1, y1))

            #angle normalization, beta is the angle between the arms
            angle = (a + 180 + 90 + b) * math.pi / 180
            x2, y2 = math.sin(angle) * length, math.cos(angle) * length
            pygame.draw.line(screen, (0, 255, 0), (x1, y1), (x1 + x2, y1 + y2))

            pygame.draw.rect(screen, (0, 255, 0), (offset, offset, 10, 10))
            #destination where the arm should be...
            pygame.draw.rect(screen, (0, 0, 255), (offset + math.sqrt(x * x + y * y) * scale * length, offset - z * scale * length, 10, 10)) #negativ z because its flipped
            pygame.display.update()
