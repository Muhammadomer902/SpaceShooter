#include"Enemy.h"


class Invaders:public Enemy
{
public:
Invaders()
{
destroied = true;
x = -300;y = -400;
health = 0;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
Invaders(std::string png_path, int xpos, int ypos)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xpos;y=ypos;health = 0;
sprite.setPosition(xpos,-50);
sprite.setScale(0.5,0.5);
destroied = false;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
void move()
{
        if(sprite.getPosition().y<y)
        {
        delta_y = 1;
        delta_y*=speed;
        sprite.move(delta_x,delta_y);
        }
}
void movex(int xpos)
{
        
}
};

class Alpha:public Invaders
{
public:
Alpha()
{
destroied = true;
x = -3000;y = -4000;
health = 0;score = 10;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
Alpha(std::string png_path, int xpos, int ypos)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xpos;y=ypos;health = 1;score = 10;
sprite.setPosition(xpos,-100);
sprite.setScale(0.5,0.5);
destroied = false;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
void fire(float time, int level)
{
 //       score = 10*level;
if(time-timera>5 && !destroied)
{
        timera = time;
        if(i==numbomb)
        i=0;
        x = sprite.getPosition().x;
        y = sprite.getPosition().y;
        if(level==1)
                b[i]->bomb("img/PNG/Lasers/laserRed01.png",x+28,y+50,0.5);
        else if(level == 2)
                b[i]->bomb("img/PNG/Lasers/laserRed01.png",x+28,y+50,1.0);
        else
                b[i]->bomb("img/PNG/Lasers/laserRed01.png",x+28,y+50,1.5);
        i+=1;
}
}

};


class Beta:public Invaders
{
public:
Beta()
{
destroied = true;
x = -3000;y = -4000;
health = 0;score = 20;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
Beta(std::string png_path, int xpos, int ypos)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xpos;y=ypos;health = 2;score = 20;
sprite.setPosition(xpos,-100);
sprite.setScale(0.5,0.5);
destroied = false;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
void fire(float time, int level)
{
 //       score = 20*level;
if(time-timerb>3 && !destroied)
{
        timerb = time;
        if(i==numbomb)
        i=0;
        x = sprite.getPosition().x;
        y = sprite.getPosition().y;
        if(level==1)
        b[i]->bomb("img/PNG/Lasers/laserRed02.png",x+25,y+55,0.75);
        else if (level==2)
        b[i]->bomb("img/PNG/Lasers/laserRed02.png",x+25,y+55,1.25);
        else
        b[i]->bomb("img/PNG/Lasers/laserRed02.png",x+25,y+55,1.75);
        i+=1;
}
}

};

class Gamma:public Invaders
{
public:
Gamma()
{
destroied = true;
x = -3000;y = -4000;
health = 0;score = 30;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
Gamma(std::string png_path, int xpos, int ypos)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xpos;y=ypos;health = 3;score = 30;
sprite.setPosition(xpos,-100);
sprite.setScale(0.5,0.5);
destroied = false;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
void fire(float time, int level)
{
//score = 30*level;
if(time-timerg>2 && !destroied)
{
        timerg = time;
        if(i==numbomb)
        i=0;
        x = sprite.getPosition().x;
        y = sprite.getPosition().y;
        if(level==1)
        b[i]->bomb("img/PNG/Lasers/laserRed03.png",x+40,y+52,1.0);
        else if(level==2)
        b[i]->bomb("img/PNG/Lasers/laserRed03.png",x+40,y+52,1.5);
        else
        b[i]->bomb("img/PNG/Lasers/laserRed03.png",x+40,y+52,2.0);
        i+=1;
}
}
};


//Monster

class Monster:public Enemy
{
public:
int tempx = 0,tempy = 0;
Monster()
{
destroied = true;
x = -30000;y = 4000;
health = 30;score = 40;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
Monster(std::string png_path, int xpos, int ypos, double scalex,double scaley)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xpos;y=ypos;health = 30;destroied=false;speed = 0.01;score = 40;
sprite.setPosition(xpos,-500);
sprite.setScale(scalex,scaley);
//
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
//
}
void move()
{
        if(sprite.getPosition().y<y)
        {
        delta_y = 1;
        delta_y*=speed;
        sprite.move(0,delta_y);
        }
}
void movex(int xpos)
{
        if(sprite.getPosition().x<xpos)
        {
                delta_x = 1;
                delta_x*=speed;
                sprite.move(delta_x,0);
        }
        else if(sprite.getPosition().x>xpos)
        {
                delta_x = -1;
                delta_x*=speed;
                sprite.move(delta_x,0);
        }
}
void fire(float time, int mnum)
{
if(time-timerm>1.75 && !destroied &&  mnum==0)
{
        timerm = time;
        if(i==numbomb)
        i=0;
        tempx = sprite.getPosition().x;
        tempy = sprite.getPosition().y;
        b[i]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+20,tempy+305,0.3);
        b[i+1]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+168,tempy+440,0.3);
        b[i+2]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+243,tempy+440,0.3);
        b[i+3]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+388,tempy+305,0.3);
        if(timerm-lasertimer>3)
        {
                b[i+4]->bomb("img/PNG/Lasers/monsterlaser.png",tempx+140,tempy-620,1.3, 0.5,4.5);
                lasertimer = timerm;
        }
        i+=5;
}
else if(time-timerm>1.75 && !destroied && mnum==1)
{
        timerm = time;
        if(i==numbomb)
        i=0;
        tempx = sprite.getPosition().x;
        tempy = sprite.getPosition().y;
        b[i]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+80,tempy+390,0.3);
        b[i+1]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+155,tempy+350,0.3);
        b[i+2]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+237,tempy+350,0.3);
        b[i+3]->bomb("img/PNG/Lasers/laserBlue03.png",tempx+312,tempy+390,0.3);
        if(timerm-lasertimer>3)
        {
                b[i+4]->bomb("img/PNG/Lasers/monsterlaser.png",tempx+135,tempy-500,1.2, 0.5,4.5);
                lasertimer = timerm;
        }
        i+=5;
}
}

};


//






//Dragon

class Dragon:public Enemy
{
public:
int tempx = 0,tempy = 0;
Dragon()
{
destroied = true;
x = -30000;y = 4000;
health = 30;score = 50;
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
}
Dragon(std::string png_path, int xpos, int ypos, double scalex,double scaley)
{
tex.loadFromFile(png_path);
sprite.setTexture(tex);
x=xpos;y=ypos;health = 30;destroied=false;speed = 0.1;score = 50;
sprite.setPosition(xpos,-100);
sprite.setScale(scalex,scaley);
//
for(int j = 0;j<numbomb;j++)
b[j] = new Bomb;
//
}

void movex(int xpos)
{
}
void move()
{
        if(sprite.getPosition().y<y)
        {
        delta_y = 1;
        delta_y*=speed;
        sprite.move(0,delta_y);
        }
}
void fire(float time, int numdrag)
{
if(time-timerd>0.75 && !destroied &&  (numdrag==0 || numdrag==1))
{
        timerd = time;
        if(i==39)
        i=0;
        tempx = sprite.getPosition().x;
        tempy = sprite.getPosition().y;
        b[i]->bomb("img/dragonfire_45.png",tempx-100,tempy+180,0.55,0.5,0.5);
        b[i+1]->bomb("img/dragonfire.png",tempx+50,tempy+220,0.7,0.5,0.5);
        b[i+2]->bomb("img/dragonfire45.png",tempx+100,tempy+180,0.55,0.5,0.5);
        i+=3;
}
if(time-timerm>2 && !destroied && numdrag==2)
{
        timerd = time;
        if(i==39)
        i=0;
        tempx = sprite.getPosition().x;
        tempy = sprite.getPosition().y;
        b[i]->bomb("img/dragonfire.png",tempx+20,tempy+200,0.7,0.5,0.5);
        b[i+1]->bomb("img/dragonfire.png",tempx+60,tempy+200,0.7,0.5,0.5);
        b[i+2]->bomb("img/dragonfire.png",tempx+100,tempy+200,0.7,0.5,0.5);
        i+=3;
}
}


};

//
