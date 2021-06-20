#include <stdbool.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  


//void int[4] converteComandosParaInt();
//------------------Taregas------------------incl
//TaskHandle_t TarefaSegundoCore;

//------------------LoRa------------------
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     23   // GPIO23 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;
int packetSize = 0;
int valor_RX = 2048;
int valor_RY = 2048;
int valor_LX = 2048;
int valor_LY = 2048;



 

//------------------Motores------------------
int potenciaMaxima = 247;
int potenciaMinima = 70;

//------------------Acelerômetro------------------
int16_t ax1;
int16_t ay1;
int16_t az1;
float ax2 = 0;
float ay2 = 0;
float az2 = 0;
float ax3 = 0;
float ay3 = 0;
float az3 = 0;

//------------------Giroscópio------------------
int16_t gx1;
int16_t gy1;
int16_t gz1;
float gr_por_seg_x = 0;
float gr_por_seg_y = 0;
float gr_por_seg_z = 0;
float gx3 = 0;
float gy3 = 0;
float gz3 = 0;
float gx4 = 0;
float gy4 = 0;
float gz4 = 0;
float gz_ini = 0; 
float radToDegr = 57.295795;

typedef struct {
  uint16_t RX = 0;
  uint16_t RY = 0;
  uint16_t LX = 0;
  uint16_t LY = 0;
} Comandos_int;


typedef struct {
  int16_t inclinacaoMinima = -15 * 100; //inclinação em graus * 100 para aumentar a resolução, já que a função map retorna int
  int16_t inclinacaoMaxima = 15 * 100;  
  uint16_t potenciaMaxima = 247;
  uint16_t potenciaMinima = 70;
  int16_t grauInicialEixoZ = 0;
} Modo_de_operacao;


typedef struct {
  int32_t sp_gx = 0;           //Setpoint grau do angulo eixo x 
  int32_t sp_gy = 0;           //Setpoint grau do angulo eixo y 
  uint32_t sp_potencia = 0;    //Setpoint potencia          
  int32_t sp_gz = 0;           //Setpoint grau do angulo eixo z 
} SetPoints;

enum ESTADOS{
  AGUARDANDO_DECOLAGEM,
  SETPOINTS,
  
  
}
