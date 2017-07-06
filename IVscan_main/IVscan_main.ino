#include <Wire.h>

#define SLVAD 0b01001111
#define T_CRNT 0.1    //LSB Current value [mA]
#define T_VLTG 1.25   //LSB Voltage value [mV]
#define C_CRNT 0.2    //Current colect value [mA]
#define C_VLTG 22     //Voltage colect value [mV]
#define RasPi_IN 7
#define RasPi_OUT 8
#define RasPi_LED 13
#define CALC_TIME 200 //計測時間[ms]

enum{
  M_STNG,
  M_CRNT,
  M_VLTG,
};

union Data{
  int out;
  byte law[2];
} RcvData;

int Calc(unsigned int);
void CalcIVP();
void ResetAndShow();
void RasPi_ResetAndShow();
void ShowData();
void Seido_Sokutei(int);
int RasPi_Shake();
void RC_Seitei(int);

unsigned long g_past = 0;
unsigned char g_times = 0;
int g_crnt[100];
unsigned int g_vltg[100];
float MPP_tre = 0;
unsigned char MPP_n = 0;
boolean level;


void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(250000);  // start serial for output
  
  Wire.beginTransmission(SLVAD);
  Wire.write(M_STNG);
  Wire.write(0b01000000);
  Wire.write(0b00000111);
  Wire.endTransmission();
  Wire.requestFrom(SLVAD, 2);
  
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  pinMode(RasPi_IN, INPUT);
  level = digitalRead(RasPi_IN);
  pinMode(RasPi_OUT, OUTPUT);
  digitalWrite(RasPi_OUT, 0);
}

void loop()
{ 
  /*---RasPiと同期実験用---*/
  /*
  switch(RasPi_Shake()){
    case 1:
      Serial.println("MPPT START.");
      digitalWrite(RasPi_LED, HIGH);
      g_past = millis();
      digitalWrite(2, 0);
      while ((millis()-g_past) <= CALC_TIME){
        CalcIVP();      //IVP計算
      }
      break;
    case 0:
      Serial.println("MPPT END.");
      digitalWrite(RasPi_LED, LOW);
      RasPi_ResetAndShow();   //IVPdata送信&リセット
      break;
    case -1:
      Serial.println("Connection timeout.");
      break;
    default:
      Serial.println("Unknown error.");
  }
  */

  /*---単機実験用---*/
  ResetAndShow();
  CalcIVP();

  /*---RC整定用(方形波発生)---*/
  //RC_Seitei(CALC_TIME);

  /*---ina226精度測定用---*/
  //Seido_Sokutei()
    
}

//RC整定用. 引数： msec = 測定時間[ミリ秒]
void RC_Seitei(int msec){
  digitalWrite(2, 1);
  delay(msec);
  digitalWrite(2, 0);
  delay(msec);
}

//返り値： 1 = 開始命令受信,   0 = 終了命令受信,   -1 = タイムアウト
int RasPi_Shake(){
  unsigned long l = millis();
  boolean input;
  while((input = digitalRead(RasPi_IN)) == level){
      if(millis() - l > 5000)
        return -1;                       //5s来なかったらタイムアウト
  }   //RasPiからの信号を受信するまでまつ
  level = input;
  digitalWrite(RasPi_OUT, input);        //RasPiに応答
  return (int)input;
}

void Seido_Sokutei(int msec){
  Serial.print((float)Calc(M_VLTG) * T_VLTG);
  Serial.print(',');
  Serial.println((float)Calc(M_CRNT) * T_CRNT);
  delay(msec);
}


int Calc(unsigned int mode){  
  Wire.beginTransmission(SLVAD);  //Read data.
  Wire.write(mode);
  Wire.endTransmission();
  Wire.requestFrom(SLVAD, 2);
  
  while (Wire.available()){
    RcvData.law[Wire.available() - 1] = Wire.read();
  }
  return RcvData.out; 
}

void CalcIVP(){
  float temp;
  g_crnt[g_times] = Calc(M_CRNT);
  g_vltg[g_times] = Calc(M_VLTG);
  temp = (float)g_crnt[g_times] * (float)g_vltg[g_times];
  if(temp > MPP_tre){
    MPP_tre = temp;
    MPP_n = g_times;
  }
  g_times++;
  delay(5);
}

void ResetAndShow(){
  if((millis()-g_past) > CALC_TIME){
    digitalWrite(2, 1);
    ShowData();
    g_times = 0;
    MPP_n = 0;
    MPP_tre = 0;
    delay(40);
    //delayMicroseconds(500);
    g_past=millis();
    digitalWrite(2, 0);
  }
}

void RasPi_ResetAndShow(){
    digitalWrite(2, 1);
    ShowData();
    g_times = 0;
    MPP_n = 0;
    MPP_tre = 0;
}

void ShowData(){
  int i;
  Serial.print("Calc times = ");
  Serial.println(g_times);
  //delay(1000);
  
  for(i = 0; i < g_times; i++){
    Serial.print(i);
    Serial.print(", ,");
    Serial.print((float)g_vltg[i] * T_VLTG);
    Serial.print(",[mV] ,");
    Serial.print((float)g_crnt[i] * T_CRNT);
    Serial.println(",[mA] ,");
  }
  
    Serial.println("MPPdatas...");
    Serial.print((float)g_vltg[MPP_n] * T_VLTG);
    Serial.print(",[mV] ,");
    Serial.print((float)g_crnt[MPP_n] * T_CRNT);
    Serial.println(",[mA] ,");
    
}

