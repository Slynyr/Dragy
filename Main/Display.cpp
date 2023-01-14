#include <TFT_eSPI.h>
#include <SPI.h>

//Display pins
#define MOSI 23
#define SCK 18
#define CS 15
#define RESET 4
#define dc 2

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
    tft.setRotation(-1);
    tft.fillScreen(BACKGROUNDCOLOR);
}

//----TOOLS
void drawText(char* text, int x, int y, int size){
    tft.setTextSize(size);
    tft.drawString(text, x, y);
}

//----MAIN
void aligningGPS(){
    
}

void debug(){
    drawText("test", 0, 0, 5);
}

void renderDisplay(){
    if (state == "main"){
        debug();    
        Serial.println(tft.width());
        Serial.println(tft.height());
    }
}