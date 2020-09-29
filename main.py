import numpy as np
import RPi.GPIO as GPIO
import time
import math
from servo import Servo

alpha = Servo(pin=19) #first rotation
beta = Servo(pin=13) #second rotation
gamma = Servo(pin=6) #rotation on the x-y
grab = Servo(pin=26) 

DELAY = 1

def init():
    gamma.move(90)
    grab.move(0)
    alpha.move(90)
    beta.move(0)

def close():
    """
    release every servo and cleanup the connections
    """
    grab.close()
    alpha.close()
    gamma.close()
    beta.close()
    GPIO.cleanup()


def move_to_position(*position):
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
    

    if y < 0:
        y = 0

    #first calculate the needed rotation on the x-y-axis
    gamma.move(math.atan2(y, x) * 180 / math.pi)

    #calculate the raltive angle for alpha to get the absolut angle of beta
    if length > 2:
        length = 2
    
    angle = math.acos(length / 2) * 180 / math.pi
    beta_angle = (90 - angle)

    #calculate the absolute angle of alpha
    #length_2 = math.sqrt(x*len)
    angle += math.asin(z) * 180 / math.pi
    alpha.move(180 - angle)
    print(angle)
    if angle < 90:
        beta_angle += angle
    beta.move(90 + beta_angle)


#init()
#time.sleep(DELAY)
grab.move(90)
time.sleep(DELAY)
grab.move(0)
"""x, y, z = 0, 0, 0
while True:
    key = input()
    size = 0.1
    if key is "w":
        x += size
    if key is "s":
        x -= size
    if key is " ":
        z += size
    if key is "u":
        z -= size
    if key is "a":
        y += size
    if key is "d":
        y -= size
    move_to_position(x, y, z)"""

time.sleep(DELAY)

close()
