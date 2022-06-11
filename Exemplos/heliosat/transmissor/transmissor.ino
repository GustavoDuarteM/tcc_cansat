/* Incluindo a biblioteca */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Adafruit_BMP085.h>
#include <MQUnifiedsensor.h>


/* Definições da MQ135 */
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 
#define type "MQ-135" 
#define ADC_Bit_Resolution 10 
#define RatioMQ135CleanAir 3.6 


/* Declarando bmp e MQ135 */
Adafruit_BMP085 bmp;
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);


struct package
{
  int Temperatura;
  float Altitude;
  float Pressao;
  float CO;
  float Alcohol;
  float CO2;
  float NH4;
  float Acetona;
};

typedef struct package Package;
Package data; 


/* Rádio entre o transmissor e receptor */
RF24 radio(9,10);  /* Certifique-se que estão conectados nos mesmos pinos */
const uint64_t pipes[2] = { 
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


void setup()
{
  Serial.begin(9600); /* Na velocidade 9600 no Monitor Serial */
  radio.begin(); /* Inicia o NRF2L001 */
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  
  MQ135.setRegressionMethod(1); /* Inicia o MQ135 */
  MQ135.init(); 

  bmp.begin(); /* Inicia o BMP180 */

  float calcR0 = 0; /* Formula de calibração do MQ135 */
    for(int i = 1; i<=10; i ++)
    {
      MQ135.update(); 
      calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    }
  MQ135.setR0(calcR0/10);
}


void loop(void)
{ /* Conteúdo a enviar */
  data.Temperatura = bmp.readTemperature();
  data.Altitude = bmp.readAltitude();
  data.Pressao = bmp.readPressure();
  data.CO = MQ135.readSensor(); MQ135.setA(605.18); MQ135.setB(-3.937);
  data.Alcohol = MQ135.readSensor(); MQ135.setA(77.255); MQ135.setB(-3.18); 
  data.NH4 = MQ135.readSensor(); MQ135.setA(102.2 ); MQ135.setB(-2.473); 
  data.Acetona = MQ135.readSensor(); MQ135.setA(34.668); MQ135.setB(-3.369); 
  data.CO2 = MQ135.readSensor(); MQ135.setA(110.47); MQ135.setB(-2.862); 
  
  radio.stopListening(); /* Rádio para de receber */
  
  bool ok = radio.write( &data, sizeof(data) ); /* Reescreve os novos dados em cima dos antigos */
  MQ135.update(); /* Atualiza os dados do MQ135 */
  radio.startListening(); /* Rádio volta a receber */

}
