#include <MQUnifiedsensor.h>

/* Definições da MQ135 */
#define RatioMQ135CleanAir 3.6 
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0
#define type "MQ-135"
#define ADC_Bit_Resolution 10
#define RatioMQ135CleanAir 3.6

/* Declarando bmp e MQ135 */
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup()
{
  Serial.begin(9600); /* Na velocidade 9600 no Monitor Serial */
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

void loop(void)
{ 
  double CO = MQ135.readSensor(); MQ135.setA(605.18); MQ135.setB(-3.937);
  double Alcohol = MQ135.readSensor(); MQ135.setA(77.255); MQ135.setB(-3.18);
  double NH4 = MQ135.readSensor(); MQ135.setA(102.2 ); MQ135.setB(-2.473);
  double Acetona = MQ135.readSensor(); MQ135.setA(34.668); MQ135.setB(-3.369);
  double CO2 = MQ135.readSensor(); MQ135.setA(110.47); MQ135.setB(-2.862);

  Serial.print("CO :");
  Serial.println(CO);
  Serial.print("Alcohol :");
  Serial.println(Alcohol);
  Serial.print("NH4 :");
  Serial.println(NH4);
  Serial.print("Acetona :");
  Serial.println(Acetona);
  Serial.print("CO2 :");
  Serial.println(CO2);
  
  MQ135.update(); /* Atualiza os dados do MQ135 */
  delay(2000);
}
