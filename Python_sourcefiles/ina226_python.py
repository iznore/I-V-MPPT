import RPi.GPIO as GPIO
import time
import commands
import datetime

vltg = []
crnt = []
calctime = []

def GetV():
    check = commands.getoutput("i2cget -y 1 0x4f 0x02 w")
    return (int(check[4:6],16)*256+int(check[2:4],16))*1.25/1000

def GetA():
    check = commands.getoutput("i2cget -y 1 0x4f 0x01 w")
    if int(check[4:6],16)<128:
        return (int(check[4:6],16)*256+int(check[2:4],16))*0.1
    else:
        return (int(check[4:6],16)*256+int(check[2:4],16)-256*256)*0.1


GPIO.setmode(GPIO.BCM)
GPIO.setup(5,GPIO.OUT)
check = commands.getoutput("i2cset -y 1 0x4f 0x00 0x40 0x07 i")
time.sleep(1)

GPIO.output(5,GPIO.HIGH)
print "LED-on"
time.sleep(1)

start = time.time()

while time.time() - start < 1.0 :
    vltg.append(GetV())
    crnt.append(GetA())
    now = datetime.datetime.now()
    calctime.append(now.strftime("%H:%M:%S.") + "%04d" % (now.microsecond // 1000))
    time.sleep(0.1)


GPIO.output(5,GPIO.LOW)
print "LED-off"
time.sleep(1)

for num in range(len(vltg)) :
    print calctime[num] + 's, ' + str(vltg[num]) + " V, " + str(crnt[num])+" mA "

GPIO.cleanup()
exit
