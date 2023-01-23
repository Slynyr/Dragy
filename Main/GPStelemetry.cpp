#include <TinyGPSPlus.h>
#include <HardwareSerial.h>


//GPS module pins
#define GPSrx 16
#define GPStx 17
static const uint32_t GPSBaud = 9600;

//telemetry
double speedKMH = 0.f;
double altitudeFT = 0.f;
double lng = 0.f;
double lat = 0.f; 
double accelerationDelta = 0.f;
double lastAccelerationTick = millis();
double lastAccerlationValue = 0.f;
double initMillis = millis();
double accelerationRefreshRate = .5; //seconds
double lastGPSPingMillis = millis();
double gpsTimeout = 15; //seconds
int satelliteCount = 0;

//states
bool isGPSLocked = false;
bool printTelemetry = false; 

//GPS objects
TinyGPSPlus gps;
HardwareSerial hs(1);

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

        if ((millis() - lastAccelerationTick) > (accelerationRefreshRate * 1000)){
            //Serial.println("[NOTIF] Speed Delta has been updated");
            accelerationDelta = speedKMH - lastAccerlationValue;
            lastAccerlationValue = speedKMH;
            lastAccelerationTick = millis();
            Serial.println(accelerationDelta);
            //add G force calculation
        }

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