#include "menu.h"
extern bool startScreen;

extern Personage spaceShip;
extern Adafruit_SSD1306 display;
extern int currentScore;

void displayAll()
{
    if(startScreen)loadStartScreen();
    else if(!startScreen && spaceShip.getStatus() && !pauseGame()) drawScreen();
    else if(!startScreen && spaceShip.getStatus() && pauseGame()) drawPause();
    else if(!spaceShip.getStatus()) looseScreen();
}

void loadStartScreen()
{
    clear();
    drawing(32,0,SPACE_LOGO);
    drawing(15,25,INVADERS_LOGO);
    loading();
    update();
}

void looseScreen()
{
    clear();
        display.setTextSize(2);             
        display.setTextColor(SSD1306_WHITE);        
        display.setCursor(7,0);             
        display.println(F("GAME OVER!"));

        
        display.setTextSize(1);             
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(7,25); 
        display.println(F("High score: "));        
        display.setCursor(75,25);             
        display.println(getHighScore());

        display.setCursor(7,35); 
        display.println(F("Your score: "));        
        display.setCursor(75,35);             
        display.println(currentScore);

        display.setTextSize(1);
        display.setCursor(35,55);
        display.println(F("Play again"));
    update();
}

void drawPause()
{
    clear();
        display.setTextSize(2);             
        display.setTextColor(SSD1306_WHITE);        
        display.setCursor(35,25);             
        display.println(F("PAUSE"));
    update();
}
