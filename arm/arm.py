from servo import Servo
import RPi.GPIO as GPIO

#init the arm
alpha, beta, gamma, grab  = 0, 0, 0, 0
servos = [Servo(19), Servo(13), Servo(6), Servo(26)]

def close():
    for servo in servos:
        servo.close()
    GPIO.cleanup()

def move_arm(angles):
    alpha, beta, gamma, grab = angles
    for servo, angle in zip(angles, servo):
        servo.move(angle)

def get_angles():
    return alpha, beta, gamma, grab
