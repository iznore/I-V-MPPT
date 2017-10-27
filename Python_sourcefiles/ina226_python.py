import RPi.GPIO as GPIO
import time
import commands
import datetime
import smbus
from smbus import SMBus

vltg = []
crnt = []
calctime = []

def GetV():
    voltage_data = bus.read_i2c_block_data(0x40,0x02,2)
    return (voltage_data[0] * 256 + voltage_data[1]) * 1.25 * 0.001
    
def GetA():
    current_data = bus.read_i2c_block_data(0x40,0x01,2)
    return (current_data[0] * 256 + current_data[1]) * 0.1 * 0.001

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

#File out
f = open('result.csv', 'w')
for num in range(len(vltg)) :
    print calctime[num] + 's, ' + str(vltg[num]) + " V, " + str(crnt[num])+" mA "

f.close()
GPIO.cleanup()
exit
