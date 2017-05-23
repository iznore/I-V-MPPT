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

boolean g_i = 0;
float g_crnt[200]={}, g_vltg[200]={};
int g_count = 0;

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
  unsigned long  l = millis();
  ((l%=1000) > 500)? g_i = true : g_i=false;
  
  
  
  Serial.print(g_i);
  Serial.print(',');
  digitalWrite(2,g_i);
  Calc(g_vltg, M_CRNT, T_CRNT);
  Serial.print(',');
  Calc(g_crnt, M_VLTG, T_VLTG);
  Serial.println();
  g_count++;
}

void Calc(float *rcv, unsigned int mode, float trans){
  int RcvData[2]={};

  if(*rcv == NULL){
    Serial.println("Rcv allay is full.");
    return;
  }
  
  Wire.beginTransmission(SLVAD);  //Read data.
  Wire.write(mode);
  Wire.endTransmission();
  Wire.requestFrom(SLVAD, 2);
  
  while (Wire.available()){
    RcvData[Wire.available() - 1] = Wire.read();
  }
  *rcv = RcvData[0] + (RcvData[1] << 8);
  rcv++;
}

void ClearData(float *data, int *counter){
  int i = 0;
  int len = sizeof(data)/sizeof(float);

  *counter = 0;
  for(; i<len; i++){
    data[i] = 0;
  }
}

void Print(float *data){

}

