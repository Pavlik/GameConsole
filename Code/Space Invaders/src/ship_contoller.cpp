#include <ship_controller.h>

extern Personage spaceShip;
uint32_t timeAlifeShip;

volatile bool fire  = 0; 

void setupButtons() 
{
  pinMode(LEFT_PIN,INPUT_PULLUP);                  
  pinMode(RIGHT_PIN,INPUT_PULLUP);                  
  pinMode(4,INPUT_PULLUP);             
  pinMode(PAUSE_RESTART_PIN,INPUT_PULLUP);  
  attachPCINT(20, fire_button, FALLING);              
}

void fire_button()
{
  fire = 1;
}

bool moveLeftFlag = 1;
bool moveRightFlag = 1;
uint32_t moveTimerDebounce;
int x_ship = 64-SHIP_W/2;

void setShipLeftPosition()
{  
  if(digitalRead(LEFT_PIN) && !moveLeftFlag)
  //if(!digitalRead(LEFT_PIN) && millis() - moveTimerDebounce >= 50 && !moveLeftFlag)
  {
    x_ship -= SHIP_STEP;
    if(x_ship <= 0) x_ship = 0;
    spaceShip.setXposition(x_ship);
    moveLeftFlag = 1;
  }
  if(!digitalRead(LEFT_PIN) && moveLeftFlag)
  //if(digitalRead(LEFT_PIN) && millis() - moveTimerDebounce >= 50 && moveLeftFlag)
  {
    moveLeftFlag = 0;
    moveTimerDebounce = millis();
  }
}

void setShipRightPosition()
{  
  
  if(digitalRead(RIGHT_PIN) && !moveRightFlag)                                             //Закоментировать строку, если не используется аппаратное устранение дребезга
  /*if(!digitalRead(RIGHT_PIN) && millis() - moveTimerDebounce >= 50 && !moveRightlag)*/   //Раскоментировать строку, если не используется аппаратное устранение дребезга
  {
    x_ship += SHIP_STEP;
    if(x_ship >= 128-SHIP_W) x_ship = 128-SHIP_W;
    spaceShip.setXposition(x_ship);
    moveRightFlag = 1;
  }
  if(!digitalRead(RIGHT_PIN) && moveRightFlag)
  /*if(digitalRead(RIGHT_PIN) && millis() - moveTimerDebounce >= 50 && moveRightlag)*/
  {
    moveRightFlag = 0;
    moveTimerDebounce = millis();
  }
}

/*
  Функция рестарта игры
  Срабатывает в случае нажатия кнопки при условии что корабль был убит
*/

uint32_t restartTimerDebounce = 0;
void restartGame()
{

  if(digitalRead(PAUSE_RESTART_PIN) && millis() - restartTimerDebounce >= 50 && !spaceShip.getStatus())
  {
    timeAlifeShip = millis();
    resetAll();
    restartTimerDebounce = millis();
  }
}

/*
  Функция паузы игры
  Срабатывает при нажатии кнопки в случае если корабль жив и время игры больше 1 секунды 
*/
bool pause = 0;
bool pauseFlag = 0;
bool pauseGame()
{
  if(digitalRead(PAUSE_RESTART_PIN) && millis() - restartTimerDebounce >= 50 && spaceShip.getStatus() && !pauseFlag && millis()-timeAlifeShip >= 1000)
  {
    pause = !pause;
    pauseFlag = 1;
    restartTimerDebounce = millis();
  }
  if(!digitalRead(PAUSE_RESTART_PIN) && millis() - restartTimerDebounce >= 50 && spaceShip.getStatus() && pauseFlag)
  {
    pauseFlag = 0;
    restartTimerDebounce = millis();
  }
  return pause;
}
