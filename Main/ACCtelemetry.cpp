#include "cmath"
#include "Wire.h"
#include "Arduino.h"
//accelorometer pins
//https://forum.arduino.cc/t/calculate-g-force-using-an-accelerometer-gy-521-mpu-6050-arduino-due/248850
//https://lastminuteengineers.com/mpu6050-accel-gyro-arduino-tutorial/

//i2s 
const int MPU = 0x68; 

//telemetry
float vector = 0.f;
float ax = 0.f;
float ay = 0.f;
float az = 0.f;
float gx = 0.f;
float gy = 0.f; 
float gz = 0.f;
float temperature = 0.f;
float heading = 0.f;

int gCounter(){

}

void ACCtelemetry(){
    Wire.beginTransmission(MPU);

    Wire.write(0x3B); //talk to ACCEL register
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 7*2, true); //*

    //retrieving telemetry
    ax = Wire.read()<<8 | Wire.read(); //reading from 2 registers
    ay = Wire.read()<<8 | Wire.read();
    az = Wire.read()<<8 | Wire.read();
    temperature = Wire.read()<<8 | Wire.read();
    gx = Wire.read()<<8 | Wire.read();
    gy = Wire.read()<<8 | Wire.read();
    gz = Wire.read()<<8 | Wire.read();

    //calculations
    vector = sqrt(ax + ay + az*az);
}

void printTelemetry(){
    //CANT PRINTF FLAOTS REASON https://forum.arduino.cc/t/printf-with-floats/332051/9
    //Serial.printf("ax")
    Serial.print("ax: "); Serial.print(ax);
    Serial.print(" | ay: "); Serial.print(ay);
    Serial.print(" | az: "); Serial.print(az);
 
    Serial.print(" | temperature: "); Serial.print(temperature);

    Serial.print(" | gx: "); Serial.print(ax);
    Serial.print(" | gy: "); Serial.print(ay);
    Serial.print(" | gz: "); Serial.print(az);

    Serial.println();
}

void initializeAccelorometer(){
    Wire.begin();
    Wire.beginTransmission(MPU);
    
    //Reset acc
    Wire.write(0x6B); //talk to specified shift register
    Wire.write(0x00); //reset shift register
    Wire.endTransmission(true);

    //config g-force range
    Wire.write(0x1C); //talk to config register
    Wire.write(0x10); //set the register bits as 00010000 (+/- 8g full scale range) 
    Wire.endTransmission(true);

    //config gyro
    Wire.write(0x1B); //talk to config register
    Wire.write(0x10); //set the register bits as 00010000 (1000deg/s full scale)
    Wire.endTransmission(true);

    delay(20);
}

