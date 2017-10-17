#define CLK_PIN 2
#define SDI_PIN 3
#define CS_PIN 4
#define LDAC_PIN 5
#define SW_PIN 6
#define FROM_PY_PIN 8
#define TO_PY_PIN 9

int value = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(CLK_PIN,OUTPUT);
pinMode(SDI_PIN,OUTPUT);
pinMode(CS_PIN,OUTPUT);
pinMode(LDAC_PIN,OUTPUT);
pinMode(SW_PIN,OUTPUT);
pinMode(FROM_PY_PIN,INPUT);
pinMode(TO_PY_PIN,OUTPUT);
digitalWrite(CLK_PIN, LOW);
}

void loop() {
    value++;
    value%=1000;
    //LDAC CS start up
    digitalWrite(LDAC_PIN, HIGH);
    digitalWrite(CS_PIN, LOW);
    //Write to DAC register
    digitalWrite(SDI_PIN, LOW);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
    //Unbuffered
    digitalWrite(SDI_PIN, LOW);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
    //Output gain = 1x
    digitalWrite(SDI_PIN, HIGH);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
    //Active mode oparation. Vout is available
    digitalWrite(SDI_PIN, HIGH);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
    //DAC Databits output
    for (int i = 0; i < 10; i++){
        if (((value >> (9 - i)) & 0x1) == 1)
            digitalWrite(SDI_PIN, HIGH);
        else
            digitalWrite(SDI_PIN, LOW);
        digitalWrite(CLK_PIN, HIGH);
        digitalWrite(CLK_PIN, LOW);
    }
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_PIN, LOW);
    //output
    digitalWrite(LDAC_PIN, LOW);
    digitalWrite(CS_PIN, HIGH);
}
