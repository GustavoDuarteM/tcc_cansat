/* Incluindo a biblioteca */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Adafruit_BMP085.h>
#include <MQUnifiedsensor.h>
#include <Wire.h>

/* Definições da MQ135 */
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 
#define s_type "MQ-135" 
#define ADC_Bit_Resolution 10 
#define RatioMQ135CleanAir 3.6 
const int MPU=0x68;

/* Declarando bmp e MQ135 */
Adafruit_BMP085 bmp;
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, s_type);

enum Data_Types { mq135_type, accelerometer_type, bmp_type };

struct mq135_package{
  int type;
  float NH4;
  float Acetona;
  float CO;
  float Alcohol;
  float CO2;
};

struct accelerometer_package {
  int type;
  int acelX;
  int acelY;
  int acelZ;
  int giroX;
  int giroY;
  int giroZ;
};

struct bmp_package{
  int type;
  int Temperatura;
  float Altitude;
  float Pressao;
};

typedef struct mq135_package MQ135_package;
typedef struct accelerometer_package Accelerometer_package;
typedef struct bmp_package BMP_package;


/* Rádio entre o transmissor e receptor */
RF24 radio(9,10);  /* Certifique-se que estão conectados nos mesmos pinos */
const uint64_t pipes[2] = { 
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


void setup(){
  Serial.begin(9600); /* Na velocidade 9600 no Monitor Serial */
  bmp.begin(); /* Inicia o BMP180 */
  start_accelerometer();
  start_radio();
  start_MQ135();   
}

void loop(void){
  /* BMP data */
  read_bmp_data();
  delay(1000);
  /* MQ135 data */ 
  read_MQ135_data();
  delay(1000);
  /*accelerometer data*/
  read_accelerometer_data();
  delay(1000);
}

void start_accelerometer(){
  Wire.begin();
  Wire.beginTransmission(MPU);  
  Wire.write(0x6B);             

  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
}

void start_radio(){
  radio.begin(); /* Inicia o NRF2L001 */
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(pipes[0]);
  radio.enableDynamicPayloads();
  radio.stopListening();
}

void start_MQ135(){
  MQ135.setRegressionMethod(1); /* Inicia o MQ135 */
  MQ135.init(); 

  float calcR0 = 0; /* Formula de calibração do MQ135 */
    for(int i = 1; i<=10; i ++)
    {
      MQ135.update(); 
      calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    }
  MQ135.setR0(calcR0/10);
}

void read_accelerometer_data(){
  Accelerometer_package data; 
  data.type =  Data_Types::accelerometer_type;

  Wire.beginTransmission(MPU);      //transmite
  Wire.write(0x3B);                 // Endereço 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);     //Finaliza transmissão
  Wire.requestFrom(MPU,14,true);   //requisita bytes
  
  data.acelX = Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  data.acelY = Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  data.acelZ = Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Wire.read()<<8|Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  data.giroX = Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  data.giroY = Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  data.giroZ = Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  radio.write( &data, sizeof(data) );
}

void read_MQ135_data(){
  MQ135_package data;
  data.type =  Data_Types::mq135_type;
  
  MQ135.update(); /* Atualiza os dados do MQ135 */
  
  data.CO = MQ135.readSensor(); MQ135.setA(605.18); MQ135.setB(-3.937);
  data.Alcohol = MQ135.readSensor(); MQ135.setA(77.255); MQ135.setB(-3.18); 
  data.NH4 = MQ135.readSensor(); MQ135.setA(102.2 ); MQ135.setB(-2.473); 
  data.Acetona = MQ135.readSensor(); MQ135.setA(34.668); MQ135.setB(-3.369); 
  data.CO2 = MQ135.readSensor(); MQ135.setA(110.47); MQ135.setB(-2.862);
  radio.write(&data, sizeof(data));
}

void read_bmp_data(){
  BMP_package data; 
  data.type =  Data_Types::bmp_type;

  data.Temperatura = bmp.readTemperature();
  data.Altitude = bmp.readAltitude();
  data.Pressao = bmp.readPressure();
  radio.write(&data, sizeof(data) );
}
