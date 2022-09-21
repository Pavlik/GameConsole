#include <Arduino.h>
#include <EEPROM.h> 
#include <display.h>
// #include <objects.h>??????
int randomInvader;
int x_cor_inv  = 16;
int y_cor_inv  = MONSTER_H;
int step       = MONSTER_W;
int h_distance = 12;
bool colomnState[6];
int currentScore = 0;
int highScore;

float invaderSpeed = INWADERS_SPEED;

Personage spaceShip(64-SHIP_W/2, 64 - SHIP_H, 1, 3, SHIP);
Object shipBullet(64-SHIP_W/4+2,64-SHIP_H,1);
Object monstersBullet(0,0,0);

Personage invaders[MONSTERS] = {
                        Personage(x_cor_inv,0,1,1, SPRUT), Personage(x_cor_inv + step,0,1,1, SPRUT), Personage(x_cor_inv + 2*step,0,1,1, SPRUT),
                        Personage(x_cor_inv + 3*step,0,1,1, SPRUT), Personage(x_cor_inv + 4*step,0,1,1, SPRUT), Personage(x_cor_inv + 5*step,0,1,1, SPRUT),

                        Personage(x_cor_inv,h_distance,1,1, CRAB), Personage(x_cor_inv + step,h_distance,1,1, CRAB), Personage(x_cor_inv + 2*step,h_distance,1,1, CRAB),
                        Personage(x_cor_inv + 3*step,h_distance,1,1, CRAB), Personage(x_cor_inv + 4*step,h_distance,1,1, CRAB), Personage(x_cor_inv + 5*step,h_distance,1,1, CRAB),

                        Personage(x_cor_inv,h_distance*2,1,1, OCTOPUS), Personage(x_cor_inv + step,h_distance*2,1,1, OCTOPUS), Personage(x_cor_inv + 2*step,h_distance*2,1,1, OCTOPUS),
                        Personage(x_cor_inv + 3*step,h_distance*2,1,1, OCTOPUS), Personage(x_cor_inv + 4*step,h_distance*2,1,1, OCTOPUS), Personage(x_cor_inv + 5*step,h_distance*2,1,1, OCTOPUS),

};


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void myDisplaySetup()
{
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))for(;;); // Don't proceed, loop forever
    for(byte i = 0; i < COLOMNS; i++) colomnState[i] = 1;
    for(byte i = 5; i < 8; i++) pinMode(i, OUTPUT);
    srand(analogRead(A1));
}

void drawScreen()
{
  clear();
  lightShipLifs();
  drawShip();
  drawInvaders();

  drawScore();
  writeHighScoreToMemory();

  shipBulletMoving(BULLET_SPEED);

  invadersBulletMoving(BULLET_SPEED);
  invadersBullet(SHOT_FREQUENCY);

  killMonster();
  killShipByDescent();
  killShipByBullet();
  
  
  checkMonsters();

  shiftLeftBorder();
  shiftRightBorder();
  update();
}

/*
Функция отрисовки спрайтов 
Передваймые значения: Координата по оси X, координата по оси Y, спрайт с bitmap
*/

void drawing(int x, int y, int sprite)
{
  switch (sprite)
  {
    case SPRUT: 
      display.drawBitmap(x, y, sprut, MONSTER_W, MONSTER_H, SSD1306_WHITE); 
      break;
    case CRAB: 
      display.drawBitmap(x, y, crab, MONSTER_W, MONSTER_H, SSD1306_WHITE); 
      break;
    case OCTOPUS: 
      display.drawBitmap(x, y, octopus, MONSTER_W, MONSTER_H, SSD1306_WHITE); 
      break;
    case SHIP: 
      display.drawBitmap(x, y, ship, SHIP_W, SHIP_H, SSD1306_WHITE); 
      break;
    case INVADER_BULLET: 
      display.drawBitmap(x, y, invader_bullet, INVADER_BULLET_W, INVADER_BULLET_H, SSD1306_WHITE); 
      break;
    case SHIP_BULLET: 
      display.drawBitmap(x, y, ship_bullet, SHIP_BULLET_W, SHIP_BULLET_H, SSD1306_WHITE); 
      break;
    case SPACE_LOGO:
      display.drawBitmap(x,y,space_logo,64,18, SSD1306_WHITE);
      break;
    case INVADERS_LOGO:
      display.drawBitmap(x,y,invaders_logo,104,18, SSD1306_WHITE);
      break;
  }
}

/*
Функция отрисовки корабля
*/

void drawShip()
{
  if(spaceShip.getStatus()) 
  {
    drawing(spaceShip.getXposition(), spaceShip.getYposition(), SHIP);
  }
}

int invader_new_x_position;
int invader_new_y_position;

void drawInvaders()
{
  monstersSpeed(); // функция задает скорость движения монстров
  invadersMoving(invaderSpeed, INWADERS_DESCENT);
  /*
    В цикле задаются позиции для монстров
  */
  for(int i = 0; i < MONSTERS; i++)
  {
    if(i < 6) 
    {
      invader_new_x_position = invaders[0].getXposition()+i*MONSTER_W; 
      invader_new_y_position = invaders[0].getYposition();
    }
    if(i >= 6 && i < 12)
    {
      invader_new_x_position = invaders[0].getXposition()+(i-6)*MONSTER_W;
      invader_new_y_position = invaders[0].getYposition()+h_distance;
    }
    if(i >= 12 )
    {
      invader_new_x_position = invaders[0].getXposition()+(i-12)*MONSTER_W;
      invader_new_y_position = invaders[0].getYposition()+h_distance*2;
    }
    
    invaders[i].setXposition(invader_new_x_position);
    invaders[i].setYposition(invader_new_y_position);
    /*
      отрисовка определенного спрайта для каждого ряда монстров
    */
    if(invaders[i].getStatus() && i < 6) 
      drawing(invaders[i].getXposition(), invaders[i].getYposition(), SPRUT);
    if(invaders[i].getStatus() && i >=6 && i< 12) 
      drawing(invaders[i].getXposition(), invaders[i].getYposition(), CRAB);
    if(invaders[i].getStatus() && i >= 12) 
      drawing(invaders[i].getXposition(), invaders[i].getYposition(), OCTOPUS);
  }
}

bool sideFlag = 1;
int rightBorder = MONSTER_W*MONSTERS_IN_ROW-4; 
int leftBorder = 1;
float buffPosition = 16.0;
float *pBuff = &buffPosition;
bool downFlag = 0;
byte descentCounter = 0;

void invadersMoving(float speed, int descent)
{
  /*0 - left, 1 - right*/
  //Почему если написать так: buffPosition = (float)invaders[0].getXposition(); , то скорость будет не равномерной???
    if(sideFlag)
    {
      /*
      изначально позиция равно 16, тут к ней прибавляется переменная speed, которая передается через invaderSpeed (задается в monstersSpeed())
      */
      buffPosition += speed; 
      invaders[0].setXposition(buffPosition);
      if(buffPosition >= 128-rightBorder) sideFlag = 0; // меняется направление движения, rightBorder задается в функции 
    }
    if(!sideFlag)
    {
      buffPosition -= speed;
      invaders[0].setXposition(buffPosition);
      if(buffPosition <= leftBorder) sideFlag = 1; // меняется направление движения, leftBorder задается в функции 
    }
    /*
    Если монстры касаются границы, тогда они оускаются на передаваемое значение переменной descent
    */
    if(downFlag == sideFlag) 
    {
      downFlag = !sideFlag;
      descentCounter+=1;
      if(descentCounter % 2 != 0)invaders[0].setYposition(invaders[0].getYposition() + descent);
    }

}

/*
  Проверяет нажатие кнопки выпуска ракеты корабля
*/
extern volatile bool fire;
bool fireState = 0;

void checkFireState()
{
  if(fire && spaceShip.getStatus())
  {
    fireState = 1;
    fire = 0;
  } 
  if(fire && !spaceShip.getStatus())
  {
    fireState = 0;
    fire = 0;
  } 
}

/*
  Задает логику движения ракеты корабля
*/
bool bulletStart = 0; // изначально ракета готова к запуску
int bullet_position = 0;

void shipBulletMoving(float speed) 
{
  if(bulletStart == 0 && !fireState)                                  //Ракета готова к запуску и неразрешено стрелять
  {
    shipBullet.setXposition(spaceShip.getXposition() + (SHIP_W-3)/2); // позиция ракеты по середине корабля по X
    bullet_position = 62-SHIP_H;                                      // позиция ракеты по середине корабля по Y
    shipBullet.setYposition(bullet_position);
    shipBullet.setStatus(1);                                          // Ракета не столкнулась с монстром
  }
  if(bulletStart == 0 && fireState)                                   //Ракета готова к запуску и разрешено стрелять
  {
    bullet_position -= speed;
    if(bullet_position <= 0) 
      {
        bullet_position = 62-SHIP_H;
        bulletStart = 1;
        shipBullet.setXposition(spaceShip.getXposition() + (SHIP_W-3)/2);
        fireState = 0;
      }
    shipBullet.setYposition(bullet_position); 
  }
  if(bulletStart && fireState == 0 ) bulletStart = 0;                         //Ракета выпущена и неразрешено стрелять
  if(shipBullet.getStatus() && spaceShip.getStatus())                         //Отрисовывать ракету, если она и корабль не уничтожены 
    drawing(shipBullet.getXposition(),shipBullet.getYposition(), SHIP_BULLET);
}

void killMonster()
{
  for(byte i = 0; i < MONSTERS; i ++)
  {  
    if(invaders[i].getStatus()  && (shipBullet.getXposition() >= invaders[i].getXposition() && shipBullet.getXposition() <= invaders[i].getXposition() + MONSTER_W-4) 
       && ((shipBullet.getYposition() <= invaders[i].getYposition() + MONSTER_H/2+2) && (shipBullet.getYposition() > invaders[i].getYposition())) && shipBullet.getStatus()
       && spaceShip.getStatus()) 
    {
      // Система очков
      if(invaders[i].getSkin() == OCTOPUS) currentScore += 5;
      if(invaders[i].getSkin() == CRAB) currentScore += 10;
      if(invaders[i].getSkin() == SPRUT) currentScore += 15;
      shipBullet.setStatus(0);  //Ракета столкнулась и уничтожена
      invaders[i].setStatus(0); //Угичтожен монстр
    }
  }
} 

byte currentLeftColomn = 0;
byte currentRightColomn = MONSTERS_IN_ROW;

/*
  Проверяет есть ли живые монстры, и если нет, тогда возвращает параметры монстров к исходным
*/
void checkMonsters()
{
  int counter = 0;
  for(int i = 0; i < MONSTERS; i++)
  {
    if(invaders[i].getStatus() == 0) counter++;
    if(counter == MONSTERS) 
    {
      for(int j = 0; j < MONSTERS; j++)
      {
        invaders[j].setStatus(1);
        if(j < COLOMNS) colomnState[j] = 1;
      }
      invaders[0].setYposition(0);

      *pBuff = 16;
      //invaders[0].setXposition(16); 
      invaderSpeed = INWADERS_SPEED;

      rightBorder = MONSTER_W*COLOMNS-4;
      leftBorder = 1;
      currentLeftColomn = 0;
      currentRightColomn = MONSTERS_IN_ROW;
    }
  }
}

/*
  Сдвигает левую границу движения монстров, если уничтожен крайняя левая колона
*/
void shiftLeftBorder()
{
  for(int i = 0; i < MONSTERS/ROWS; i++)
  {
    if(!invaders[i].getStatus() && !invaders[i+MONSTERS_IN_ROW].getStatus() && !invaders[i+MONSTERS_IN_ROW*2].getStatus() && colomnState[i] && currentLeftColomn == i)
    {
      leftBorder -= MONSTER_W;
      colomnState[i] = 0;
      currentLeftColomn++;
    }
  }
}

/*
  Сдвигает правую границу движения монстров, если уничтожен крайняя правая колона
*/

void shiftRightBorder()
{
  for(int i = MONSTERS/ROWS; i > 0; i--)
  {
    if(!invaders[i-1].getStatus() && !invaders[i-1+MONSTERS_IN_ROW].getStatus() && !invaders[i-1+MONSTERS_IN_ROW*2].getStatus() && colomnState[i-1] && currentRightColomn == i)
    {
      rightBorder -= MONSTER_W;
      colomnState[i-1] = 0;
      currentRightColomn--;
    }
  }
}

/*
  Если координата монстра опустилась до высты корабля, то корабль считается уничтоженным
*/

void killShipByDescent()
{
  for(byte i = 0; i < MONSTERS; i++)
    if(invaders[i].getYposition() + MONSTER_H >= (spaceShip.getYposition() - SHIP_BULLET_H) && invaders[i].getStatus() && spaceShip.getStatus()) 
      spaceShip.setStatus(0);
}

/*
  Если координаты пули монстра совпадают с границами корабля, тогда считать корабль уничтоженым
*/

void killShipByBullet()
{
  if(
      monstersBullet.getXposition() + INVADERS_BULLET_X_SHIFT >= spaceShip.getXposition()                      &&  // left border of the ship
      monstersBullet.getXposition() + INVADERS_BULLET_X_SHIFT <= spaceShip.getXposition() + SPACE_SHIP_X_SHIFT &&  // right border of the ship
      monstersBullet.getYposition() + INVADERS_BULLET_Y_SHIFT >= spaceShip.getYposition()                      &&  // top border of the ship
      monstersBullet.getStatus()
    )
    {
      monstersBullet.setStatus(0);
      spaceShip.setHP(spaceShip.getHP()-1);
      lightShipLifs();
      if(spaceShip.getHP() == 0) spaceShip.setStatus(0);
    }
}

/*
  Зажигает светодиоды, обознчающие жизни корабля
*/

void lightShipLifs()
{
  if(spaceShip.getHP() == 3) 
    for(byte i = 5; i < 8; i++) 
      digitalWrite(i, 1);
  else if(spaceShip.getHP() == 2) 
    for(byte i = 5; i < 7; i++) 
    {
      digitalWrite(i, 1); 
      digitalWrite(7, 0);
    }
  else if(spaceShip.getHP() == 1)
  { 
    digitalWrite(5, 1); 
    digitalWrite(6, 0); 
    digitalWrite(7, 0);
  }
  else if(spaceShip.getHP() == 0 ) 
    for(byte i = 5; i < 8; i++) 
      digitalWrite(i, 0);
}

/*
Функция управления скоростью движения монстров
Скорость изначально равна INWADERS_SPEED (1)
При каждом новом цикле сравнивается значение buffer и total и если они не равны, тогда новая скорость рассчитывается так:
  количество живых монстров умножается на коэфициент CHANGE_GRADE (0.15) и прибавляется к переменной new_speed
*/

void monstersSpeed()
{ 
  static byte buffer = MONSTERS;
  static float new_speed = invaderSpeed;
  byte total = getAlifeMonsters();
  if(buffer != total) 
    {
      invaderSpeed = new_speed + (float)(MONSTERS - total) * CHANGE_GRADE;
      buffer = total;
    }
}

void invadersBullet(byte shotFrequency)
{
  static byte counterForBullet = 0;
  static bool flagForBullet    = 0;
  counterForBullet++;
  if(getAlifeMonsters() <= 2) shotFrequency/=2;
  if(counterForBullet >= shotFrequency && !monstersBullet.getStatus()) flagForBullet = 1;
  if(flagForBullet) 
  {
    flagForBullet = 0;
    counterForBullet = 0;

    do
    {
      randomInvader = rand() % MONSTERS;
    } while(!invaders[randomInvader].getStatus());

    for(byte i = 0; i < MONSTERS; i++)
    {
      if(randomInvader == i && invaders[i].getStatus()) 
      {
        monstersBullet.setXposition(invaders[i].getXposition()+4);
        monstersBullet.setYposition(invaders[i].getYposition()+4);
        monstersBullet.setStatus(1);
      } 
    }
  }
}

/*
  Логика движения пули монстра 
*/

void invadersBulletMoving(float speed)
{
  if(monstersBullet.getYposition() <= 64 && monstersBullet.getStatus()) monstersBullet.setYposition(monstersBullet.getYposition()+speed);
  else monstersBullet.setStatus(0);
  if(monstersBullet.getStatus()) drawing(monstersBullet.getXposition(), monstersBullet.getYposition(), INVADER_BULLET);
}

/*
  Возвразщает количество живых монстров
*/

byte getAlifeMonsters()
{
  byte counter = 0;
  for(byte i = 0; i < MONSTERS; i++) if(invaders[i].getStatus()) counter ++;
  return counter;
}

void drawScore()
{
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);             
  display.println(currentScore);

  if(highScore < 10) display.setCursor(120,0);
  if(highScore > 10 && highScore < 100) display.setCursor(115,0);
  if(highScore > 100 && highScore < 1000) display.setCursor(110,0);
  if(highScore > 1000 && highScore < 10000) display.setCursor(103,0);     
  display.println(getHighScore());
}

void writeHighScoreToMemory()
{
  if(currentScore > highScore) 
  {
    EEPROM.update(1, highByte(currentScore));
    EEPROM.update(2, lowByte(currentScore));
  }
}

int getHighScore()
{
  highScore = word(EEPROM.read(1), EEPROM.read(2));
  return highScore;
}

void update()
{
  display.display();
}

void clear()
{
  display.clearDisplay();
}


void resetAll()
{
  if(!spaceShip.getStatus())
  {
    for(int j = 0; j < MONSTERS; j++)
      {
        invaders[j].setStatus(1);
        if(j < COLOMNS) colomnState[j] = 1;
      }
      invaders[0].setYposition(0);
      *pBuff = 16;
      invaderSpeed = INWADERS_SPEED;
      rightBorder = MONSTER_W*COLOMNS-4;
      leftBorder = 1;
      currentLeftColomn = 0;
      currentRightColomn = MONSTERS_IN_ROW;
      currentScore = 0;

      spaceShip.setHP(3);
      spaceShip.setStatus(1);
      spaceShip.setXposition(64-SHIP_W/2);
      spaceShip.setYposition(64 - SHIP_H);
  }
}

bool startScreen = 1;
byte counterForLoading;
void loading()
{
  display.drawRect(14, 50, 100, 4, SSD1306_WHITE);
  if(counterForLoading < 100) counterForLoading+=2;
  display.fillRect(14, 50, counterForLoading, 4, SSD1306_WHITE); 
  if(counterForLoading >=100) startScreen = 0;
}
