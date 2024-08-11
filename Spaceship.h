#include <SFML/Graphics.hpp>
#include<string.h>
#include <iostream>
using namespace std;
using namespace sf;
#include"Bullet.h"
#include"Addon.h"

class Player{
public:
//
int  numbullet = 20;
Bullet** b = new Bullet*[numbullet];
Addon** a = new Addon*[101];
//
Texture tex, tex_45, tex45;
Sprite sprite;
float speed=0.2, timer = 0;
int x,y,i = 0;
bool destroied;
Player()
{
	destroied = true;
	x = -1000;y=-3000;
	for(int j = 0;j<numbullet;j++)
	b[j]=new Bullet;
	for(int j=0;j<101;j++)
	a[j]= new Addon;
}
Player(std::string png_path)
{
tex.loadFromFile(png_path);
tex_45.loadFromFile("img/playertiltship-45.png");
tex45.loadFromFile("img/playertiltship45.png");
sprite.setTexture(tex);
x=350;y=700;destroied = false;
sprite.setPosition(x,y);
sprite.setScale(0.75,0.75);
//
for(int j = 0;j<numbullet;j++)
b[j]=new Bullet;
for(int j=0;j<101;j++)
{
	if(j==0)
	a[j] = new Lives("img/lives.png");
	else if (j==25 || j==75)
	a[j] = new Danger("img/dangeraddon.png");
	else if(j==50)
	a[j]  = new Fire("img/firebullet.png");
	else if(j==100)
	a[j] = new Powerup("img/shield.png");
	else
	a[j]= new Addon;
}
//
}
//
void db()
{
	for(int i = 0; i<numbullet;i++)
	{
		if(b[i]->getpy()<-15)
		{
		delete b[i];
		b[i] = new Bullet;
		}
	}
}
void db(int x)
{
delete b[x];
b[x] = new Bullet;
}
//
void fire(float time)
{
	if(time-timer>0.5)
	{
	timer = time;
	if(i==numbullet)
	i = 0;
	x = sprite.getPosition().x;
	y = sprite.getPosition().y;
	b[i]->bullet("img/PNG/Lasers/laserGreen02.png",x,y,time);
	i+=1;
	}
}
// void fire(float time, string png_path)
// {
// 	if(time-timer>0.5)
// 	{
// 	timer = time;
// 	if(i==numbullet)
// 	i = 0;
// 	x = sprite.getPosition().x;
// 	y = sprite.getPosition().y;
// 	b[i]->firebullet(png_path,x,y,time,0.1,0.2);
// 	i+=1;
// 	}
// }
void move(std::string s){
float delta_x=0,delta_y=0;
x = sprite.getPosition().x;
y = sprite.getPosition().y;

if(s=="l")
	delta_x=-1;
else if(s=="r")
	delta_x=1;
if(s=="u")
	delta_y=-1;
else if(s=="d")
	delta_y=1;
	
if(x>770 && y>670)
sprite.setPosition(-30,0);
else if( x<-70 && y>670)
sprite.setPosition(730,0);
else if( x<-70 && y<30)
sprite.setPosition(730,700);
else if( x>770 && y<30)
sprite.setPosition(-30,700);
else if(y<-70)
sprite.setPosition(x,770);
else if(y>770)
sprite.setPosition(x,-70);
else if(x>770)
sprite.setPosition(-70,y);
else if(x<-70)
sprite.setPosition(770,y);

delta_x*=speed;
delta_y*=speed;

sprite.move(delta_x, delta_y);
sprite.setTexture(tex);
}
void move(std::string s, std::string rl){
float delta_x=0,delta_y=0;
x = sprite.getPosition().x;
y = sprite.getPosition().y;

if(rl =="l")
{
	delta_x=-1;
	sprite.setTexture(tex_45);
	}
else if(rl =="r")
{
	delta_x=1;
	sprite.setTexture(tex45);
}
if(s=="u")
	delta_y=-1;
else if(s=="d")
delta_y = 1;
	
if(x>770 && y>670)
sprite.setPosition(-30,0);
else if( x<-70 && y>670)
sprite.setPosition(730,0);
else if( x<-70 && y<30)
sprite.setPosition(730,700);
else if( x>770 && y<30)
sprite.setPosition(-30,700);
else if(y<-70)
sprite.setPosition(x,770);
else if(y>770)
sprite.setPosition(x,-70);
else if(x>770)
sprite.setPosition(-70,y);
else if(x<-70)
sprite.setPosition(770,y);

delta_x*=speed;
delta_y*=speed;

sprite.move(delta_x, delta_y);
}

int getpy()
{
return sprite.getPosition().y;
}
int getpx()
{
return sprite.getPosition().x;
}
int forran()
{
	srand(time(0));
return rand()%101;
}

};
