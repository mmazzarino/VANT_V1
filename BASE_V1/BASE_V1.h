#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  

// Definições LoRa
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     23   // GPIO23 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    915E6
unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

// Definições Joystick
#define ADC_RX 15
#define ADC_RY 35
#define ADC_LX 32
#define ADC_LY 33
uint16_t valor_RX = 2048;
uint16_t valor_RY = 2048;
uint16_t valor_LX = 2048;
uint16_t valor_LY = 2048;
