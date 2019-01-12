#!/usr/bin/env python

import RPi.GPIO as GPIO
import serial, string, time
import os
import sys

FIRE_BTN = 16
ARM_SWTCH = 22
RX_LGHT = 12
SYS_FAULT_LGHT = 21
CMD_SNT_LGHT = 19
DECORATIVE_STUPIDITY = 5

GPIO.setmode(GPIO.BCM)
GPIO.setup(FIRE_BTN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(ARM_SWTCH, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(RX_LGHT, GPIO.OUT)
GPIO.setup(SYS_FAULT_LGHT, GPIO.OUT)
GPIO.setup(CMD_SNT_LGHT, GPIO.OUT)
GPIO.setup(DECORATIVE_STUPIDITY, GPIO.OUT)

def goodRX(ser):
	bite = ''
	try:
		bite = ser.read()
	except:
		print('No connection, resetting.')
		GPIO.output(RX_LGHT, GPIO.LOW)
		GPIO.output(SYS_FAULT_LGHT, GPIO.HIGH)
		os.execv('controller.py', sys.argv)

	print(bite)
	if bite == b'a':
		return True
	else:
		return False

def fire(ser):
	while True:
		ser.write(b'b')
		print('Fired.')

GPIO.output(DECORATIVE_STUPIDITY, GPIO.HIGH)
GPIO.output(CMD_SNT_LGHT, GPIO.LOW)
time.sleep(5)

try:
	ser = serial.Serial('/dev/ttyACM0', 9600, 8, 'N', 1, timeout=1)
except:
	print('No boot connection, resetting.')
	GPIO.output(SYS_FAULT_LGHT, GPIO.HIGH)
	os.execv('controller.py', sys.argv)

while True:
	if (GPIO.input(FIRE_BTN) == GPIO.LOW) and (GPIO.input(ARM_SWTCH) == GPIO.LOW) and goodRX(ser):
		print("FIRE")
		GPIO.output(CMD_SNT_LGHT, GPIO.HIGH)
		GPIO.output(RX_LGHT, GPIO.LOW)
		fire(ser)
	else:
		print("Inactive.")

	if goodRX(ser):
		GPIO.output(RX_LGHT, GPIO.HIGH)
		GPIO.output(SYS_FAULT_LGHT, GPIO.LOW)
	else:
		GPIO.output(RX_LGHT, GPIO.LOW)
		GPIO.output(SYS_FAULT_LGHT, GPIO.HIGH)

	time.sleep(.1)


