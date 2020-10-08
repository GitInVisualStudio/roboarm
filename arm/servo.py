import RPi.GPIO as GPIO
import time
import math

class Servo:

    def __init__(self, pin, default_delay=0):
        self.pin = pin
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(pin, GPIO.OUT)
        self.servo = GPIO.PWM(pin, 50)
        self.servo.start(1)
        self.default_delay = default_delay
        self.angle = 0

    def move(self, angle):
        angle = math.degrees(angle)
        self.angle = angle
        angle = angle / 18 + 2
        self.servo.ChangeDutyCycle(angle)
        time.sleep(self.default_delay / 1000)

    def close(self):
        self.servo.stop()
