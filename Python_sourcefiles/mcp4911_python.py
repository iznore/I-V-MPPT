import spidev
import time
import RPi.GPIO as GPIO

def SetDAChighbyte(value):
    return highbyte = value >> 8

def SetDAClowbyte(value):
    return lowbyte = value & 0xff

# ADCが接続されているチャネル
mcp_channel = 0

#LED GPIO
GPIO.setmode(GPIO.BCM)
led = 5
GPIO.setup(led, GPIO.OUT)

#SPIバスへのアクセスを開く
spi = spidev.SpiDev()
spi.open(0, 0)

value = 500

try:
    while True:
        r = spi.xfer2([(0x30 | SetDAChighbyte(value)), SetDAClowbyte(value)])
        time.sleep(1)
except KeyboardInterrupt:
    spi.close()

GPIO.cleanup()
