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
double acceleration = 0.f;
double lastAccelerationTick = millis();
double lastAccerlationValue = 0.f;
double initMillis = millis();
int accelerationRefreshRate = 1; //seconds

//states
bool isGPSLocked = false;

//GPS objects
TinyGPSPlus gps;
HardwareSerial hs(1);

int position(){
    ;
}

int speed(char* units){
    ;
}

void GPStelemetry(){
    if (gps.location.isValid()){
        if (isGPSLocked == false){
            isGPSLocked = true;
        }

        speedKMH = gps.speed.kmph();
        lat = gps.location.lat();
        lng = gps.location.lng();
        altitudeFT = gps.altitude.feet();

        if ((lastAccelerationTick - initMillis) > (accelerationRefreshRate * 1000)){
            lastAccelerationTick = millis();
            acceleration = speedKMH - lastAccerlationValue;
        }

        Serial.print("Location: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(", ");
        Serial.print(gps.location.lng(), 6);
        Serial.print(" | Speed: ");
        Serial.print(gps.speed.kmph());
        Serial.print("km/h");
        Serial.print(" | Acceleration: ");
        Serial.print(acceleration);
        Serial.print("km/s");
        Serial.println("");        
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
        }
    } 
}