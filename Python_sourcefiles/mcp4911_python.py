#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import RPi.GPIO as GPIO

#GPIO Num setup
GPIO.setmode(GPIO.BCM)
CLK_PIN = 4
SDI_PIN = 17
CS_PIN = 27
LDAC_PIN = 22
led = 5

#GPIO setup
GPIO.setup(led, GPIO.OUT)
GPIO.setup(CLK_PIN, GPIO.OUT)
GPIO.setup(SDI_PIN, GPIO.OUT)
GPIO.setup(CS_PIN, GPIO.OUT)
GPIO.setup(LDAC_PIN, GPIO.OUT)
GPIO.output(CLK_PIN, False)

def DACout(value):
    #LDAC CS start up
    GPIO.output(LDAC_PIN, True)
    GPIO.output(CS_PIN, False)
    #Write to DAC register
    GPIO.output(SDI_PIN, False)
    GPIO.output(CLK_PIN, True)
    GPIO.output(CLK_PIN, False)
    #Unbuffered
    GPIO.output(SDI_PIN, False)
    GPIO.output(CLK_PIN, True)
    GPIO.output(CLK_PIN, False)
    #Output gain = 1x
    GPIO.output(SDI_PIN, True)
    GPIO.output(CLK_PIN, True)
    GPIO.output(CLK_PIN, False)
    #Active mode oparation. Vout is available
    GPIO.output(SDI_PIN, True)
    GPIO.output(CLK_PIN, True)
    GPIO.output(CLK_PIN, False)
    #DAC Databits output
    for i in range(10)
        if ((value >> i) & 0x1) == 1
            GPIO.output(SDI_PIN, True)
        else:
            GPIO.output(SDI_PIN, False)
        GPIO.output(CLK_PIN, True)
        GPIO.output(CLK_PIN, False)
    #output
    GPIO.output(LDAC_PIN, False)
    GPIO.output(CS_PIN, True)
    return

outvltg = 2.5
outvltg = int(outvltg * 4095 / 5)
DACout(outvltg)
time.sleep(5)

GPIO.cleanup()
