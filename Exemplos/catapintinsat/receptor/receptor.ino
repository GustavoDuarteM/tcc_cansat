/* Incluindo a biblioteca */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

enum Data_Types { mq135_type, accelerometer_type, bmp_package };

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
int next_type = 0;

/* Rádio entre o transmissor e receptor */
RF24 radio(9,10);  /* Certifique-se que estão conectados nos mesmos pinos */
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(){
  Serial.begin(9600); /* Na velocidade 9600 no Monitor Serial */
  radio.begin(); /* Inicia o NRF2L001 */
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1,pipes[0]);
  radio.enableDynamicPayloads();
  radio.startListening(); /* Rádio começa a receber */
}

void loop(void){
  if (radio.available()){    
    uint8_t len = radio.getDynamicPayloadSize();
    if(len == sizeof(MQ135_package)){  
      listening_MQ135();
    }else if(len == sizeof(Accelerometer_package)){
      listening_accelerometer();
    }else if(len == sizeof(BMP_package)){
      listening_bmp();
    }
  }
}

void listening_accelerometer(){
  Accelerometer_package data;
  radio.read(&data, sizeof(data));

  if(next_type == 1){
    Serial.println("|  Acelerômetro"); 
    Serial.print("|  Acel:"); 
    Serial.print("  X:");Serial.print(data.acelX);
    Serial.print(" Y:");Serial.print(data.acelY);
    Serial.print(" Z:");Serial.println(data.acelZ);
    
    Serial.print("|  Giro:"); 
    Serial.print("  X:");Serial.print(data.giroX);
    Serial.print(" Y:");Serial.print(data.giroY);
    Serial.print(" Z:");Serial.println(data.giroZ);
    Serial.println("|");

    set_step(data.type);
  }
}

void listening_bmp(){
  BMP_package data; 
  radio.read(&data, sizeof(data));

  if(next_type == 2){  
    Serial.println("|  BMP "); 
    Serial.print("|  Temperatura: "); Serial.print(data.Temperatura);Serial.println("°C");
    Serial.print("|  Altitude: "); Serial.print(data.Altitude);Serial.println("m");
    Serial.print("|  Pressão Atmosférica: "); Serial.print(data.Pressao);Serial.println(" Pa");
    Serial.println("|");
    Serial.println(" ------------------------------------");
    set_step(data.type);
  }
}

void listening_MQ135(){
  MQ135_package data;
  radio.read(&data, sizeof(data));

  if(next_type == 0){
    Serial.println("|  Leituras: "); 
    Serial.println(" ------------------------------------");
    Serial.println("|  MQ135 "); 
    Serial.print("|  Monóxido de carbono: "); Serial.println(data.CO);
    Serial.print("|  Dióxido de carbono: "); Serial.println(data.CO2 + 400);
    Serial.print("|  Álcool: "); Serial.println(data.Alcohol);
    Serial.print("|  Amónio: "); Serial.println(data.NH4);
    Serial.print("|  Acetona: "); Serial.println(data.Acetona);
    Serial.println("|"); 
    
    set_step(data.type);
  }
}

void set_step(int step){
  next_type = step + 1;
  if(next_type > 2){
    next_type = 0;
  }
}
