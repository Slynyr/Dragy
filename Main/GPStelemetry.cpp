#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <cmath>
#include <math.h>

//GPS module pins
#define GPSrx 16
#define GPStx 17
static const uint32_t GPSBaud = 9600;

//telemetry
double speedKMH = 0.f;
double altitudeFT = 0.f;
double lng = 0.f;
double lat = 0.f; 
double lastLng = 0.f;
double lastLat = 0.f; 
double accelerationDelta = 0.f;
double lastAccelerationTick = millis();
double lastAccerlationValue = 0.f;
double initMillis = millis();
double accelerationRefreshRate = .5; //seconds
double lastGPSPingMillis = millis();
double lastMileageUpdate = millis();
double mileageUpdateTime = .5; //seconds
double gpsTimeout = 15; //seconds
int satelliteCount = 0;
double totalDistance = 0.f;

//states
bool isGPSLocked = false;
bool printTelemetry = true; 

//GPS objects
TinyGPSPlus gps;
HardwareSerial hs(1);

//calculate distance in km between 2 points
double toRad(double degree) {
    return degree/180 * M_PI;
}

double calculateDistance(double lat1, double long1, double lat2, double long2) {
    double dist;
    dist = sin(toRad(lat1)) * sin(toRad(lat2)) + cos(toRad(lat1)) * cos(toRad(lat2)) * cos(toRad(long1 - long2));
    dist = acos(dist);
    dist = 6371 * dist;
    return dist;
}

//telemetry
void GPStelemetry(){
    if (gps.location.isValid()){
        if (isGPSLocked == false){
            isGPSLocked = true;
        }

        speedKMH = gps.speed.kmph();
        lat = gps.location.lat();
        lng = gps.location.lng();
        altitudeFT = gps.altitude.feet();
        satelliteCount = gps.satellites.value();

        //calculating acceleration
        if ((millis() - lastAccelerationTick) > (accelerationRefreshRate * 1000)){
            //Serial.println("[NOTIF] Speed Delta has been updated");
            accelerationDelta = speedKMH - lastAccerlationValue;
            lastAccerlationValue = speedKMH;
            lastAccelerationTick = millis();
            //Serial.println(accelerationDelta);
            //add G force calculation
        }

        //mileage counter
        if (lastLat == 0 && lastLng == 0){ //setting first pos
            lastLat = lat;
            lastLng = lng;
        }

        totalDistance += calculateDistance(lat, lng, lastLat, lastLng);//mileage calculation
        lastLat = lat;
        lastLng = lng;

        if (printTelemetry){
            Serial.print("Satellites: ");
            Serial.print(satelliteCount);
            Serial.print(" | Location: ");
            Serial.print(gps.location.lat(), 6);
            Serial.print(", ");
            Serial.print(gps.location.lng(), 6);
            Serial.print(" | Speed: ");
            Serial.print(gps.speed.kmph());
            Serial.print("km/h");
            Serial.print(" | Acceleration-Delta: ");
            Serial.print(accelerationDelta);
            Serial.print("km/h");
            Serial.print(" | Total-Distance: ");
            Serial.print(totalDistance);
            Serial.print("km");
            Serial.println(""); 
        }       
    }
}

void initializeGPS(){
    //initializing pins
    pinMode(GPSrx, INPUT);
    pinMode(GPStx, OUTPUT);

    hs.begin(9600, SERIAL_8N1, GPSrx, GPStx);
    Serial.println("[STATUS] GPS initialized");
}

void GPSmanager(){
    if (hs.available() > 0){
        if (gps.encode(hs.read())){
            GPStelemetry();
            lastGPSPingMillis = millis();
        } else {
            if (((millis() - lastGPSPingMillis)) > (gpsTimeout * 1000)){
                if (isGPSLocked) {
                    isGPSLocked = false; 
                    Serial.println("[WARN] GPS has timed out");
                }
            }
        }
    } 
}