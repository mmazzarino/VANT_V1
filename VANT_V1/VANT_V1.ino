#include "VANT_V1.h"

void setup() {
  Serial.begin(115200);
  configuraLora();

}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    cbk(packetSize);  
  }
  delay(10);
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { 
    packet += (char) LoRa.read(); 
  }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  loraData();
}

void loraData(){
  Serial.println("Received "+ packSize + " bytes");
  Serial.println(packet);
  Serial.println(rssi); 
}

/*
void loopSegundoCore(){

}

void configuraProcessadores(){ 
  xTaskCreatePinnedToCore(loopSegundoCore,     //Função para implementar a tarefa
                          "TarefaSegundoCore", //Nome da tarefa
                          10000,               //Tamanho da pilha em palavras
                          NULL,                //Parâmetro de entrada de tarefa
                          1,                   //Prioridade da tarefa
                          &TarefaSegundoCore,  //Identificador de tarefa
                          0);                  //Núcleo onde a tarefa deve ser executada            
}
*/

void configuraLora(){
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);  
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.receive();
  Serial.println("init ok");
  delay(1500);
}

void configuraIMU(){

}

void configuraRMT(){

}

void configuraGPS(){

}

void recebeDadosDaBase(){

}

void defineEmpuxoDeModoComum(){

}


void defineAnguloDosEixos(){

}


void pegaDadosDoIMU(){

}

void filtraDadosDoIMU(){

}

void calcula_PID_dosEixos(){

}

void calculaEmpuxoDeModoDiferencial(){

}

void totalizaEmpuxosIndividuais(){

}

void montaFrameDshot(){

}

void enviaFrameParaESC(){

}

void pegaGeolocalizacao(){

}

void enviaDadosParaBase(){

}

void loopSegundoCore(){

}
