#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import RPi.GPIO as GPIO
import commands
import datetime

#ivcalc vals
vltg = []
crnt = []
calctime = []

#Define numbers
vms = 200
ctvms = 13

#GPIO Num setup
GPIO.setmode(GPIO.BCM)
CLK_PIN = 4
SDI_PIN = 17
CS_PIN = 27
LDAC_PIN = 22
SW_PIN = 5

#GPIO setup
GPIO.setup(SW_PIN, GPIO.IN)
GPIO.setup(CLK_PIN, GPIO.OUT)
GPIO.setup(SDI_PIN, GPIO.OUT)
GPIO.setup(CS_PIN, GPIO.OUT)
GPIO.setup(LDAC_PIN, GPIO.OUT)
GPIO.output(CLK_PIN, False)

#i2c device:ina226 setup
check = commands.getoutput("i2cset -y 1 0x4f 0x00 0x40 0x07 i")
time.sleep(1)

#file setup
now = datetime.datetime.now()
f = open(now.strftime("FETIVSCAN_%Y%m%d_%H%M%S.csv"), 'w')
f.write(now.strftime("%Y%m%d_%H:%M:%S.") + "%04d" % (now.microsecond // 1000) + '\n')
f.write('Time[h:m:s.ms],Voltage[mV],Current[mA]\n')

#others setup
initstate = GPIO.input(SW_PIN)
lptime = vms * ctvms

def GetV():
    check = commands.getoutput("i2cget -y 1 0x4f 0x02 w")
    return (int(check[4:6],16)*256+int(check[2:4],16))*1.25/1000

def GetA():
    check = commands.getoutput("i2cget -y 1 0x4f 0x01 w")
    if int(check[4:6],16)<128:
        return (int(check[4:6],16)*256+int(check[2:4],16))*0.1
    else:
        return (int(check[4:6],16)*256+int(check[2:4],16)-256*256)*0.1

def CalcIV():
    vltg.append(GetV())
    crnt.append(GetA())
    now = datetime.datetime.now()
    calctime.append(now.strftime("%H:%M:%S.") + "%04d" % (now.microsecond // 1000))
    return

def InitDatas():
    vltg = []
    crnt = []
    calctime = []
    return

def DACout(value):
    #print('digital value = ' + str(value))
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
    for i in range(10):
        if ((value >> (9 - i)) & 0x1) == 1:
            GPIO.output(SDI_PIN, True)
        else:
            GPIO.output(SDI_PIN, False)
        GPIO.output(CLK_PIN, True)
        GPIO.output(CLK_PIN, False)
    GPIO.output(CLK_PIN, True)
    GPIO.output(CLK_PIN, False)
    GPIO.output(CLK_PIN, True)
    GPIO.output(CLK_PIN, False)
    #output
    GPIO.output(LDAC_PIN, False)
    GPIO.output(CS_PIN, True)
    return

#main
while(GPIO.input(SW_PIN) == initstate):
    for j in range(lptime):
        outvltg =  float(j) / lptime * 819
        DACout(int(outvltg))
        CalcIV()
    DACout(0)
    for num in range(len(vltg)):
        #print calctime[num] + 's, ' + str(vltg[num]) + " V, " + str(crnt[num])+" mA "
        f.write(calctime[num] + ',' + str(vltg[num]) + ',' + str(crnt[num]) + ',\n')
    InitDatas()

DACout(0)
f.close()
GPIO.cleanup()
