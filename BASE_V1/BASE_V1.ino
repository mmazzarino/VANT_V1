#include "BASE_V1.h"

void setup() {
  Serial.begin(115200);
  configuraLora();
}

void loop() {
  //testaEnvioLora();
  //recebeComandosJoystick();
  enviaComandosVant();
  delay(100);
}

void recebeComandosJoystick(){
  valor_RX = analogRead(ADC_RX);
  valor_RY = analogRead(ADC_RY);
  valor_LX = analogRead(ADC_LX);
  valor_LY = analogRead(ADC_LY);
}

void enviaComandosVant(){
  valor_RX = 100;
  valor_RY = 2045;
  valor_LX = 244;
  valor_LY = 4094;
  
  char valor_RX_f[5] = {' ', ' ', ' ', ' ', '\0'};
  char valor_RY_f[5] = {' ', ' ', ' ', ' ', '\0'};
  char valor_LX_f[5] = {' ', ' ', ' ', ' ', '\0'};
  char valor_LY_f[5] = {' ', ' ', ' ', ' ', '\0'};
  
  sprintf (valor_RX_f, "%04d", valor_RX);
  sprintf (valor_RY_f, "%04d", valor_RY);
  sprintf (valor_LX_f, "%04d", valor_LX);
  sprintf (valor_LY_f, "%04d", valor_LY);

  Serial.println(valor_RX_f);

  LoRa.beginPacket();  
  LoRa.print(valor_RX_f);
  LoRa.print(valor_RY_f); 
  LoRa.print(valor_LX_f);
  LoRa.print(valor_LY_f);
  LoRa.endPacket();
}

void configuraLora(){
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("init ok");
  delay(1500);
}

/*
void testaEnvioLora(){
  Serial.println("Sending packet: ");
  Serial.println(String(counter));
  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);             
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);   
}
*/
