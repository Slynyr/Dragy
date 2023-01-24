#ifndef _GPSTELEMETRY_H_
#define _GPSTELEMETRY_H_

//telemetry
extern double speedKMH;
extern double altitudeFT;
extern double lng;
extern double lat;

extern double accelerationDelta;
extern int satelliteCount;
extern double totalDistance;

//states 
extern bool isGPSLocked;

void GPSmanager();
//main GPS loop

void initializeGPS();
//initializes GPS module

void GPStelemetry();
//decodes telemetry data from the GPS module


#endif