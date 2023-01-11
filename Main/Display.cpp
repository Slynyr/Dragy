#include <TFT_eSPI.h>
#include <SPI.h>

//Display pins
#define MOSI 23
#define SCK 18
#define CS 15
#define RESET 4
#define dc 2
#define BACKGROUNDCOLOR TFT_BLACK
TFT_eSPI tft=TFT_eSPI;

//state
char* state = "main";


void initializeDisplay(){
    tft.init();
    tft.fillscreen(BACKGROUNDCOLOR);
}

void drawText(char* text, int x, int y, int size){
    tft.setTextSize(size);
    tft.drawString(text, x, y);
}

void debug(){
    drawText("test", 100, 100, 5);
}

void renderDisplay(){
    if (state == "main"){
        debug();
    }
}