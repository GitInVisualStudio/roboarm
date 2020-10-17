import RPi.GPIO as GPIO
from servo import Servo
import numpy as np
import math

#init the arm
#alpha = 13, beta = 6, gamma = 22, delta = 5
servos = [Servo(13), Servo(6), Servo(22), Servo(5)]
x, y, z = 0, 0, 0

def close():
    for servo in servos:
        servo.close()
    GPIO.cleanup()

def move(*angles):
    prev_angle = math.pi / 2
    k = 0
    for angle, servo in zip(angles, servos):
        #if the serve is not a joing the prev_angle should be 90 deg, because only the joints influzes eachothers angles
        if k > 2:
            prev_angle = math.pi / 2
        servo.move(angle, prev_angle)
        prev_angle = servo.angle
        k += 1

def set_position(*position):
    x, y, z = position

def get_position():
    return x, y, z

def _current_position():
    angles = get_current_angles()[:3]
    return np.cos(angles).sum(), np.sin(angles).sum()

def move_to_position(position=None):
    if position is None:
        position = get_position()
    #TODO: make the invers kinemetics
    position = np.array(position)
    current_angles = get_current_angles()
    
    #gradient descent to move the arm
    length = math.sqrt(position[0]**2 + position[1]**2)
    target = np.array([length, position[2]])
    error = target - [np.cos(current_angles[:2]).sum(), np.sin(current_angles[:2]).sum()]

    gamma = np.arctan2(error[1], error[0])
    target = target - error / np.linalg.norm(error)

    pos2 = [np.cos(current_angles[0]).sum(), np.sin(current_angles[0]).sum()]
    error = target - pos2

    beta = np.arctan2(error[1], error[0])
    target = target - error / np.linalg.norm(error)

    error = target

    alpha = np.arctan2(error[1], error[0])

    delta = np.arctan2(position[1], position[0])
    
    new_angles = [alpha, beta, gamma, delta]

    for k, angle in enumerate(new_angles):
        if math.isnan(angle):
            new_angles[k] = 0

    move(*new_angles)

def get_current_angles():
    return np.array([servo.angle for servo in servos])
