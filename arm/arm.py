from servo import Servo
import RPi.GPIO as GPIO

#init the arm
#alpha = 13, beta = 6, gamma = 22, delta = 5
servos = [Servo(13), Servo(6), Servo(22), Servo(5)]

def close():
    for servo in servos:
        servo.close()
    GPIO.cleanup()

def move_arm(*angles):
    for angle, servo in zip(angles, servos):
        servo.move(angle)

def get_angles():
    return [servo.angle for servo in servos]
