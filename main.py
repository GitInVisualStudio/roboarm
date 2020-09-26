import numpy as np
import RPi.GPIO as GPIO
import time
import math
from servo import Servo

alpha = Servo(pin=13) #first rotation
beta = Servo(pin=19) #second rotation
gamma = Servo(pin=6) #rotation on the x-y
grab = Servo(pin=26) 

DELAY = 1

def init():
    rotation.move(90)
    grab.move(0)
    forward.move(90)
    height.move(180)

def close():
    """
    release every servo and cleanup the connections
    """
    grab.close()
    forward.close()
    rotation.close()
    height.close()
    GPIO.cleanup()


def move_to_position(positon):
    """
    moves the arm to the position
    positon is a 3d vector x, y, z
    """
    x, y, z = position

    #first calculate the need rotation on the x-y-axis
    gamma.move(math.atan(y, x) * 180 / math.pi)

    #calculate the distance to the point
    length = math.sqrt(x**2 + y**2 + z**2)

    #calculate the raltive angle for alpha to get the absolut angle of beta
    angle = math.acos(length / 2) * 180 / math.pi
    beta.move(angle / 2)

    #calculate the absolute angle of alpha
    length = math.sqrt(x**2 + y**2)
    angle += math.atan(z / length) * 180 / math.pi
    alpha.move(angle)


init()

time.sleep(DELAY)

close()
