import RPi.GPIO as GPIO
import time
import commands

def GetV():
check = commands.getoutput("i2cget -y 1 0x4f 0x02 w")
return (int(check[4:6],16)*256+int(check[2:4],16))*1.25/1000

def GetA():
check = commands.getoutput("i2cget -y 1 0x4f 0x04 w")
if int(check[4:6],16)<128:
return (int(check[4:6],16)*256+int(check[2:4],16))*0.1
else:
return (int(check[4:6],16)*256+int(check[2:4],16)-256*256)*0.1


GPIO.setmode(GPIO.BCM)
GPIO.setup(5,GPIO.OUT)

check = commands.getoutput("i2cset -y 1 0x4f 0x05 0x0a 0x00 i")
time.sleep(1)
print str(GetV())+" V, "+str(GetA())+" mA "

GPIO.output(5,GPIO.HIGH)
print "LED-on"
time.sleep(1)
print str(GetV())+" V, "+str(GetA())+" mA "

GPIO.output(5,GPIO.LOW)
print "LED-off"
time.sleep(1)
print str(GetV())+" V, "+str(GetA())+" mA "

GPIO.cleanup()
exit
