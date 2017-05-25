#include <Wire.h>

#define SLVAD 0b01001111
#define T_CRNT 0.1    //LSB Current value [mA]
#define T_VLTG 1.25   //LSB Voltage value [mV]
#define C_CRNT 0.2    //Current colect value [mA]
#define C_VLTG 22     //Voltage colect value [mV]

enum{
  M_STNG,
  M_CRNT,
  M_VLTG,
};

float Calc(unsigned int, float);
unsigned long g_past = 0;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  
  /*
  Wire.beginTransmission(SLVAD);
  Wire.write(M_STNG);
  Wire.write(0b01000000);
  Wire.write(0b00000111);
  Wire.endTransmission();
  Wire.requestFrom(SLVAD, 2);
  */
  
  /*
  int RcvData[2]={};
  int Data=0;
  while (Wire.available())
  {
    RcvData[Wire.available() - 1] = Wire.read();    
  }
  Data = RcvData[0] + (RcvData[1] << 8);
  Serial.println(Data);
  */
  Serial.println("I,V");
}

void loop()
{ 
   
  if((millis()-g_past) >= 10){
    digitalWrite(2, 1);
    delay(1);
    g_past=millis();
  }
  digitalWrite(2, 0);
  
  /*
  Serial.print(Calc(M_CRNT, T_CRNT));
  Serial.print(',');
  Serial.println(Calc(M_VLTG, T_VLTG));
    */
}

float Calc(unsigned int mode, float trans){
  byte RcvData[2]={};
  Wire.beginTransmission(SLVAD);  //Read data.
  Wire.write(mode);
  Wire.endTransmission();
  Wire.requestFrom(SLVAD, 2);
  
  while (Wire.available()){
    RcvData[Wire.available() - 1] = Wire.read();
  }
  return (float)(RcvData[0] + (RcvData[1] << 8)) * trans; 
}


