#include <SFML/Graphics.hpp>
#include<string.h>
#include <iostream>
using namespace std;
using namespace sf;

class Bullet
{
public:
Texture tex;
Sprite sprite;
float speed=1.2, time= 0,firetime = 0;
int x,y;
Bullet()
{
x = -10;
y = -30;
}
void bullet(std::string png_path, int xp , int yp, int t)
{
if(t - time>0.01)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xp+36;y=yp-25;
sprite.setPosition(x,y);
sprite.setScale(0.5,0.5);
time = t;
}
}
// void firebullet(std::string png_path, int xp , int yp, int t, double scalex, double scaley)
// {
// tex.loadFromFile(png_path);
// sprite.setTexture(tex);
// x=xp+36;y=yp-25;
// sprite.setPosition(x,y);
// sprite.setScale(scalex,scaley);
// }

void move()
{
y = sprite.getPosition().y;
if(y>-30)
{
float delta_x = 0,delta_y=-0.5;
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
