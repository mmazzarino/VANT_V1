#include "BASE_V1.h"

void setup() {
  Serial.begin(115200);
  configuraLora();
}

void loop() {

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
