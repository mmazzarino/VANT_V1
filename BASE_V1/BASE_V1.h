#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  

//Pinos do ESP32 TTGO T-beam V1.1
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
