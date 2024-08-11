#include <SFML/Graphics.hpp>
#include<string.h>
#include <iostream>
using namespace std;
using namespace sf;
#include"Bomb.h"


class Enemy
{
public:
int numbomb = 40;
Bomb** b = new Bomb*[numbomb];
Texture tex;
Sprite sprite;
bool destroied ;
float speed=0.5, delta_x= 0, delta_y= 0;
static float timera, timerb , timerg, timerm,lasertimer,timerd;
int x,y, health, i =0, score;
Enemy()
{
destroied = true;
x = -300;y = -400;
health = 0;
}
Enemy(std::string png_path, int xpos, int ypos)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xpos;y=ypos;health = 0;
sprite.setPosition(xpos,-50);
sprite.setScale(0.75,0.75);
//
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
//
}

virtual int getpy()
{
return this->sprite.getPosition().y;
}
virtual int getpx()
{
return this->sprite.getPosition().x;
}


//

//
virtual void dl()
{
	if(!destroied)
	{
		for(int k = 4;k<numbomb;k+=5)
		{
			delete b[k];
			b[k]=new Bomb;
		}
	}
}
virtual void db()
{
	if(!destroied)
	{
		for(int k = 0;k<numbomb;k++)
		{
			if(b[k]->getpy()>800)
			{
				delete b[k];
				b[k] = new Bomb;
			}
		}
	}
}
virtual void db(int x)
{
	if(!destroied)
	{
    	delete b[x];
    	b[x] = new Bomb;
	}
}
//

virtual void movex(int xpos) = 0;
virtual void move() = 0;
virtual void fire(float time, int level) = 0;

//

};
float Enemy::timera=0;
float Enemy::timerb=0;
float Enemy::timerg=0;
float Enemy::timerm=0;
float Enemy::lasertimer=0;
float Enemy::timerd=0;