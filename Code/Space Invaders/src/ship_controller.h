#include <PinChangeInterrupt.h>
#include "display.h"

#define PAUSE_RESTART_PIN 8
#define LEFT_PIN          2
#define RIGHT_PIN         3
#define MIDDLE            64-SHIP_W/2

void left_button();
void right_button();
void fire_button();
void setupButtons();
void restartGame();
bool pauseGame();

void setShipLeftPosition();
void setShipRightPosition();