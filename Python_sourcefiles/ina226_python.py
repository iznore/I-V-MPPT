import RPi.GPIO as GPIO
import time
import commands
#�d�����蕔���̊֐�
def GetV():
?check = commands.getoutput("i2cget -y 1 0x4f 0x02 w")
?return (int(check[4:6],16)*256+int(check[2:4],16))*1.25/1000
#�d�����蕔���̊֐�
def GetA():
?check = commands.getoutput("i2cget -y 1 0x4f 0x04 w")
?if int(check[4:6],16)<128:
??return (int(check[4:6],16)*256+int(check[2:4],16))*0.1
?else:
??return (int(check[4:6],16)*256+int(check[2:4],16)-256*256)*0.1

#LED��_�����邽�߂̃Z�b�g�A�b�v�AGPIO5���o�͂ɐݒ�
GPIO.setmode(GPIO.BCM)
GPIO.setup(5,GPIO.OUT)
#���胂�W���[�����Z�b�g�A�b�v�A0x05��a00���������ނƓd�����莞�̒l��1��1mA�ɂȂ�
check = commands.getoutput("i2cset -y 1 0x4f 0x05 0x0a 0x00 i")
time.sleep(1) #�O�̂���1�b�҂�
print str(GetV())+" V, "+str(GetA())+" mA "
#LED�_��
GPIO.output(5,GPIO.HIGH)
print "LED-on"
time.sleep(1) #�O�̂���1�b�҂�
print str(GetV())+" V, "+str(GetA())+" mA "
#LED����
GPIO.output(5,GPIO.LOW)
print "LED-off"
time.sleep(1) #�O�̂���1�b�҂�
print str(GetV())+" V, "+str(GetA())+" mA "
#GPIO���J��
GPIO.cleanup()
exit
