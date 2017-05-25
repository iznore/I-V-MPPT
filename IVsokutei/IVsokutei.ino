#include <Wire.h>

#define SLVAD 0b01001111
#define T_CRNT 0.1    //LSB Current value [mA]
#define T_VLTG 1.25   //LSB Voltage value [mV]
#define C_CRNT 0.2    //Current colect value [mA]
#define C_VLTG 22     //Voltage colect value [mV]
#define DATASIZE  500

enum{
  M_STNG,
  M_CRNT,
  M_VLTG,
};

boolean g_sw = 0;
float g_crnt[DATASIZE]={}, g_vltg[DATASIZE]={};
int g_count = 0;
unsigned long g_past = 0;

void IV_Print();
void Calc(float *, unsigned int, float);

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Wire.beginTransmission(SLVAD);
  Wire.write(M_STNG);
  Wire.write(0b01000000);
  Wire.write(0b00000111);
  Wire.endTransmission();
  Wire.requestFrom(SLVAD, 2);
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
  Serial.println("Trin,I,V");
}

void loop()
{
  unsigned long l = millis();
  
  if((l%=1000) >= 500) g_sw = 1;
  else g_sw = 0;

  if(l-g_past >= 200){
    IV_Print();
    g_count=0;
    g_past = millis();
  }
  
  
  digitalWrite(2,g_sw);
  if(g_count < DATASIZE){
    Calc(g_crnt, M_CRNT, T_CRNT);
    Calc(g_vltg, M_VLTG, T_VLTG);
  }else Serial.println("Data is full.");
  g_count++;
}

void Calc(float *p, unsigned int mode, float trans){
  byte RcvData[2]={};
  Wire.beginTransmission(SLVAD);  //Read data.
  Wire.write(mode);
  Wire.endTransmission();
  Wire.requestFrom(SLVAD, 2);
  
  while (Wire.available()){
    RcvData[Wire.available() - 1] = Wire.read();
  }
  p[g_count] = (float)(RcvData[0] + (RcvData[1] << 8)); 
}

void IV_Print(){
  int i = 0;
  while(i < g_count){
    Serial.print(g_sw);
    Serial.print(',');
    Serial.print(g_crnt[i]);
    Serial.print(',');
    Serial.println(g_vltg[i]);
    i+=10;
  }
}

