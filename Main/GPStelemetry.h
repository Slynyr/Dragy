#ifndef _GPSTELEMETRY_H_
#define _GPSTELEMETRY_H_

//telemetry
extern float speedKMH;
extern float altitudeFT;
extern float lng;
extern float lat;

//states 
extern bool isGPSLocked;

void GPSmanager();
//main GPS loop

void initializeGPS();
//initializes GPS module

void GPStelemetry();
//decodes telemetry data from the GPS module


#endif