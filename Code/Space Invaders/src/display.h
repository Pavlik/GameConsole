#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <sprites.h>
/*
Почему если objects.h писать тут, то происходит перобъявление, а если в display.cpp то все норм?
*/
#include <objects.h>

#define SHIP_STEP        10
#define BULLET_SPEED     3.5

#define INWADERS_SPEED   1
#define CHANGE_GRADE     0.15
#define INWADERS_DESCENT 2.5

#define MONSTERS         18
#define COLOMNS          6
#define ROWS             3
#define MONSTERS_IN_ROW  6
#define SHOT_FREQUENCY   30  // 1 second ~ 60



#define SCREEN_WIDTH     128     // OLED display width, in pixels
#define SCREEN_HEIGHT    64      // OLED display height, in pixels
#define OLED_RESET       -1      // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS   0x3C    // I2C adrress

void update();
void clear();

void myDisplaySetup();
void drawScreen();
void drawing(int x, int y, int sprite);

void drawShip();
void lightShipLifs();
void setShipPosition();
void killShipByDescent();
void killShipByBullet();

void drawInvaders();
void invadersMoving(float speed, int descent);
void monstersSpeed();

void checkFireState();
void shipBulletMoving(float speed);
void killMonster();
void checkMonsters();

void shiftLeftBorder();
void shiftRightBorder();




byte getAlifeMonsters();

void invadersBullet(byte shotFrequency);
void invadersBulletMoving(float speed);

void drawScore();
int getHighScore();
void writeHighScoreToMemory();

void resetAll();
void loading();

#endif
