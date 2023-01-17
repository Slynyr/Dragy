#include "Display.h"
#include "ACCtelemetry.h"
#include "GPStelemetry.h"

void setup(){
    //debug
    Serial.begin(115200);
    initializeDisplay();
    initializeGPS();
}

void loop(){
  renderDisplay();
  GPSmanager();
  ACCtelemetry();
}