import RPi.GPIO as GPIO
import time
import math

class Servo:

    def __init__(self, pin):
        self.pin = pin
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(pin, GPIO.OUT)
        self.servo = GPIO.PWM(pin, 50)
        self.servo.start(1)
        self.angle = 0.0

    def move(self, angle):
        self.angle = angle
        angle = math.degrees(angle)
        angle = angle / 18 + 2
        self.servo.ChangeDutyCycle(angle)

    def close(self):
        self.servo.stop()
