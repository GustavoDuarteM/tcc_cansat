#include<Wire.h>
 
//Endereco I2C do MPU6050
const int MPU=0x68;  //pino aberto 0X68 , pino ligado em 3,3V 0x69

//Variaveis globais
int acelX,acelY,acelZ,temperatura,giroX,giroY,giroZ;

//configurações iniciais
void setup()
{

  Serial.begin(9600);     //inicia a comunicação serial
  Wire.begin();                 //inicia I2C
  Wire.beginTransmission(MPU);  //Inicia transmissão para o endereço do MPU
  Wire.write(0x6B);             
   
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
}

//loop principal
void loop()
{
  Wire.beginTransmission(MPU);      //transmite
  Wire.write(0x3B);                 // Endereço 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);     //Finaliza transmissão
  
  Wire.requestFrom(MPU,14,true);   //requisita bytes
   
  //Armazena o valor dos sensores nas variaveis correspondentes
  acelX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  acelY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  acelZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  temperatura=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  giroX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  giroY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  giroZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
   
  //Envia valores lidos do acelerômetro
  Serial.print("Acel:"); 
  Serial.print("  X:");Serial.print(acelX);
  Serial.print("\tY:");Serial.print(acelY);
  Serial.print("\tZ:");Serial.print(acelZ);
  
  //Envia valores lidos do giroscópio
  Serial.print("\tGiro:"); 
  Serial.print("  X:");Serial.print(giroX);
  Serial.print("\tY:");Serial.print(giroY);
  Serial.print("\tZ:");Serial.print(giroZ);
   
  //Envia valor da temperatura em graus Celsius
  Serial.print("\tTemperatura: ");    Serial.println(temperatura/340.00+36.53);
   
  //Aguarda 500 ms
  delay(500);
}
