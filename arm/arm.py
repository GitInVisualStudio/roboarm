import RPi.GPIO as GPIO
from servo import Servo
import numpy as np
import math

#init the arm
#rotation 17
#greifer 26
#alpha = 27, beta = 22, gamma = 13, delta = 19
servos = [Servo(27), Servo(22), Servo(13), Servo(26), Servo(17)]
x, y, z = 0, 0, 0

def close():
    """
    cloese every connected GPIO pin
    """
    for servo in servos:
        servo.close()
    GPIO.cleanup()

def move(*angles):
    """
    move the arm with the given angels
    NOTE: the given angles are relative to the x-y axis
    """
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
    """
    returns the current position of the arm in 2d
    """
    angles = get_current_angles()[:3]
    return np.cos(angles).sum(), np.sin(angles).sum()

def get_deriv(angle):
    """
    returns the derivative of an given angle
    """
    return np.array([-math.sin(angle), math.cos(angle)])

def move_to_position(position=None):
    if position is None:
        position = get_position()
    #TODO: make the invers kinemetics via gradient descent
    # our loss funtion is (current - target)**2
    position = np.array(position)
    current_angles = get_current_angles()

    #get the error of the target position, get the deriv of the loss func and adjust the angles
    # NOTE: the target is on 2d to simplify things
    length = math.sqrt(position[0]**2 + position[1]**2)
    target = np.array([position[0], position[2]])
    
    for epoch in range(100):
        #yeah i know RMSprop is kind a overkill, but our limits of the joints are quit huge and thats why there are many minima
        beta = 0.999
        epsilon = 1e-5
        step_size = 0.001
        r_angles = np.zeros((3, 2)) #cuz we have 3 joints and 2 values per joint, x and y
        #only the first 3 servos are joints 
        for index, angle in enumerate(current_angles[:3]):
            error = _current_position() - target
            deriv = get_deriv(angle) * error
            sq_deriv = deriv * deriv #i know i could square the sum directly, but my intention is, that on axis might be more important and that wouldnt be represented
            r_angles[index] = r_angles[index] * beta + (1 - beta) * sq_deriv
            current_angles[index] -= (deriv / np.sqrt(r_angles[index] + epsilon)).sum() * step_size
            move(*current_angles)

    #the last servo controls the rotation on the x-y and we dont need gradient descent to get the angle
    #current_angles[-1] = np.arctan2(position[1], position[0])
    print(position[1])
    current_angles[-1] = position[1]
    move(*current_angles)

def get_current_angles():
    return np.array([servo.angle for servo in servos])
