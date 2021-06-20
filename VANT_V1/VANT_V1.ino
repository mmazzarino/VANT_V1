#include "VANT_V1.h"

Modo_de_operacao MOP;
SetPoints SP;

void setup() {
  Serial.begin(115200);
  configuraLora();
  configuraIMU();
  
}

void loop(){
  if(recebeuComandosBase()){
    SetPoints SP = criaSetPoints(converteComandosParaInt(), MOP);
  }
  
  delay(100);
}

/*void configuraProcessadores(){ 
  xTaskCreatePinnedToCore(loopSegundoCore,     //Função para implementar a tarefa
                          "TarefaSegundoCore", //Nome da tarefa
                          10000,               //Tamanho da pilha em palavras
                          NULL,                //Parâmetro de entrada de tarefa
                          1,                   //Prioridade da tarefa
                          &TarefaSegundoCore,  //Identificador de tarefa
                          0);                  //Núcleo onde a tarefa deve ser executada            
}
*/
void loopSegundoCore(){
  
}

bool recebeuComandosBase(){
  packetSize = 0;
  packetSize = LoRa.parsePacket(); 
  if(String(packetSize,DEC) == "16"){ //se recebeu 16 bytes
    return true;
  }else{
    return false;
  }
}

Comandos_int converteComandosParaInt(){  
  Comandos_int comandos;   
  uint16_t comandos_aux[4];
  char comando[5] = {' ', ' ', ' ', ' ', '\0'};
  char *p = comando;
  for(char numComando = 0; numComando < 4; numComando++){
    for(char cont = 0; cont < 5; cont++){
      if(cont < 4){
        comando[cont] = (char) LoRa.read();
      }
    }
    comandos_aux[numComando] = (uint16_t)atoi(p);
  }
  comandos.RX = comandos_aux[0];
  comandos.RY = comandos_aux[1];
  comandos.LX = comandos_aux[2];
  comandos.LY = comandos_aux[3]; 
  Serial.println("RSSI " + String(LoRa.packetRssi(), DEC));
  return comandos;
}

SetPoints criaSetPoints(Comandos_int comandos_int, Modo_de_operacao MOP){ 
  SetPoints sp;
  sp.sp_gx = map(comandos_int.RX, 0, 4095, MOP.inclinacaoMinima, MOP.inclinacaoMaxima); 
  sp.sp_gy = map(comandos_int.RY, 0, 4095, MOP.inclinacaoMinima, MOP.inclinacaoMaxima); 
  sp.sp_potencia = map(comandos_int.LY, 0, 4095, MOP.potenciaMinima, MOP.potenciaMaxima);   
  spP.sp_gz = map(comandos_int.LX, 0, 4095, (MOP.grauInicialEixoZ - 180), (MOP.grauInicialEixoZ + 180));
  return sp;  
}

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

  
  MOP.grauInicialEixoZ = direcaoDeDecolagem();
}

int16_t direcaoDeDecolagem(){
  return 45;
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
