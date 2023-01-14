#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

//GPS module pins
#define GPSrx RX2
#define GPStx TX2
static const uint32_t GPSBaud = 4800;

//GPS objects
TinyGPSPlus gps;
SoftwareSerial ss(GPSrx, GPStx);

int position(){
    ;
}

int speed(char* units){
    ;
}

void GPStelemetry(){
    if (gps.location.isValid()){
        Serial.print("Location: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(", ");
        Serial.print(gps.location,lng(), 6);
        Serial.println("")
    } else {
        Serial.println("[WARN] Invalid gps position")
    }
}

void initializeGPS(){
    ss.begin(GPSBaud);
    Serial.println("[STATUS] GPS initialized");
}

void GPSmanager(){
    if (ss.available() > 0){
        if (gps.encode(ss.read())){
            GPStelemetry();
        } else {
            Serial.println("[STATUS] GPS is aligning");
        }
    }
}