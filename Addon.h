#include <SFML/Graphics.hpp>
#include<string.h>
#include <iostream>
using namespace std;
using namespace sf;


class Addon
{
public:
int x, y;
float speed = 0.1;
Texture texture;
Sprite sprite;
Addon()
{
    x = 0;y = 0;
}
virtual void move()
{
    y = sprite.getPosition().y;
    if(y<900)
    {
        float delta_x = 0,delta_y=0.5;
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


class Fire: public Addon
{
public:
Fire(std::string png_path)
{
    x = -10,y=-1;
    srand(time(0));
    texture.loadFromFile(png_path);
    sprite.setTexture(texture);
    sprite.setPosition(rand()%680,10);
    sprite.setScale(0.1,0.1);
}
};

class Powerup: public Addon
{
public:
Powerup(std::string png_path)
{
    x = -100,y=-1;
    srand(time(0));
    texture.loadFromFile(png_path);
    sprite.setTexture(texture);
    sprite.setPosition(rand()%680,10);
    sprite.setScale(0.2,0.2);
}
};

class Danger: public Addon
{
public:
Danger(std::string png_path)
{
    x = -1,y=-1;
    srand(time(0));
    texture.loadFromFile(png_path);
    sprite.setTexture(texture);
    sprite.setPosition(rand()%680,10);
    sprite.setScale(0.1,0.1);
    sprite.setColor(Color::Black);
}
};

class Lives: public Addon
{
public:
Lives(std::string png_path)
{
    x = 1,y=1;
    srand(time(0));
    texture.loadFromFile(png_path);
    sprite.setTexture(texture);
    sprite.setPosition(rand()%680,10);
    sprite.setScale(0.1,0.1);
}
};
