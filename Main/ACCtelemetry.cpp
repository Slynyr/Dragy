#include "cmath"
#include "Wire.h"
#include "Arduino.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
//accelorometer pins
//https://forum.arduino.cc/t/calculate-g-force-using-an-accelerometer-gy-521-mpu-6050-arduino-due/248850
//https://lastminuteengineers.com/mpu6050-accel-gyro-arduino-tutorial/

//i2s 
Adafruit_MPU6050 mpu;

//telemetry
float vector = 0.f;
float ax = 0.f;
float ay = 0.f;
float az = 0.f;
float gx = 0.f;
float gy = 0.f; 
float gz = 0.f;
float tempRead = 0.f;
float heading = 0.f;

int gCounter(){

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

void ACCtelemetry(){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    //acceleration
    ax = a.acceleration.x;
    ay = a.acceleration.y;
    az = a.acceleration.z;

    //gyro
    gx = g.acceleration.x;
    gy = g.acceleration.y;
    gz = g.acceleration.z;

    //temp
    tempRead = temp.temperature;

    //calculations
    vector = sqrt(ax + ay + az*az);

    printTelemetry();
}

void initializeAccelorometer(){
    if (!mpu.begin()){
        Serial.println("[WARN] FAILED to find MPU6050");
    } else {
        Serial.println("[NOTIF] MPU6050 has initialized");
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

}

