#include "Display.h"
#include "ACCtelemetry.h"

void setup(){
    //debug
    Serial.begin(115200);
    
    initializeDisplay();
}

void loop(){
  renderDisplay();
}