#include <SFML/Graphics.hpp>
#include<string.h>
#include <iostream>
using namespace std;
using namespace sf;

class Bomb
{
public:
Texture tex;
Sprite sprite;
double speed=0, time= 0;
int x,y;
Bomb()
{
x = -1000;
y = -3000;
}
void bomb(std::string png_path, int xp , int yp, double s)
{
    tex.loadFromFile(png_path);
    sprite.setTexture(tex);
    x=xp;y=yp;speed = s;
    sprite.setPosition(x,y);
    sprite.setScale(0.75,0.75);
    
}
void bomb(std::string png_path, int xp , int yp, double s, double scalex,double scaley)
{
    tex.loadFromFile(png_path);
    sprite.setTexture(tex);
    x=xp;y=yp;speed = s;
    sprite.setPosition(x,y);
    sprite.setScale(scalex,scaley);
    
}

void move()
{
    y = sprite.getPosition().y;
    if(y<800)
    {
        float delta_x = 0,delta_y=0.5;
        delta_x*=speed;
        delta_y*=speed;

        sprite.move(delta_x, delta_y);
    }
}
void move(int x)
{
    y = sprite.getPosition().y;
    if(y<800 && x%3==1)
    {
        float delta_x = 0,delta_y=0.5;
        delta_x*=speed;
        delta_y*=speed;

        sprite.move(delta_x, delta_y);
    }
    else if(y<800 && x%3==0)
    {
        float delta_x = -0.5,delta_y=0.5;
        delta_x*=speed;
        delta_y*=speed;

        sprite.move(delta_x, delta_y);
    }
    else if(y<800 && x%3==2)
    {
        float delta_x = 0.5,delta_y=0.5;
        delta_x*=speed;
        delta_y*=speed;

        sprite.move(delta_x, delta_y);
    }
}

int getpy()
{
return sprite.getPosition().y;
}
int getpx()
{
return sprite.getPosition().x;
}

};
