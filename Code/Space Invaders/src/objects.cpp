#include <objects.h>

int Object::getXposition()
{
    return x_position;
}

int Object::getYposition()
{
    return y_position;
}

void Object::setXposition(int x_coordinate)
{
    x_position = x_coordinate;
}

void Object::setYposition(int y_coordinate)

{
    y_position = y_coordinate;
}

void Object::setStatus(bool alife)
{
    status = alife;
}

bool Object::getStatus()
{
    return status;
}

void Personage::setHP(int new_hp)
{
    health = new_hp;
}

int Personage::getHP()
{
    return health;
}

int Personage::getSkin()
{
    return skin;
}