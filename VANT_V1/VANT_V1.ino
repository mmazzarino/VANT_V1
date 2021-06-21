// pós mvp, lembrar de criar uma forma de saber quanto tempo cada parte do programa está tomando para ser executada, afim de melhorar o tempo

#include "VANT_V1.h"

Modo_de_operacao MOP;
SetPoints SP;
ME_ordem_de_execucao ORDEM_EXECUCAO = AGUARDAR_COMANDOS;

unsigned long pT = 0;
unsigned long cT = 0;
unsigned long dT_us = 0;

void setup() {
    Serial.begin(115200);
    configuraLora();
    configuraIMU(); 
}

unsigned long tt_u = 0;
unsigned long tt_dt = 0;
unsigned long tt_ua = 0;

void loop(){
    switch(ORDEM_EXECUCAO){ 
        case AGUARDAR_COMANDOS:
            while(!recebeuComandosBase()){
                if((recebeuComandosBase()) && (String(packetSize, DEC) == "1")){
                    MOP = configuraOperacao();
                    if(MOP.modoAutonomo){
                        ORDEM_EXECUCAO = PILOTO_AUTOMATICO;
                        break;
                    }else{
                        ORDEM_EXECUCAO = CONTROLE_REMOTO;
                        break;
                    }
                }
            }
        
        case CONTROLE_REMOTO:
            if((recebeuComandosBase()) && (String(packetSize, DEC) == "16")){
                SP = criaSetPoints(converteComandosParaInt(), MOP);
                MOP.grauInicialEixoZ = direcaoDeDecolagem();
            }
            le_IMU();
            calcula_PID();
            break;
        
        case PILOTO_AUTOMATICO: 
             break;
    }
    delay(10);
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
Modo_de_operacao configuraOperacao(){
    Modo_de_operacao MOP_value;
    char comando_op[5] = {' ', ' ', ' ', ' ', '\0'};
    char *p_op = comando_op;
    for(char cont = 0; cont < 5; cont++){
        if(cont < 4){
            comando_op[cont] = (char) LoRa.read();
        }
    }
    switch((uint16_t)atoi(p_op)){
        case 1:
            MOP_value.inclinacaoMinima = -1500;
            MOP_value.inclinacaoMaxima = 1500;
            MOP_value.potenciaMaxima = 247;
            MOP_value.potenciaMinima = 70;
            MOP_value.modoAutonomo = false;
            break;
    }  
    return MOP_value;
}
bool recebeuComandosBase(){
    packetSize = 0;
    packetSize = LoRa.parsePacket(); 
    if(packetSize){
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
      //Serial.println("RSSI " + String(LoRa.packetRssi(), DEC));
      return comandos;   
}
SetPoints criaSetPoints(Comandos_int comandos_int, Modo_de_operacao MOP){ 
    SetPoints sp;
    sp.sp_gx = map(comandos_int.RX, 0, 4095, MOP.inclinacaoMinima, MOP.inclinacaoMaxima); 
    sp.sp_gy = map(comandos_int.RY, 0, 4095, MOP.inclinacaoMinima, MOP.inclinacaoMaxima); 
    sp.sp_potencia = map(comandos_int.LY, 0, 4095, MOP.potenciaMinima, MOP.potenciaMaxima);   
    sp.sp_gz = map(comandos_int.LX, 0, 4095, (MOP.grauInicialEixoZ - 180), (MOP.grauInicialEixoZ + 180));
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
int16_t direcaoDeDecolagem(){ //IMPLEMENTAR
    return 45;
}
void configuraRMT(){

}
void configuraGPS(){

}
void calcula_PID(){
  
}
void montaFrameDshot(){

}
void enviaFrameParaESC(){

}
void pegaGeolocalizacao(){

}
void enviaDadosParaBase(){

}
