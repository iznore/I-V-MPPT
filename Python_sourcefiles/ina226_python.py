import RPi.GPIO as GPIO
import time
import commands
#電圧測定部分の関数
def GetV():
?check = commands.getoutput("i2cget -y 1 0x4f 0x02 w")
?return (int(check[4:6],16)*256+int(check[2:4],16))*1.25/1000
#電流測定部分の関数
def GetA():
?check = commands.getoutput("i2cget -y 1 0x4f 0x04 w")
?if int(check[4:6],16)<128:
??return (int(check[4:6],16)*256+int(check[2:4],16))*0.1
?else:
??return (int(check[4:6],16)*256+int(check[2:4],16)-256*256)*0.1

#LEDを点灯するためのセットアップ、GPIO5を出力に設定
GPIO.setmode(GPIO.BCM)
GPIO.setup(5,GPIO.OUT)
#測定モジュールをセットアップ、0x05にa00を書き込むと電流測定時の値が1→1mAになる
check = commands.getoutput("i2cset -y 1 0x4f 0x05 0x0a 0x00 i")
time.sleep(1) #念のため1秒待つ
print str(GetV())+" V, "+str(GetA())+" mA "
#LED点灯
GPIO.output(5,GPIO.HIGH)
print "LED-on"
time.sleep(1) #念のため1秒待つ
print str(GetV())+" V, "+str(GetA())+" mA "
#LED消灯
GPIO.output(5,GPIO.LOW)
print "LED-off"
time.sleep(1) #念のため1秒待つ
print str(GetV())+" V, "+str(GetA())+" mA "
#GPIOを開放
GPIO.cleanup()
exit
