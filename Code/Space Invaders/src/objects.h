#ifndef OBJECTS
#define OBJECTS

class Object
{
    public:
        Object(int x, int y, bool st)
        {
            x_position = x;
            y_position = y;
            status = st;
        }
        int  getXposition();
        int  getYposition();
        void setXposition(int x_coordinate);
        void setYposition(int y_coordinate);
        bool getStatus();
        void setStatus(bool alife);

    private:
        int  x_position;
        int  y_position;
        bool status;
};

class Personage : public Object
{
    public:
        Personage(int x, int y, bool st,int hp, int type) : Object(x,y,st), health(hp), skin(type)
        {}
        void setHP(int new_hp);
        int  getHP();
        int  getSkin();

    private:
        int  health;
        int  skin;
};

#endif