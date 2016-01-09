import RPi.GPIO as GPIO
import time

def main():
    while True:
        GPIO.output(25, GPIO.HIGH)
        time.sleep(0.5)
        GPIO.output(25, GPIO.HIGH)
        time.sleep(0.5)

GPIO.setmode(GPIO.BCM)
GPIO.setup(25, GPIO.OUT)

try:
    main()
finally:
    GPIO.cleanup()

