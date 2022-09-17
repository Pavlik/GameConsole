#include <Arduino.h>
#include <ship_controller.h>
#include <display.h>
#include "menu.h"

uint32_t timer = 0;

extern volatile bool left;
extern volatile bool right;
extern volatile bool fire;


void setup() {
  myDisplaySetup();
  setupButtons();
}

void loop()
{ 
  
  
  if(millis() - timer >= 16) 
  {
    if(!pauseGame())
    {
      //setShipPosition();
      setShipLeftPosition();
      setShipRightPosition();
      checkFireState();
    }
    if(pauseGame())
    {
      fire  = 0;
    }
    restartGame();
    displayAll();
    timer = millis();
  }
}