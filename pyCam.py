import picamera
import RPi.GPIO as GPIO
import os
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(11, GPIO.IN) #start pin

cam = picamera.PiCamera()
camStarted = False

if cam:
	print('Camera connected.')
	while True:
		if (GPIO.input(11) and (camStarted == False)): #start video
			cam.start_recording('ChuteCam.h264')
			camStarted = True
			startTime = time.time()
			print('Recording started.')
		elif camStarted and ((time.time() - startTime) > 30) : #stop video
			cam.stop_recording()
			print('Recording stopped.')
else:
	print('No camera found, aborting.')