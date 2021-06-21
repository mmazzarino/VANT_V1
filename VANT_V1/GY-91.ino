// Métodos relacionados ao GY-91.
// Obs. 1: o GY-91 contém os principais e mais importantes sensores do VANT, por isso os métodos devem ser relativamente otimizados desde a V1 
// Obs. 2: "Communication with all registers of the device is performed using either I2C at 400kHz or SPI at 1MHz. For
//          applications requiring faster communications, the sensor and interrupt registers may be read using SPI at 20MHz"
// Datasheet MPU9250: https://invensense.tdk.com/wp-content/uploads/2015/02/PS-MPU-9250A-01-v1.1.pdf
// Obs. 3: esse git parece ser bom: https://github.com/kriswiner/MPU9250/blob/master/MPU9250_MS5637_AHRS_t3.ino

void configuraIMU(){
    Wire.begin();
    escreve_byte(MPU9250_ADDRESS, PWR_MGMT_1, 0b10000000);       // Reseta os registros internos e restaura as configurações padrão.
    delay(100);
    escreve_byte(MPU9250_ADDRESS, PWR_MGMT_2, 0x00);             // Ativa todos sensores
    escreve_byte(MPU9250_ADDRESS, INT_PIN_CFG, 0x02);            // Ativa o desvio para acessar o magnetômetro
    escreve_byte(AK8963_ADDRESS, AK8963_CNTL, 0x00);             // Le os dados de ajuste de fábrica
    delay(10);
    escreve_byte(AK8963_ADDRESS, AK8963_CNTL, 0x0F);             // Entra no modo de acesso à ROM do fusível
    delay(10);
    le_bytes(AK8963_ADDRESS, AK8963_ASAX, 3);                    // Le os valores de calibração dos eixos x, y e z
    MagAdjustment[0] =  (float)(Wire.read() - 128)/256. + 1.;    // Retornar valores de ajuste de sensibilidade do eixo x, etc.
    MagAdjustment[1] =  (float)(Wire.read() - 128)/256. + 1.;
    MagAdjustment[2] =  (float)(Wire.read() - 128)/256. + 1.;
    escreve_byte(AK8963_ADDRESS, AK8963_CNTL, 0x00);             //Desligue o magnetômetro
    delay(10);
    escreve_byte(AK8963_ADDRESS, AK8963_CNTL, 0b00010110);       // Definir a resolução de dados do magnetômetro e obter amostra de ODR
    delay(10);
    set_accel_range(RANGE_2G);
    set_gyro_range(RANGE_GYRO_2000); 
    
}

void le_IMU(){
    gx4tst = 0.0;
    gy4tst = 0.0;
    gz4tst = 0.0;   
    for(int cont = 0; cont < 5; cont++){    
        cT = micros();             
        le_bytes(MPU9250_ADDRESS, ACCEL_XOUT_H, 14);
        ay1 = (Wire.read()<<8)|(Wire.read()); 
        ax1 = (Wire.read()<<8)|(Wire.read());  
        az1 = (Wire.read()<<8)|(Wire.read());    
        Tmp = (Wire.read()<<8)|(Wire.read());
        gy1 = (Wire.read()<<8)|(Wire.read());  
        gx1 = (Wire.read()<<8)|(Wire.read());  
        gz1 = (Wire.read()<<8)|(Wire.read());        
        filtroComplementar();              
        gx4tst += gx4;
        gy4tst += gy4;
        gz4tst += gz4;
    }   
    gx4tst /= 5;
    gy4tst /= 5;
    gz4tst /= 5;
    Tmp = (float)((Tmp/333.87) + 21.0);
}

void filtroComplementar(){
    ax2 = ((ax1 * GRAVI)/CALIB);  
    ay2 = ((ay1 * GRAVI)/CALIB);
    az2 = ((az1 * GRAVI)/CALIB);
    
    //Converte valor do acelerometro com base nos 3 eixos
    ax3 = (atan2(ax1, sqrt(pow(ay1, 2) + pow(az1, 2))))* radToDegr;  
    ay3 = (atan2(ay1, sqrt(pow(ax1, 2) + pow(az1, 2))))* radToDegr; 
    az3 = (atan2(az1, sqrt(pow(ax1, 2) + pow(ay1, 2))))* radToDegr; 
    
    // Converte valor do giro em graus por seg multiplicando uma contante relacionada à taxa de amostragem do sensor
    gr_por_seg_x = gx1 * SENSE_GYRO_2000; 
    gr_por_seg_y = gy1 * SENSE_GYRO_2000; 
    gr_por_seg_z = gz1 * SENSE_GYRO_2000; 
    
    dT_us = cT - pT;
    pT = cT;
    
    // Fusão dos dados: giro + accel
    gx4 = ((0.7 *(gx4 + (gr_por_seg_x * (dT_us/1000000)))) + (0.3 * ax3));
    gy4 = ((0.7 *(gy4 + (gr_por_seg_y * (dT_us/1000000)))) + (0.3 * ay3));
    gz4 = ((0.7 *(gz4 + (gr_por_seg_z * (dT_us/1000000)))) + (0.3 * az3));    
}

void set_accel_range(accel_range range){
    uint8_t reg = le_byte(MPU9250_ADDRESS, ACCEL_CONFIG);
    escreve_byte(MPU9250_ADDRESS, ACCEL_CONFIG, ((reg & 0b11100111)|(range<<3)));
}

void set_gyro_range(gyro_range range){
    uint8_t reg = le_byte(MPU9250_ADDRESS, GYRO_CONFIG);
    escreve_byte(MPU9250_ADDRESS, GYRO_CONFIG, ((reg & 0b11100111)|(range<<3)));
}

void escreve_byte(uint8_t adress, uint8_t reg, uint8_t value){
    Wire.beginTransmission(adress);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t le_byte(uint8_t adress, uint8_t reg){
    Wire.beginTransmission(adress);
    Wire.write(reg);
    Wire.endTransmission(false); 
    Wire.requestFrom(adress, 1);   
    return Wire.read();
}
void le_bytes(uint8_t adress, uint8_t reg, int numBytes){
    Wire.beginTransmission(adress);
    Wire.write(reg);
    Wire.endTransmission(false); 
    Wire.requestFrom(adress, numBytes);   
}
