#include <TFT_eSPI.h>
#include <SPI.h>
#include "GPStelemetry.h"
#include <string>
#include "satSprites.h"

//Display pins
#define MOSI 23
#define SCK 18
#define CS 15
#define RESET 4
#define dc 2

//ui
char speedBuffer[1023] = {0};
int velocityBarMultiplier = 50;
int gCounterBarPos[] = {240, 280};
double lastspeedDelta = accelerationDelta; //optimization
double lastspeed = speedKMH; //change to 0**
int lastSatelliteCount = 0;

//colors
//#define BACKGROUNDCOLOR 0x0842
#define BACKGROUNDCOLOR 0x0000
#define DARKBLUE 0x104D
#define RED 0xF203
#define GREEN 0x07F1
//http://www.rinkydinkelectronics.com/calc_rgb565.php
//#define BACKGROUNDCOLOR TFT_BLACK

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite smallGreenSat = TFT_eSprite(&tft);

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

void drawInt(const int varIn, int x, int y, int size, bool isCentered){
    tft.setTextSize(size);
    tft.setTextColor(TFT_WHITE, BACKGROUNDCOLOR);
    tft.drawNumber(varIn, x, y, size);
}

void alignmentGrid(){
    tft.drawLine(0, 160, 480, 160, RED);
    tft.drawLine(240, 0, 240, 320, RED);
}
//----MAIN
void aligningGPS(){
    drawText("NO GPS LOCK", 10, 160, 10, false);
}

void guageClusterBareBones(float speed){
    //speed = 112; //DEBUG LINE, REMOVE
    //alignmentGrid();

    if (speed != lastspeed){
        //int wholeSpeed = (int)speed;
        if (speed < 10){
            //drawText(String(speed, 0).c_str(), 230, 150, 5, false);
            drawText(String(speed, 0).c_str(), 200, 150, 5, false);
            //drawInt(wholeSpeed, 230, 150, 5, false);
        } else if (speed >= 10 && speed < 100) {
            drawText(String(speed, 0).c_str(), 215, 150, 5, false);
            //drawInt(wholeSpeed, 230, 150, 5, false);
        } else if (speed >= 100 && speed < 1000) {
            drawText(String(speed, 0).c_str(), 200, 150, 5, false);
            //drawInt(wholeSpeed, 230, 150, 5, false);
        } else {
            drawText("You might want to slow down", 0, 150, 5, false);
        }

        drawText("km/h", 218, 195, 2, false);
    }
    lastspeed = speed; 
}

void gIndicatorBasic(float speedDelta){
    //speedDelta = 0.75; //DEBUG LINE, REMOVE
    //speedDelta = 5; //DEBUG LINE, REMOVE

    int pixelCount = speedDelta * velocityBarMultiplier;
    if (pixelCount > 220) {
        pixelCount = 220;
    } else if (pixelCount < -220){
        pixelCount = -220;
    }

    if (speedDelta != lastspeedDelta){
        tft.fillRect(20, gCounterBarPos[1], 460, 20, BACKGROUNDCOLOR);
        tft.fillRect(gCounterBarPos[0], gCounterBarPos[1], pixelCount, 20, RED);
    } 
    lastspeedDelta = speedDelta; //attempt at optimization, reducing the load on the renderer.
}

void satelliteIndicator(int satCount){
    if (satCount != lastSatelliteCount){
        if (satCount < 6){
            Serial.println("I am being Drawn!");
            tft.setTextSize(2);
            tft.setTextColor(RED, BACKGROUNDCOLOR);
            tft.drawString(String(satCount).c_str(), 5, 3);
        } else {
            Serial.println("I am being Drawn!");
            tft.setTextSize(2);
            tft.setTextColor(GREEN, BACKGROUNDCOLOR);
            tft.drawString(String(satCount).c_str(), 5, 3);

            smallGreenSat.pushSprite(40, 40, TFT_WHITE);
        }

        tft.fillCircle(10, 10, 10, RED);
    }
    lastSatelliteCount = satCount;
}

void externManager(){
    if (!isGPSLocked && state != "aligningGPS"){
        tft.fillScreen(BACKGROUNDCOLOR);
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
        gIndicatorBasic(accelerationDelta);
        satelliteIndicator(satelliteCount);
        //debug();    
    } else if (state == "aligningGPS"){
        aligningGPS();
    }
}