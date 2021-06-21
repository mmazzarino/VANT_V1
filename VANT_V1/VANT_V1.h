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
float gx4tst = 0;
float gy4tst = 0;
float gz4tst = 0;
float gz_ini = 0; 
float radToDegr = 57.295795;
int16_t Tmp = 0;  

//------------------GY-91------------------

#define MPU9250_ADDRESS  0x68  // Change to 0x69 if AD0 is in high state
#define PWR_MGMT_1       0x6B
#define PWR_MGMT_2       0x6C
#define INT_PIN_CFG      0x37
#define AK8963_ADDRESS   0x0C  // MPU9250 must bypass to access the AK8963 in INT_PIN_CFG
#define AK8963_CNTL      0x0A  // Power down (0000), Continuous measurement mode 1 (0010), CMM2 (0110), Fuse ROM (1111) on bits 3:0
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define ACCEL_XOUT_H     0x3B  // 1º dado
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C
float MagAdjustment[3] = {0, 0, 0};
float CALIB = 16071.82;
float GRAVI = 9.81;
float AJUSTE = 1.69;
float G_GAIN = 0.0164;//0.00875;
float SENSE_GYRO_2000 = 0.0164;
float SENSE_GYRO_1000 = 0.0328;
float SENSE_GYRO_500 = 0.0655;
float SENSE_GYRO_250 = 0.131;

typedef enum{
  RANGE_16G          = 0b11,
  RANGE_8G           = 0b10,
  RANGE_4G           = 0b01,
  RANGE_2G           = 0b00
} accel_range;
typedef enum{
  RANGE_GYRO_2000    = 0b11,
  RANGE_GYRO_1000    = 0b10,
  RANGE_GYRO_500     = 0b01,
  RANGE_GYRO_250     = 0b00
} gyro_range;
typedef enum{
  SCALE_14_BITS      = 0,
  SCALE_16_BITS      = 1
} mag_scale;
typedef enum{
  MAG_8_Hz           = 0,
  MAG_100_Hz         = 1
} mag_speed;

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
  bool modoAutonomo = false;
} Modo_de_operacao;


typedef struct {
  int32_t sp_gx = 0;           //Setpoint grau do angulo eixo x 
  int32_t sp_gy = 0;           //Setpoint grau do angulo eixo y 
  uint32_t sp_potencia = 0;    //Setpoint potencia          
  int32_t sp_gz = 0;           //Setpoint grau do angulo eixo z 
} SetPoints;

typedef enum {
  AGUARDAR_COMANDOS,
  CONTROLE_REMOTO,
  PILOTO_AUTOMATICO
} ME_ordem_de_execucao;

/*
typedef enum {
  AGUARDAR_COMANDOS,
  VOAR
} ME_ordem_de_execucao;*/
