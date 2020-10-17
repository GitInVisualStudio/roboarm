import RPi.GPIO as GPIO
import time
import math

class Servo:

    def __init__(self, pin):
        self.pin = pin
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(pin, GPIO.OUT)
        self.servo = GPIO.PWM(pin, 50)
        self.servo.start(0)
        self.angle = 0.0

    def move(self, angle, prev_servo):
        self.angle = angle
        angle = math.degrees(angle)
        angle = angle + 90 - math.degrees(prev_servo)
        if angle > 180:
            self.angle = math.pi / 2 + prev_servo
            angle = 180
        if angle < 0:
            self.angle = prev_servo - math.pi / 2
            angle = 0
        if self.pin == 6: #if you ask, the arm is bullshit
            angle = 180 - angle
        angle = angle / 18 + 2
        self.servo.ChangeDutyCycle(angle)

    def close(self):
        self.servo.stop()
