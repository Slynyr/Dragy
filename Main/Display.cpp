#include <TFT_eSPI.h>
#include <SPI.h>
#include "GPStelemetry.h"
#include <string>

//Display pins
#define MOSI 23
#define SCK 18
#define CS 15
#define RESET 4
#define dc 2

//ui
char speedBuffer[1023] = {0};

//colors
#define BACKGROUNDCOLOR 0x0842
#define DARKBLUE 0x104D
//http://www.rinkydinkelectronics.com/calc_rgb565.php
//#define BACKGROUNDCOLOR TFT_BLACK

TFT_eSPI tft = TFT_eSPI();

//state
char* state = "main";

//----INIT
void initializeDisplay(){
    tft.init(); 
    tft.setRotation(3);
    tft.fillScreen(BACKGROUNDCOLOR);
}

//----TOOLS
void drawText(const char* text, int x, int y, int size, bool isCentered){
    tft.setTextSize(size);
    tft.setTextColor(TFT_WHITE, BACKGROUNDCOLOR);
    if (isCentered){
      //tft.setTextDatum(4);
      tft.drawString(text, x, y, size);
    } else {
      //tft.setTextDatum(0);
      tft.drawString(text, x, y);
    }
}

//----MAIN
void aligningGPS(){
    drawText("NO GPS LOCK", 10, 160, 10, false);
}

void guageClusterBareBones(float speed){
  //String(speed, 2).c_str()
  //sprintf(speedBuffer, "%.0f", speed);
  drawText(String(speed, 0).c_str(), 240, 160, 5, false);
}

void externManager(){
    if (!isGPSLocked && state != "aligningGPS"){
        state = "aligningGPS";
    } else if (isGPSLocked && state != "main"){
      tft.fillScreen(BACKGROUNDCOLOR);
      state = "main";
    }
}


void debug(){
    //drawText("test", 0, 0, 5);
}

void renderDisplay(){
    externManager();
    if (state == "main"){
        guageClusterBareBones(speedKMH);
        //debug();    
    } else if (state == "aligningGPS"){
        aligningGPS();
    }
}