from servo import Servo
import numpy as np
import math

#init the arm
#alpha = 13, beta = 6, gamma = 22, delta = 5
servos = [Servo(13), Servo(6), Servo(22)]
x, y, z = 0, 0, 0

def close():
    for servo in servos:
        servo.close()
    GPIO.cleanup()

def move(*angles):
    for angle, servo in zip(angles, servos):
        servo.move(angle)

def set_position(*position):
    x, y, z = position

def get_position():
    return x, y, z

def _current_position():
    angles = get_current_angles()
    return np.cos(angles).sum(), np.sin(angles).sum()

def move_to_position(position=None):
    if position is None:
        position = get_position()
    #TODO: make the invers kinemetics
    position = np.array(position)
    current_angles = get_current_angles()
    
    #gradient descent to move the arm
    target = np.array([math.sqrt(position[0]**2 + position[1]**2), position[2]])
    error = target - [np.cos(current_angles[:2]).sum(), np.sin(current_angles[:2]).sum()]

    if np.linalg.norm(error) == 0:
        return

    gamma = np.arctan2(error[1], error[0])
    target = target - error / np.linalg.norm(error)

    pos2 = [np.cos(current_angles[0]).sum(), np.sin(current_angles[0]).sum()]
    error = target - pos2

    if np.linalg.norm(error) == 0:
        return

    beta = np.arctan2(error[1], error[0])
    target = target - error / np.linalg.norm(error)

    error = target

    if np.linalg.norm(error) == 0:
        return

    alpha = np.arctan2(error[1], error[0])
    
    move(alpha, beta, gamma)

def get_current_angles():
    return np.array([servo.angle for servo in servos])
