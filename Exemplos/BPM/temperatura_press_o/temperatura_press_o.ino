#include <Wire.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_BMP085.h> //INCLUSÃO DE BIBLIOTECA
 
Adafruit_BMP085 bmp; //OBJETO DO TIPO Adafruit_BMP085 (I2C)
   
void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  if (!bmp.begin()){ //SE O SENSOR NÃO FOR INICIALIZADO, FAZ
  Serial.println("Sensor BMP180 não foi identificado! Verifique as conexões."); //IMPRIME O TEXTO NO MONITOR SERIAL
  while(1){} //SEMPRE ENTRE NO LOOP
  }
}
   
void loop(){
    
    Serial.print("Temperatura: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(bmp.readTemperature()); //IMPRIME NO MONITOR SERIAL A TEMPERATURA
    Serial.println(" *C (Grau Celsius)"); //IMPRIME O TEXTO NO MONITOR SERIAL
        
    Serial.print("Pressão: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(bmp.readPressure()); //IMPRIME NO MONITOR SERIAL A PRESSÃO
    Serial.println(" Pa (Pascal)"); //IMPRIME O TEXTO NO MONITOR SERIAL
     
    Serial.print("Altitude: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(bmp.readAltitude()); //IMPRIME NO MONITOR SERIAL A ALTITUDE APROXIMADA
    Serial.println(" m (Metros)"); //IMPRIME O TEXTO NO MONITOR SERIAL
    
    Serial.print("Pressão a nível do mar (calculada): "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(bmp.readSealevelPressure()); //IMPRIME NO MONITOR SERIAL A PRESSÃO A NÍVEL DO MAR
    Serial.println(" Pa (Pascal)"); //IMPRIME O TEXTO NO MONITOR SERIAL

    Serial.print("Altitude real: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(bmp.readAltitude(101500)); //IMPRIME NO MONITOR SERIAL A ALTITUDE REAL
    Serial.println(" m (Metros)"); //IMPRIME O TEXTO NO MONITOR SERIAL
     
    Serial.println("-----------------------------------"); //IMPRIME UMA LINHA NO MONITOR SERIAL
    delay(2000); //INTERVALO DE 2 SEGUNDOS
}
