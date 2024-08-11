#include<SFML/Graphics.hpp>
#include<time.h>
#include<fstream>
#include<cmath>
#include<iostream>
using namespace std;
#include "Spaceship.h"
#include "Invader.h"
const char title[] = "OOP-Project, Spring-2023";
using namespace sf;

class Game
{
public:
Sprite background, l[5], healthbar, redhealthbar; //Game background sprite
Sprite* lives = l;
Texture bg_texture, live_texture, ehealthbartexture, ehealthtext;
Player* p; //player 
// add other game attributes
int numbullet = 20, numenemy = 0, level = 1, stage = 0, numbomb = 40, health = 3, score = 0, nummonster = 0, addonnum = -1, a = 0, numdragon = 0;
bool allenemydestroied = true, pause  = false, allenemyinplace = false, firebullet = false, immunity = false, monsterspawn = false, dragonspawn = false;
string lines1, lines2,lines3;
Enemy** ene = nullptr;
Enemy** mon = nullptr;
Enemy** dra = nullptr;
Addon** add = nullptr;
fstream Gamefile;
Font font;

Game()
{
p=new Player("img/player_ship.png");
bg_texture.loadFromFile("img/background.jpg");
background.setTexture(bg_texture);
background.setScale(2, 1.5);
live_texture.loadFromFile("img/lives.png");
ehealthbartexture.loadFromFile("img/healthbar.png");
healthbar.setTexture(ehealthbartexture);
healthbar.setPosition(100,100);
healthbar.setScale(1.2,0.2);
ehealthtext.loadFromFile("img/redhealthbar.png");
redhealthbar.setTexture(ehealthtext);
redhealthbar.setPosition(27,88);
redhealthbar.setScale(0.09*30,0.5);
font.loadFromFile("vaca_6918982/vaca.ttf");
//
	Gamefile.open("Highscores.txt", fstream::in);

	getline(Gamefile,lines1);
	getline(Gamefile,lines2);
	getline(Gamefile,lines3);

	Gamefile.close();
//

mon = new Enemy*[2];
mon[0] = new Monster();
mon[1] = new Monster();

//

a=0;
add = new Addon*[10];
for(int i = 0;i<10;i++)
add[i]=	new Addon;

//

dra = new Enemy*[3];
dra[0]= new Dragon();
dra[1]=new Dragon();
dra[2]=new Dragon();

//
}

void start_game(int l = 1, int s = 0)
{
	background.setTexture(bg_texture);
    srand(time(0));
	nummonster = rand()%2;
    RenderWindow window(VideoMode(780, 780), title);
    Clock clock;
    float timer=0, btimer = 0, bftimer = 0, etime = 0, edtime = 0, botime = 0, boftime = 0, delaybomb = 0, monstertimer = 0, monsterfiretimer = 0, delayaddon = 0, firebullettimer = 0,immunitytimer = 0, dragontimer = 0, dragonspawntime = 0;
    level = l;
	stage = s;

    while (window.isOpen())
    {
		Event e;
        float time = clock.getElapsedTime().asSeconds(); 
		//
        while (window.pollEvent(e))
        {  
            if (e.type == Event::Closed) // If cross/close is clicked/pressed
                window.close(); //close the game   
			else if (e.type == Event::KeyPressed && e.key.code == Keyboard::P) 
			{
				if(pause)
				pause = false;
				else
				{
					pause = true;
					Text pauset("Game Paused",font,50);
					pauset.setPosition(200,350);
					window.draw(pauset);
					window.display();
				}
			}	                     	    
        }
		if(pause)
		{
			time = 0;
			continue;
		}
        else
		{
        	btimer+=time;
        	etime+=time;
			botime+=time;
			delaybomb+=time;
			monstertimer+=time;
			delayaddon+=time;
			dragontimer+=time;
			if(firebullet)
			{
				firebullettimer+=time;
			}
			if(immunity)
			{
				immunitytimer+=time;
			}
			if(monsterspawn)
			{
				monsterfiretimer+=time;
			}
			if(dragonspawn)
			{
				dragonspawntime+=time;
			}
		}

//addon
		for(int i = 0;i<5;i++)
		{
			if(i<health)
			{
				lives[i].setTexture(live_texture);
				lives[i].setPosition(30*i,0);
				lives[i].setScale(0.1,0.1);
			}
		}

		for(int i = 0;i<10;i++)
		{
			if(add[i]->x!=0)
			add[i]->move();
			if(add[i]->getpy()>800)
			{
				if(add[i]->x==-1)
				score+=15;
				delete add[i];
				add[i]= new Addon;
			}
		}
		addonnum = p->forran();
		if(p->a[addonnum]->x!=0 && delayaddon >5)
		{
			delayaddon=0;
			add[a] = p->a[addonnum];
			if(addonnum==0)
			p->a[addonnum]=new Lives("img/lives.png");
			else if(addonnum==25 || addonnum==75)
			p->a[addonnum]=new Danger("img/dangeraddon.png");
			else if(addonnum==50)
			p->a[addonnum]=new Fire("img/firebullet.png");
			else if(addonnum==100)
			p->a[addonnum]=new Powerup("img/shield.png");
			a+=1;
			if(a==10)
			a = 0;
		}
		spaceshipaddoncollision();
		if(firebullettimer>5)
		{
			firebullet = false;
			firebullettimer=0;
		}
		if(immunitytimer>5)
		{
			immunity = false;
			immunitytimer=0;
		}
//
//enemy invader
		for(int i = 0;i<numenemy;i++)
		{
			if(ene[i]->getpy()<ene[i]->y)
			ene[i]->move();
		}
		for(int i = 0;i<numenemy && !allenemyinplace;i++)
		{
			if(ene[i]->getpy()<ene[i]->y)
			{
				allenemyinplace = false;
				break;
			}
			else
			allenemyinplace = true;
		}

        for(int i = 0;i<numenemy;i++)
		{
			if(!ene[i]->destroied)
			{
				allenemydestroied = false;
				break;
			}
			else
			{
				allenemydestroied = true;
			}
		}

		if(allenemydestroied && (etime-edtime>1) && mon[nummonster]->destroied && monstertimer<60 && dra[numdragon]->destroied && dragontimer<40)
		{
				if(stage==3 && level!=3)
				{
					level+=1;
					stage = 1;
				}
				else
				stage+=1;
		   edtime = etime;
     	   generateenemy();
		   delaybomb = 0;
 	    }
        p->db();
        if(btimer-bftimer>0.001)
        {
        	bftimer = btimer;
        	for(int i = 0;i<numbullet;i++)
        	{
        		if(p->b[i]->x!=-10 && p->b[i]->y!=-30)
        		p->b[i]->move();
        	}
        }

		if(health>0 && firebullet==false)
        checkbulletcollision();
		else if(health>0 && firebullet)
		checkfirebulletcollision();

		if(allenemyinplace && delaybomb>1.5)
		enemydropbomb(botime);

		if(botime-boftime>0.001)
		{
			boftime = botime;
			for(int i = 0;i<numenemy;i++)
			{
				for(int j = 0;j<numbomb;j++)
				{
					if(ene[i]->b[j]->x!=-10 && ene[i]->b[j]->y!=-30 && !ene[i]->destroied)
					ene[i]->b[j]->move();
				}
			}
		}

		for(int i = 0;i<numenemy;i++)
		{
			if(!ene[i]->destroied)
			ene[i]->db();
		}

		if(!immunity)
		checkboombcollision();

		if(health>0)
		checkspaceshipcollision();
//enemymonster     
		if(mon[nummonster]->getpy()!=mon[nummonster]->y)
		mon[nummonster]->move();
		if(mon[nummonster]->getpx()!=p->getpx()-300)
		mon[nummonster]->movex(p->getpx()-160);
		if(monstertimer>60 && mon[nummonster]->destroied && allenemydestroied && dra[numdragon]->destroied && dragontimer<40)
		{
			nummonster = rand()%2;
			generatemonster();
			monstertimer = 0;
			monsterspawn = true;
		}
		bulletcollisionmonster();
		if(monsterspawn)
		{
			mon[nummonster]->fire(monsterfiretimer, nummonster);
			for(int i = 0;i<numbomb ;i++)
			{
			mon[nummonster]->b[i]->move();
			}
			checkcollisionmonsterbomb();
			mon[nummonster]->db();
		}
//enemydragon

		if(dragontimer>40 && !monsterspawn && allenemydestroied && !dragonspawn)
		{
			numdragon=rand()%3;
			generatedragon();
			dragontimer=0;
			dragonspawn=true;
		}
		if(dragonspawn)
		{
			dra[numdragon]->move();
			if(numdragon!=2)
			dra[numdragon]->fire(dragontimer, numdragon);
			for(int i = 0;i<numbomb ;i++)
			{
			dra[numdragon]->b[i]->move(i);
			}
			firecollisionspaceship();
			dra[numdragon]->db();
		}
		if(dragonspawntime>5)
		{
			score+=50;
			dragonspawn=false;
			dragonspawntime = 0;
			for(int i =0;i<numbomb;i++)
			delete dra[numdragon]->b[i];
			delete dra[numdragon];
			dra[numdragon]= new Dragon();
			dra[numdragon]->timerd=0;
		}

//
		if(health<=0)
		break;

        //
        clock.restart();
        timer += time;  
        //cout<<btimer<<endl; 

        if (Keyboard::isKeyPressed(Keyboard::Space) && allenemyinplace && delaybomb>1.5){ //If space is pressed
            //  if(!firebullet)
			  p->fire(btimer);
			//   else
			//   p->fire(btimer, "img/firebullet.png");
              bftimer  = btimer;  // Player will fire
            }
	if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Left)) //If left key is pressed
            p->move("u","l");   // Player will move to left
	else if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Right)) // If right key is pressed
            p->move("u","r");  //player will move to right
        else if (Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Left)) //If left key is pressed
            p->move("d","l");   // Player will move to left
	else if (Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Right)) // If right key is pressed
            p->move("d","r");  //player will move to right
	else if (Keyboard::isKeyPressed(Keyboard::Up)) //If up key is pressed
            p->move("u");    //playet will move upwards
	else if (Keyboard::isKeyPressed(Keyboard::Down)) // If down key is pressed
            p->move("d");  //player will move downwards
        else if (Keyboard::isKeyPressed(Keyboard::Right))
        p->move("r");
            else if (Keyboard::isKeyPressed(Keyboard::Left))
        p->move("l");
	////////////////////////////////////////////////
	/////  Call your functions here            ////
	//////////////////////////////////////////////

	window.clear(Color::Black); //clears the screen
	window.draw(background);  // setting background
	if(!p->destroied)
	window.draw(p->sprite);   // setting player on screen
	//
	for(int i = 0;i<numenemy;i++)
	{
	window.draw(ene[i]->sprite);
	}
	for(int i = 0;i<numbullet;i++)
	{
	if(p->b[i]->x!=-10 && p->b[i]->y!=-30)
	window.draw(p->b[i]->sprite);
	}
	for(int i = 0;i<numenemy;i++)
	{
		for(int j = 0;j<numbomb;j++)
		{
			if(ene[i]->b[j]->x!=-1000 && ene[i]->b[j]->y!=-3000 && !ene[i]->destroied)
			window.draw(ene[i]->b[j]->sprite);
		}
	}
	if(!mon[nummonster]->destroied)
	{
		for(int i = 0;i<numbomb;i++)
		window.draw(mon[nummonster]->b[i]->sprite);
		window.draw(mon[nummonster]->sprite);
		window.draw(healthbar);
		window.draw(redhealthbar);
		redhealthbar.setScale(0.09*mon[nummonster]->health,0.5);
	}
	if(dragonspawn)
	{
		for(int i = 0;i<numbomb;i++)
		window.draw(dra[numdragon]->b[i]->sprite);
		window.draw(dra[numdragon]->sprite);
	}
	for(int i = 0;i<health;i++)
	{
		window.draw(lives[i]);
	}
	for(int i = 0;i<10;i++)
	{
		if(add[i]->x!=0)
		window.draw(add[i]->sprite);
	}
	//
	window.display();  //Displying all the sprites
	}

}

void generateenemy()
{
	int y = 0, x = 0;
	if(ene!=nullptr)
	{
		for(int i = 0;i<numenemy;i++)
		delete ene[i];
		delete[]ene;
	}
	if(level == 1 && stage ==1)
	{
		x = 100;y = 100;
		numenemy = 12;
		ene = new Enemy*[numenemy];
		for(int i = 0;i<numenemy;i+=numenemy/3)
		{
			for(int j = 0, t = 1;j<numenemy/3;j++, t++)
			{
				if(i<4)
				ene[j+i] = new Gamma("img/enemy_3.png", x+(100*t), y);
				else if(i<8)
				{
					if((i+j)<6)
					ene[j+i] = new Beta("img/enemy_2.png", x, y+(t*100));
					else
					ene[j+i] = new Beta("img/enemy_2.png", x+500, y+(t*100));
					if(t == 2)
					t = 0;
				}
				else if(i<12)
				ene[j+i] = new Alpha("img/enemy_1.png", x+(100*t), y+300);
			}
		}
	}
	else if (level == 1 && stage ==2)
	{
		x = 50;y = 100;
		numenemy = 14;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+100, y);
		ene[2] = new Gamma("img/enemy_3.png", x+200, y);
		ene[3] = new Gamma("img/enemy_3.png", x+300, y);
		ene[4] = new Gamma("img/enemy_3.png", x+400, y);
		ene[5] = new Gamma("img/enemy_3.png", x+500, y);
		ene[6] = new Gamma("img/enemy_3.png", x+600, y);
		ene[7] = new Beta("img/enemy_2.png", x+50, y+100);
		ene[8] = new Beta("img/enemy_2.png", x+550, y+100);
		ene[9] = new Beta("img/enemy_2.png", x+100, y+200);
		ene[10] = new Beta("img/enemy_2.png", x+500, y+200);
		ene[11] = new Alpha("img/enemy_1.png", x+150, y+300);
		ene[12] = new Alpha("img/enemy_1.png", x+450, y+300);
		ene[13] = new Alpha("img/enemy_1.png", x+300, y+400);
	}
	else if (level == 1 && stage ==3)
	{
		x = 150;y = 0;
		numenemy = 17;
		ene = new Enemy*[numenemy];
		for(int i = 0, t = 0;i<numenemy;i++, t++)
		{
			if(i<3)
			{
				y=50;x = 30;
				ene[i] = new Gamma("img/enemy_3.png", x+(75*t), y+(50*t));
				if(i==2)
				{
					x = 700;y = 0;
					t = 0;
				}
			}
			else if(i<6)
			{
				ene[i] = new Gamma("img/enemy_3.png", x+(-75*t), y+(50*t));
			}
			else if(i<9)
			{
				ene[i] = new Beta("img/enemy_2.png", x+(-75*t), y+(50*t));
				if(i==8)
				{
					x = 250;y = 200;
					t = 0;
				}
			}
			else if(i<11)
			{
				ene[i] = new Beta("img/enemy_2.png", x, y);
				if(i==10)
				{
					y-=100;
					t = 0;
				}
				else
				x+=150;y+=100;
			}
			else if(i<14)
			{
				ene[i] = new Alpha("img/enemy_1.png", x+(75*t), y+(40*t));
				if(i==13)
				{
					x = -50;y = y+(50*t);
					t=0;
				}
			}
			else
			{
				ene[i] = new Alpha("img/enemy_1.png", x+(75*t), y-(35*t));
			}
		}
	}
	else if(level == 2 && stage == 1)
	{
		x = 350;y = 100;
		numenemy = 14;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+80, y+20);
		ene[2] = new Gamma("img/enemy_3.png", x-80, y+20);
		ene[3] = new Gamma("img/enemy_3.png", x+150, y+60);
		ene[4] = new Gamma("img/enemy_3.png", x-150, y+60);
		ene[5] = new Beta("img/enemy_2.png", x+190, y+130);
		ene[6] = new Beta("img/enemy_2.png", x-170, y+130);
		ene[7] = new Beta("img/enemy_2.png", x+190, y+200);
		ene[8] = new Beta("img/enemy_2.png", x-170, y+200);
		ene[9] = new Alpha("img/enemy_1.png", x+150, y+270);
		ene[10] = new Alpha("img/enemy_1.png", x-130, y+270);
		ene[11] = new Alpha("img/enemy_1.png", x+80, y+310);
		ene[12] = new Alpha("img/enemy_1.png", x-80, y+310);
		ene[13] = new Alpha("img/enemy_1.png", x, y+330);
	}
	else if(level == 2 && stage == 2)
	{
		x = 350;y = 100;
		numenemy = 16;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+75, y+50);
		ene[2] = new Gamma("img/enemy_3.png", x-75, y+50);
		ene[3] = new Gamma("img/enemy_3.png", x+150, y+100);
		ene[4] = new Gamma("img/enemy_3.png", x-150, y+100);
		ene[5] = new Beta("img/enemy_2.png", x+225, y+150);
		ene[6] = new Beta("img/enemy_2.png", x-225, y+150);
		ene[7] = new Beta("img/enemy_2.png", x+300, y+200);
		ene[8] = new Beta("img/enemy_2.png", x-300, y+200);
		ene[9] = new Beta("img/enemy_2.png", x+225, y+250);
		ene[10] = new Beta("img/enemy_2.png", x-225, y+250);
		ene[11] = new Alpha("img/enemy_1.png", x+150, y+300);
		ene[12] = new Alpha("img/enemy_1.png", x-150, y+300);
		ene[13] = new Alpha("img/enemy_1.png", x+75, y+350);
		ene[14] = new Alpha("img/enemy_1.png", x-75, y+350);
		ene[15] = new Alpha("img/enemy_1.png", x, y+400);
	}
	else if(level == 2 && stage == 3)
	{
		x = 350;y = 200;
		numenemy = 22;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+50, y-65);
		ene[2] = new Gamma("img/enemy_3.png", x-50, y-65);
		ene[3] = new Gamma("img/enemy_3.png", x+100, y-130);
		ene[4] = new Gamma("img/enemy_3.png", x-100, y-130);
		ene[5] = new Gamma("img/enemy_3.png", x+190, y-150);
		ene[6] = new Gamma("img/enemy_3.png", x-190, y-150);
		ene[7] = new Beta("img/enemy_2.png", x+290, y-140);
		ene[8] = new Beta("img/enemy_2.png", x-270, y-140);
		ene[9] = new Beta("img/enemy_2.png", x+360, y-60);
		ene[10] = new Beta("img/enemy_2.png", x-340, y-60);
		ene[11] = new Beta("img/enemy_2.png", x+310, y+30);
		ene[12] = new Beta("img/enemy_2.png", x-290, y+30);
		ene[13] = new Alpha("img/enemy_1.png", x+260, y+100);
		ene[14] = new Alpha("img/enemy_1.png", x-240, y+100);
		ene[15] = new Alpha("img/enemy_1.png", x+200, y+150);
		ene[16] = new Alpha("img/enemy_1.png", x-180, y+150);
		ene[17] = new Alpha("img/enemy_1.png", x+140, y+200);
		ene[18] = new Alpha("img/enemy_1.png", x-120, y+200);
		ene[19] = new Alpha("img/enemy_1.png", x+80, y+250);
		ene[20] = new Alpha("img/enemy_1.png", x-60, y+250);
		ene[21] = new Alpha("img/enemy_1.png", x, y+300);
	}
	else if(level == 3 && stage == 1)
	{
		x = 150;y = 100;
		numenemy = 20;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+200, y);
		ene[2] = new Gamma("img/enemy_3.png", x+400, y);
		ene[3] = new Gamma("img/enemy_3.png", x+100, y+100);
		ene[4] = new Gamma("img/enemy_3.png", x+300, y+100);
		ene[5] = new Beta("img/enemy_2.png", x+10, y+200);
		ene[6] = new Beta("img/enemy_2.png", x+210, y+200);
		ene[7] = new Beta("img/enemy_2.png", x+410, y+200);
		ene[8] = new Beta("img/enemy_2.png", x+110, y+300);
		ene[9] = new Beta("img/enemy_2.png", x+310, y+300);
		ene[10] = new Alpha("img/enemy_1.png", x+110, y);
		ene[11] = new Alpha("img/enemy_1.png", x+310, y);
		ene[12] = new Alpha("img/enemy_1.png", x+10, y+100);
		ene[13] = new Alpha("img/enemy_1.png", x+210, y+100);
		ene[14] = new Alpha("img/enemy_1.png", x+410, y+100);
		ene[15] = new Alpha("img/enemy_1.png", x+110, y+200);
		ene[16] = new Alpha("img/enemy_1.png", x+310, y+200);
		ene[17] = new Alpha("img/enemy_1.png", x+10, y+300);
		ene[18] = new Alpha("img/enemy_1.png", x+210, y+300);
		ene[19] = new Alpha("img/enemy_1.png", x+410, y+300);
	}
	else if(level == 3 && stage == 2)
	{
		x = 100;y = 50;
		numenemy = 21;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+100, y);
		ene[2] = new Gamma("img/enemy_3.png", x+200, y);
		ene[3] = new Gamma("img/enemy_3.png", x+300, y);
		ene[4] = new Gamma("img/enemy_3.png", x+400, y);
		ene[5] = new Gamma("img/enemy_3.png", x+500, y);
		ene[6] = new Gamma("img/enemy_3.png", x+50, y+100);
		ene[7] = new Gamma("img/enemy_3.png", x+150, y+100);
		ene[8] = new Gamma("img/enemy_3.png", x+250, y+100);
		ene[9] = new Gamma("img/enemy_3.png", x+350, y+100);
		ene[10] = new Gamma("img/enemy_3.png", x+450, y+100);
		ene[11] = new Beta("img/enemy_2.png", x+110, y+200);
		ene[12] = new Beta("img/enemy_2.png", x+210, y+200);
		ene[13] = new Beta("img/enemy_2.png", x+310, y+200);
		ene[14] = new Beta("img/enemy_2.png", x+410, y+200);
		ene[15] = new Beta("img/enemy_2.png", x+160, y+300);
		ene[16] = new Beta("img/enemy_2.png", x+260, y+300);
		ene[17] = new Beta("img/enemy_2.png", x+360, y+300);
		ene[18] = new Alpha("img/enemy_1.png", x+210, y+400);
		ene[19] = new Alpha("img/enemy_1.png", x+310, y+400);
		ene[20] = new Alpha("img/enemy_1.png", x+260, y+500);
	}
	else if(level == 3 && stage == 3)
	{
		x = 50;y = 0;
		numenemy = 36;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+75, y+50);
		ene[2] = new Gamma("img/enemy_3.png", x+150, y+100);
		ene[3] = new Gamma("img/enemy_3.png", x+225, y+150);
		ene[4] = new Gamma("img/enemy_3.png", x+300, y+200);
		ene[5] = new Gamma("img/enemy_3.png", x+300, y);
		ene[6] = new Gamma("img/enemy_3.png", x+225, y+50);
		ene[7] = new Gamma("img/enemy_3.png", x+75, y+150);
		ene[8] = new Gamma("img/enemy_3.png", x, y+200);
		x = 420;y = 250;
		ene[9] = new Beta("img/enemy_2.png", x, y);
		ene[10] = new Beta("img/enemy_2.png", x+75, y+50);
		ene[11] = new Beta("img/enemy_2.png", x+150, y+100);
		ene[12] = new Beta("img/enemy_2.png", x+225, y+150);
		ene[13] = new Beta("img/enemy_2.png", x+300, y+200);
		ene[14] = new Beta("img/enemy_2.png", x+300, y);
		ene[15] = new Beta("img/enemy_2.png", x+225, y+50);
		ene[16] = new Beta("img/enemy_2.png", x+75, y+150);
		ene[17] = new Beta("img/enemy_2.png", x, y+200);
		x = 440;y = 0;
		ene[18] = new Alpha("img/enemy_1.png", x, y);
		ene[19] = new Alpha("img/enemy_1.png", x+60, y+50);
		ene[20] = new Alpha("img/enemy_1.png", x+120, y+100);
		ene[21] = new Alpha("img/enemy_1.png", x+180, y+150);
		ene[22] = new Alpha("img/enemy_1.png", x+240, y+200);
		ene[23] = new Alpha("img/enemy_1.png", x+240, y);
		ene[24] = new Alpha("img/enemy_1.png", x+180, y+50);
		ene[25] = new Alpha("img/enemy_1.png", x+60, y+150);
		ene[26] = new Alpha("img/enemy_1.png", x, y+200);
		x = 60;y = 270;
		ene[27] = new Alpha("img/enemy_1.png", x, y);
		ene[28] = new Alpha("img/enemy_1.png", x+60, y+50);
		ene[29] = new Alpha("img/enemy_1.png", x+120, y+100);
		ene[30] = new Alpha("img/enemy_1.png", x+180, y+150);
		ene[31] = new Alpha("img/enemy_1.png", x+240, y+200);
		ene[32] = new Alpha("img/enemy_1.png", x+240, y);
		ene[33] = new Alpha("img/enemy_1.png", x+180, y+50);
		ene[34] = new Alpha("img/enemy_1.png", x+60, y+150);
		ene[35] = new Alpha("img/enemy_1.png", x, y+200);
	}
	else if(level == 3 && stage == 4)
	{
		x = 350;y = 250;
		numenemy = 33;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+100, y);
		ene[2] = new Gamma("img/enemy_3.png", x-100, y);
		ene[3] = new Gamma("img/enemy_3.png", x, y-100);
		ene[4] = new Gamma("img/enemy_3.png", x, y+100);
		ene[5] = new Gamma("img/enemy_3.png", x+75, y+65);
		ene[6] = new Gamma("img/enemy_3.png", x-75, y+65);
		ene[7] = new Gamma("img/enemy_3.png", x+75, y-65);
		ene[8] = new Gamma("img/enemy_3.png", x-75, y-65);
		x+=10;
		ene[9] = new Beta("img/enemy_2.png", x+200, y);
		ene[10] = new Beta("img/enemy_2.png", x-200, y);
		ene[11] = new Beta("img/enemy_2.png", x, y+180);
		ene[12] = new Beta("img/enemy_2.png", x, y-180);
		ene[13] = new Beta("img/enemy_2.png", x+95, y+155);
		ene[14] = new Beta("img/enemy_2.png", x-95, y+155);
		ene[15] = new Beta("img/enemy_2.png", x+95, y-155);
		ene[16] = new Beta("img/enemy_2.png", x-95, y-155);
		ene[17] = new Beta("img/enemy_2.png", x+170, y-100);
		ene[18] = new Beta("img/enemy_2.png", x+170, y+100);
		ene[19] = new Beta("img/enemy_2.png", x-170, y-100);
		ene[20] = new Beta("img/enemy_2.png", x-170, y+100);
		y+=18;
		ene[21] = new Alpha("img/enemy_1.png", x, y+250);
		ene[22] = new Alpha("img/enemy_1.png", x, y-250);
		ene[23] = new Alpha("img/enemy_1.png", x+300, y);
		ene[24] = new Alpha("img/enemy_1.png", x-300, y);
		ene[25] = new Alpha("img/enemy_1.png", x+125, y+210);
		ene[26] = new Alpha("img/enemy_1.png", x-125, y+210);
		ene[27] = new Alpha("img/enemy_1.png", x+125, y-210);
		ene[28] = new Alpha("img/enemy_1.png", x-125, y-210);
		ene[29] = new Alpha("img/enemy_1.png", x+230, y+120);
		ene[30] = new Alpha("img/enemy_1.png", x+230, y-120);
		ene[31] = new Alpha("img/enemy_1.png", x-230, y+120);
		ene[32] = new Alpha("img/enemy_1.png", x-230, y-120);
	}
	else if(level == 3 && stage == 5)
	{
		x = 350;y = 300;
		numenemy = 33;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+150, y);
		ene[2] = new Gamma("img/enemy_3.png", x-150, y);
		ene[3] = new Gamma("img/enemy_3.png", x, y-100);
		ene[4] = new Gamma("img/enemy_3.png", x, y+100);
		ene[5] = new Gamma("img/enemy_3.png", x+75, y+50);
		ene[6] = new Gamma("img/enemy_3.png", x-75, y+50);
		ene[7] = new Gamma("img/enemy_3.png", x+75, y-50);
		ene[8] = new Gamma("img/enemy_3.png", x-75, y-50);
		x+=10;
		ene[9] = new Beta("img/enemy_2.png", x+250, y);
		ene[10] = new Beta("img/enemy_2.png", x-250, y);
		ene[11] = new Beta("img/enemy_2.png", x, y+200);
		ene[12] = new Beta("img/enemy_2.png", x, y-200);
		ene[13] = new Beta("img/enemy_2.png", x+75, y+150);
		ene[14] = new Beta("img/enemy_2.png", x-75, y+150);
		ene[15] = new Beta("img/enemy_2.png", x+75, y-150);
		ene[16] = new Beta("img/enemy_2.png", x-75, y-150);
		ene[17] = new Beta("img/enemy_2.png", x+150, y-100);
		ene[18] = new Beta("img/enemy_2.png", x+150, y+100);
		ene[19] = new Beta("img/enemy_2.png", x-150, y-100);
		ene[20] = new Beta("img/enemy_2.png", x-150, y+100);
		y+=18;
		ene[21] = new Alpha("img/enemy_1.png", x, y+300);
		ene[22] = new Alpha("img/enemy_1.png", x, y-300);
		ene[23] = new Alpha("img/enemy_1.png", x+350, y);
		ene[24] = new Alpha("img/enemy_1.png", x-350, y);
		ene[25] = new Alpha("img/enemy_1.png", x+125, y+210);
		ene[26] = new Alpha("img/enemy_1.png", x-125, y+210);
		ene[27] = new Alpha("img/enemy_1.png", x+125, y-210);
		ene[28] = new Alpha("img/enemy_1.png", x-125, y-210);
		ene[29] = new Alpha("img/enemy_1.png", x+230, y+120);
		ene[30] = new Alpha("img/enemy_1.png", x+230, y-120);
		ene[31] = new Alpha("img/enemy_1.png", x-230, y+120);
		ene[32] = new Alpha("img/enemy_1.png", x-230, y-120);
	}
	else if(level == 3 && stage == 6)
	{
		x = 200;y = 100;
		numenemy = 40;
		ene = new Enemy*[numenemy];
		ene[0] = new Gamma("img/enemy_3.png", x, y);
		ene[1] = new Gamma("img/enemy_3.png", x+100, y);
		ene[2] = new Gamma("img/enemy_3.png", x-100, y);
		ene[3] = new Gamma("img/enemy_3.png", x, y-100);
		ene[4] = new Gamma("img/enemy_3.png", x+75, y-65);
		ene[5] = new Gamma("img/enemy_3.png", x-75, y-65);
		x+=300;
		ene[6] = new Gamma("img/enemy_3.png", x, y);
		ene[7] = new Gamma("img/enemy_3.png", x+100, y);
		ene[8] = new Gamma("img/enemy_3.png", x-100, y);
		ene[9] = new Gamma("img/enemy_3.png", x, y-100);
		ene[10] = new Gamma("img/enemy_3.png", x+75, y-65);
		ene[11] = new Gamma("img/enemy_3.png", x-75, y-65);
		x=135;y=175;
		ene[12] = new Beta("img/enemy_2.png", x, y);
		ene[13] = new Beta("img/enemy_2.png", x+75, y);
		ene[14] = new Beta("img/enemy_2.png", x+150, y);
		ene[15] = new Beta("img/enemy_2.png", x+225, y);
		ene[16] = new Beta("img/enemy_2.png", x+300, y);
		ene[17] = new Beta("img/enemy_2.png", x+375, y);
		ene[18] = new Beta("img/enemy_2.png", x+450, y);
		ene[19] = new Beta("img/enemy_2.png", x+30, y+100);
		ene[20] = new Beta("img/enemy_2.png", x+125, y+100);
		ene[21] = new Beta("img/enemy_2.png", x+200, y+100);
		ene[22] = new Beta("img/enemy_2.png", x+275, y+100);
		ene[23] = new Beta("img/enemy_2.png", x+350, y+100);
		ene[24] = new Beta("img/enemy_2.png", x+425, y+100);
		x = 225;y = 375; 
		ene[25] = new Alpha("img/enemy_1.png", x, y);
		ene[26] = new Alpha("img/enemy_1.png", x+75, y);
		ene[27] = new Alpha("img/enemy_1.png", x+150, y);
		ene[28] = new Alpha("img/enemy_1.png", x+225, y);
		ene[29] = new Alpha("img/enemy_1.png", x+300, y);
		ene[30] = new Alpha("img/enemy_1.png", x+25, y+75);
		ene[31] = new Alpha("img/enemy_1.png", x+100, y+75);
		ene[32] = new Alpha("img/enemy_1.png", x+175, y+75);
		ene[33] = new Alpha("img/enemy_1.png", x+250, y+75);
		ene[34] = new Alpha("img/enemy_1.png", x+75, y+150);
		ene[35] = new Alpha("img/enemy_1.png", x+150, y+150);
		ene[36] = new Alpha("img/enemy_1.png", x+225, y+150);
		ene[37] = new Alpha("img/enemy_1.png", x+110, y+200);
		ene[38] = new Alpha("img/enemy_1.png", x+185, y+200);
		ene[39] = new Alpha("img/enemy_1.png", x+145, y+250);
	}
	allenemydestroied = false;
	allenemyinplace = false;
}

void checkbulletcollision()
{
	if(level == 1 && stage ==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<=3)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
									score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<=7)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 1 && stage ==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<7)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 1 && stage ==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<6)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 2 && stage ==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 2 && stage ==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 2 && stage ==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<7)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<13)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
    else if(level == 3 && stage ==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<10)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<18)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
    else if(level == 3 && stage ==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<18)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==4)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<21)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==5)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<21)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==6)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<12)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
									delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else if(i<25)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
									delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
									delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						p->db(j);
        					}
        				}
				}
			}
	}
}

void enemydropbomb(float time)
{
	int alpha=0,beta=0,gamma=0;
	if(ene!=nullptr)
	{
		if(level == 1 && stage ==1)
		{
			gamma = rand()%4;
			beta = (rand()%4) + 4;
			alpha = (rand()%4) +8;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 1 && stage ==2)
		{
			gamma = rand()%8;
			beta = (rand()%8) + 3;
			alpha = (rand()%8) +6;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 1 && stage ==3)
		{
			gamma = rand()%6;
			beta = (rand()%6) + 5;
			alpha = (rand()%6) +11;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 2 && stage ==1)
		{
			gamma = rand()%5;
			beta = (rand()%5) + 4;
			alpha = (rand()%5) +9;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 2 && stage ==2)
		{
			gamma = rand()%5;
			beta = (rand()%5) + 5;
			alpha = (rand()%5) +11;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 2 && stage ==3)
		{
			gamma = rand()%7;
			beta = (rand()%7) + 6;
			alpha = (rand()%7) + 15;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 3 && stage ==1)
		{
			gamma = rand()%5;
			beta = (rand()%5) + 5;
			alpha = (rand()%5) + 15;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 3 && stage ==2)
		{
			gamma = rand()%11;
			beta = (rand()%11) + 7;
			alpha = (rand()%11) + 10;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 3 && stage ==3)
		{
			gamma = rand()%9;
			beta = (rand()%9) + 9;
			alpha = (rand()%9) + 27;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 3 && stage ==4)
		{
			gamma = rand()%9;
			beta = (rand()%9) + 12;
			alpha = (rand()%9) + 24;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 3 && stage ==5)
		{
			gamma = rand()%9;
			beta = (rand()%9) + 12;
			alpha = (rand()%9) + 24;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
		else if(level == 3 && stage ==6)
		{
			gamma = rand()%12;
			beta = (rand()%12) + 13;
			alpha = (rand()%12) + 28;
			ene[gamma]->fire(time,level);
			ene[beta]->fire(time,level);
			ene[alpha]->fire(time,level);
		}
	}
}

void checkboombcollision()
{
	for(int i = 0;i<numenemy;i++)
	{
		for(int j = 0;j<numbomb;j++)
		{
			if(ene[i]->b[j]->getpx()>=p->getpx() && ene[i]->b[j]->getpx()<=p->getpx()+72 && ene[i]->b[j]->getpy()>=p->getpy()-25 && ene[i]->b[j]->getpy()<=p->getpy()+45)
			{
				health-=1;
				if(health==0)
				{
					delete p;
					p = new Player;
				}
				ene[i]->db(j);
			}
		}
	}
}

void checkspaceshipcollision()
{
	if(level == 1 && stage ==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<=3)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<=7)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 1 && stage==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<7)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<11)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 1 && stage==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<7)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<11)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 2 && stage==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<9)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 2 && stage==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<11)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 2 && stage==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<7)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<13)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 3 && stage==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<10)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 3 && stage==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<11)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<18)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 3 && stage==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<18)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 3 && stage==4)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<21)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 3 && stage==5)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<21)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
	else if(level == 3 && stage==6)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<12)
				{
        			if((p->getpy()<ene[i]->getpy()+64 && p->getpy()>ene[i]->getpy()-78) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+58))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Gamma;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else if(i<25)
				{
        			if((p->getpy()<ene[i]->getpy()+71 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-56 && p->getpx()<ene[i]->getpx()+38))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Beta;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
				else
				{
        			if((p->getpy()<ene[i]->getpy()+52 && p->getpy()>ene[i]->getpy()-70) && (p->getpx()>ene[i]->getpx()-51 && p->getpx()<ene[i]->getpx()+35))
        			{
						health-=1;
						delete ene[i];
						ene[i] = new Alpha;
						score+=ene[i]->score;
						if(health==0)
						{
							delete p;
							p = new Player;
						}
        			}
				}
			}
	}
}

void displayhighscore()
{
    //

    Texture bg, gold,silver,bronze;
    bg.loadFromFile("img/highscorebackground.png");
    background.setTexture(bg);
	gold.loadFromFile("img/goldbadge.png");
	silver.loadFromFile("img/silverbadge.png");
	bronze.loadFromFile("img/bronzebadge.png");
	Sprite Gold,Silver,Bronze;
	Gold.setTexture(gold);
	Gold.setPosition(600,100);
	Gold.setScale(0.15,0.15);
	Silver.setTexture(silver);
	Silver.setPosition(600,200);
	Silver.setScale(0.3,0.3);
	Bronze.setTexture(bronze);
	Bronze.setPosition(585,300);
	Bronze.setScale(0.15,0.15);
    
    //

	Text score1(lines1,font,30);
	score1.setPosition(150, 100);
	Text score2(lines2,font,30);
	score2.setPosition(150, 200);
	Text score3(lines3,font,30);
	score3.setPosition(150, 300);
	Text returnback("BACK TO MENU", font, 50);
    returnback.setPosition(250, 600);

	RenderWindow window(VideoMode(780, 780), title);

    while (window.isOpen())
	{
		Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=643 && mousePositiony>=615 && mousePositiony<=654)
                {
                    window.close();
                }
            }
        }

		window.clear(Color::Black); //clears the screen
	    window.draw(background);  // setting background
		window.draw(score1);
		window.draw(score2);
		window.draw(score3);
		window.draw(Gold);
		window.draw(Silver);
		window.draw(Bronze);
		window.draw(returnback);
		window.display();
	}
}

void takescore()
{
	health = 3;
	p->destroied= false;
	int count = 0, comma = 0, score1 = 0, score2 = 0, score3 = 0, temp = score;
	char num;
	string numscore ="\0", toswap, name;
	while(lines1[count] !='\0')
	{
		count+=1;
		if(lines1[count]==',')
		comma=count+1;
	}
	for(int i = comma,j=1;i<count;i++)
	{
		num =lines1[i];
		if(num!='0')
		score1 += (num -'0')*j;
		else
		score1*=10;
		if(j==1)
		j = 10;
		else
		j = pow(j,2);
	}
	count  = 0;
	while(lines2[count] !='\0')
	{
		count+=1;
		if(lines2[count]==',')
		comma=count+1;
	}
	for(int i = comma,j=1;i<count;i++)
	{
		num =lines2[i];
		if(num!='0')
		score2 += (num -'0')*j;
		else
		score2*=10;
		if(j==1)
		j = 10;
		else
		j = pow(j,2);
	}
	count  = 0;
	while(lines3[count] !='\0')
	{
		count+=1;
		if(lines3[count]==',')
		comma=count+1;
	}
	for(int i = comma,j=1;i<count;i++)
	{
		num =lines3[i];
		if(num!='0')
		score3 += (num -'0')*j;
		else
		score3*=10;
		if(j==1)
		j = 10;
		else
		j = pow(j,2);
	}
	count  = 0;

	for(int i = 0;temp!=0;i++)
	{
		num = (temp%10 + '0');
		temp=temp/10;

		toswap = numscore;

		for(int j = 0;j!=i+1;j++)
		{
			if(j==0)
			numscore=num;
			else
			numscore+=toswap[j-1];
		}

	}


	//

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::White);
	text.setPosition(250, 200);
	Text scoreheader("Your score is:", font,30);
	scoreheader.setPosition(250,300);
	Text toshowscore(numscore,font, 30);
	toshowscore.setPosition(250, 400);
	Text returnback("BACK TO MENU", font, 50);
    returnback.setPosition(250, 600);
	Text entername("Enter your name:", font, 30);
	entername.setPosition(250, 100);

	//

    Texture bg, gold,silver,bronze;
    bg.loadFromFile("img/enterscorebackground.jpg");
  	background.setTexture(bg);
	gold.loadFromFile("img/goldbadge.png");
	silver.loadFromFile("img/silverbadge.png");
	bronze.loadFromFile("img/bronzebadge.png");
	Sprite Gold,Silver,Bronze;
	Gold.setTexture(gold);
	Gold.setPosition(600,400);
	Gold.setScale(0.15,0.15);
	Silver.setTexture(silver);
	Silver.setPosition(600,400);
	Silver.setScale(0.3,0.3);
	Bronze.setTexture(bronze);
	Bronze.setPosition(585,400);
	Bronze.setScale(0.15,0.15);
    
 	//

	if(score>score1)
	{
			RenderWindow window(VideoMode(780, 780), title);
			{
				while (window.isOpen())
				{
					Event event;
       				while (window.pollEvent(event))
        			{
         			    if (event.type == Event::Closed)
         			    {
          			      window.close();
           				}
						if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
           				{
                			float mousePositionx= Mouse::getPosition(window).x;
                			float mousePositiony= Mouse::getPosition(window).y;
                			if (mousePositionx>=250 && mousePositionx<=643 && mousePositiony>=615 && mousePositiony<=654)
                			{
                			    window.close();
                			}
           				}


						if (event.type == Event::TextEntered)
            			{
               				if (event.text.unicode == 13) // if Enter key is pressed
                			{
                  				text.setString(name);
								window.close();
               				}
                			else if (event.text.unicode == 8) // if Backspace key is pressed
                			{
                    			if (name!="\0") 
								{
									while(name[count]!='\0')
									{
										count+=1;
									}
									name[count] = '\0';
									count = 0;
								}
								name.pop_back();
               				}
                			else
               		 		{
                 		 	  name += static_cast<char>(event.text.unicode);
              		 		}

               				text.setString( name );
           				}



        			}

					Gamefile.open("Highscores.txt", fstream::out);
					Gamefile<<name<<","<<numscore<<endl;
					Gamefile<<lines1<<endl;
					Gamefile<<lines2<<endl;
					Gamefile.close();


					window.clear(Color::Black); //clears the screen
	    			window.draw(background);  // setting background
					window.draw(entername);
					window.draw(text);
					window.draw(scoreheader);
					window.draw(toshowscore);
					window.draw(returnback);
					if(score>500)
					window.draw(Bronze);
					else if(score>1000)
					window.draw(Silver);
					else if(score>5000)
					window.draw(Gold);
					window.display();
				}
			}
	}
	else if(score>score2)
	{
					RenderWindow window(VideoMode(780, 780), title);
			{
				while (window.isOpen())
				{
					Event event;
       				while (window.pollEvent(event))
        			{
         			    if (event.type == Event::Closed)
         			    {
          			      window.close();
           				}
						if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
           				{
                			float mousePositionx= Mouse::getPosition(window).x;
                			float mousePositiony= Mouse::getPosition(window).y;
                			if (mousePositionx>=250 && mousePositionx<=643 && mousePositiony>=615 && mousePositiony<=654)
                			{
                			    window.close();
                			}
           				}


						if (event.type == Event::TextEntered)
            			{
               				if (event.text.unicode == 13) // if Enter key is pressed
                			{
                  				text.setString(name);
								window.close();
               				}
                			else if (event.text.unicode == 8) // if Backspace key is pressed
                			{
                    			if (name!="\0") 
								{
									while(name[count]!='\0')
									{
										count+=1;
									}
									name[count] = '\0';
									count = 0;
								}
								name.pop_back();
               				}
                			else
               		 		{
                 		 	  name += static_cast<char>(event.text.unicode);
              		 		}

               				text.setString( name );
           				}



        			}

										Gamefile.open("Highscores.txt", fstream::out);
															Gamefile<<lines1<<endl;
					Gamefile<<name<<","<<numscore<<endl;
					Gamefile<<lines2<<endl;
					Gamefile.close();
					
					window.clear(Color::Black); //clears the screen
	    			window.draw(background);  // setting background
					window.draw(entername);
					window.draw(text);
					window.draw(scoreheader);
					window.draw(toshowscore);
					window.draw(returnback);
					if(score>500)
					window.draw(Bronze);
					else if(score>1000)
					window.draw(Silver);
					else if(score>5000)
					window.draw(Gold);
					window.display();
				}
			}
	}
	else if(score>score3)
	{
					RenderWindow window(VideoMode(780, 780), title);
			{
				while (window.isOpen())
				{
					Event event;
       				while (window.pollEvent(event))
        			{
         			    if (event.type == Event::Closed)
         			    {
          			      window.close();
           				}
						if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
           				{
                			float mousePositionx= Mouse::getPosition(window).x;
                			float mousePositiony= Mouse::getPosition(window).y;
                			if (mousePositionx>=250 && mousePositionx<=643 && mousePositiony>=615 && mousePositiony<=654)
                			{
                			    window.close();
                			}
           				}


						if (event.type == Event::TextEntered)
            			{
               				if (event.text.unicode == 13) // if Enter key is pressed
                			{
                  				text.setString(name);
								window.close();
               				}
                			else if (event.text.unicode == 8) // if Backspace key is pressed
                			{
                    			if (name!="\0") 
								{
									while(name[count]!='\0')
									{
										count+=1;
									}
									name[count] = '\0';
									count = 0;
								}
								name.pop_back();
               				}
                			else
               		 		{
                 		 	  name += static_cast<char>(event.text.unicode);
              		 		}

               				text.setString( name );
           				}



        			}

														Gamefile.open("Highscores.txt", fstream::out);
															Gamefile<<lines1<<endl;
																				Gamefile<<lines2<<endl;
					Gamefile<<name<<","<<numscore<<endl;
					Gamefile.close();

					window.clear(Color::Black); //clears the screen
	    			window.draw(background);  // setting background
					window.draw(entername);
					window.draw(text);
					window.draw(scoreheader);
					window.draw(toshowscore);
					window.draw(returnback);
					if(score>500)
					window.draw(Bronze);
					else if(score>1000)
					window.draw(Silver);
					else if(score>5000)
					window.draw(Gold);
					window.display();
				}
			}
	}
	else
	{
			RenderWindow window(VideoMode(780, 780), title);
			{
				while (window.isOpen())
				{
					Event event;
       				while (window.pollEvent(event))
        			{
         			    if (event.type == Event::Closed)
         			    {
          			      window.close();
           				}
						if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
           				{
                			float mousePositionx= Mouse::getPosition(window).x;
                			float mousePositiony= Mouse::getPosition(window).y;
                			if (mousePositionx>=250 && mousePositionx<=643 && mousePositiony>=615 && mousePositiony<=654)
                			{
                			    window.close();
                			}
           				}


						if (event.type == Event::TextEntered)
            			{
               				if (event.text.unicode == 13) // if Enter key is pressed
                			{
                  				text.setString(name);
                   				name.clear();
								window.close();
               				}
                			else if (event.text.unicode == 8) // if Backspace key is pressed
                			{
                    			if (name!="\0") 
								{
									while(name[count]!='\0')
									{
										count+=1;
									}
									name[count] = '\0';
									count = 0;
								}
								name.pop_back();
               				}
                			else
               		 		{
                 		 	  name += static_cast<char>(event.text.unicode);
              		 		}

               				text.setString( name );
           				}



        			}

					window.clear(Color::Black); //clears the screen
	    			window.draw(background);  // setting background
					window.draw(entername);
					window.draw(text);
					window.draw(scoreheader);
					window.draw(toshowscore);
					window.draw(returnback);
					if(score>500)
					window.draw(Bronze);
					else if(score>1000)
					window.draw(Silver);
					else if(score>5000)
					window.draw(Gold);
					window.display();
				}
			}
	}


    //

}

void generatemonster()
{
	if(mon!=NULL)
	{
		delete mon[0];
		delete mon[1];
	}
	mon[0] = new Monster("img/monster.png",182,35,0.31,0.31);
	mon[1] = new Monster("img/monster1.png",190,40,0.6,0.6);
}

void bulletcollisionmonster()
{
	if(nummonster == 0)
	{
		for(int i = 0;i<numbullet;i++)
		{
			if((p->b[i]->getpx()<mon[nummonster]->getpx()+410 && p->b[i]->getpx()>mon[nummonster]->getpx()-5) && (p->b[i]->getpy()<mon[nummonster]->getpy()+450 && p->b[i]->getpy()>mon[nummonster]->getpy()))
			{
				mon[nummonster]->health-=1;
				if(mon[nummonster]->health==0)
				{
					delete mon[nummonster];
					mon[nummonster]= new Monster;
					score+=mon[nummonster]->score;
					monsterspawn=false;
				}
				p->db(i);
			}
		}
	}
	else
	{
		for(int i = 0;i<numbullet;i++)
		{
			if((p->b[i]->getpx()<mon[nummonster]->getpx()+360 && p->b[i]->getpx()>mon[nummonster]->getpx()+20) && (p->b[i]->getpy()<mon[nummonster]->getpy()+400 && p->b[i]->getpy()>mon[nummonster]->getpy()))
			{
				mon[nummonster]->health-=1;
				if(mon[nummonster]->health==0)
				{
					delete mon[nummonster];
					mon[nummonster]= new Monster;
					score+=mon[nummonster]->score;
					monsterspawn=false;
				}
				p->db(i);
			}
		}
	}
}

void spaceshipaddoncollision()
{
	for(int i = 0;i<10;i++)
	{
			if(add[i]->getpx()>=p->getpx()-12 && add[i]->getpx()<=p->getpx()+62 && add[i]->getpy()>=p->getpy()-25 && add[i]->getpy()<=p->getpy()+80)
			{
				if(health<5 && add[i]->x==1)
				{
					health+=1;
					delete add[i];
					add[i] = new Addon;
				}
				else if(add[i]->x==-1)
				{
					health = 0;
					delete add[i];
					add[i] = new Addon;
				}
				else if(add[i]->x==-10)
				{
					firebullet = true;
					delete add[i];
					add[i] = new Addon;
				}
				else if(add[i]->x==-100)
				{
					immunity  =true;
					delete add[i];
					add[i] = new Addon;
				}
			}
	}
}

void checkfirebulletcollision()
{
	if(level == 1 && stage ==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<=3)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
									score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<=7)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 1 && stage ==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<7)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 1 && stage ==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<6)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 2 && stage ==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 2 && stage ==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 2 && stage ==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<7)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<13)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
    else if(level == 3 && stage ==1)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<5)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<10)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==2)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<11)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<18)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
    else if(level == 3 && stage ==3)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<18)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==4)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<21)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==5)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<9)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<21)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
																		delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
	else if(level == 3 && stage ==6)
	{
			for(int i = 0;i<numenemy;i++)
			{
				if(i<12)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+50 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-5 && p->b[j]->getpx()<ene[i]->getpx()+85))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
									delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Gamma;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else if(i<25)
				{
        				for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+55 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
									delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Beta;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
				else
				{
					    for(int j = 0;j<numbullet;j++)
        				{
        					if((p->b[j]->getpy()<ene[i]->getpy()+45 && p->b[j]->getpy()>ene[i]->getpy()-30) && (p->b[j]->getpx()>ene[i]->getpx()-7 && p->b[j]->getpx()<ene[i]->getpx()+65))
        					{
								ene[i]->health-=1;
								if(ene[i]->health==0)
								{
									delete []ene[i]->b;
        							delete ene[i];
        							ene[i] = new Alpha;
																		score+=ene[i]->score;
								}
        						
        					}
        				}
				}
			}
	}
}

void checkcollisionmonsterbomb()
{
		for(int j = 0;j<numbomb;j++)
		{
			if(j%4!=0 && mon[nummonster]->b[j]->getpx()>=p->getpx() && mon[nummonster]->b[j]->getpx()<=p->getpx()+72 && mon[nummonster]->b[j]->getpy()>=p->getpy()-25 && mon[nummonster]->b[j]->getpy()<=p->getpy()+45)
			{
				health-=1;
				if(health==0)
				{
					delete p;
					p = new Player;
				}
				mon[nummonster]->db(j);
			}
		}
		for(int j = 4;j<numbomb;j+=5)
		{
			if(mon[nummonster]->b[j]->getpx()+30>=p->getpx()-30 && mon[nummonster]->b[j]->getpx()<=p->getpx()+25 && mon[nummonster]->b[j]->getpy()+800<=p->getpy()+25)
			{
				health=0;
				if(health==0)
				{
					delete p;
					p = new Player;
				}
			}
		}
}

void generatedragon()
{
	if(dra!=NULL)
	{
		delete dra[0];
		delete dra[1];
		delete dra[2];
	}
	dra[0] = new Dragon("img/dragon.png",p->getpx()-77,40,1.0,1.0);
	dra[1] = new Dragon("img/dragon1.png",p->getpx()-77,40,1.0,1.0);
	dra[2] = new Dragon("img/dragon2.png",p->getpx()-48,40,0.3,0.3);
}

void firecollisionspaceship()
{
		for(int j = 0;j<numbomb;j++)
		{
			if(dra[numdragon]->b[j]->getpx()>=p->getpx() && dra[numdragon]->b[j]->getpx()<=p->getpx()+72 && dra[numdragon]->b[j]->getpy()>=p->getpy()-25 && dra[numdragon]->b[j]->getpy()<=p->getpy()+45)
			{
				health=0;
				if(health==0)
				{
					delete p;
					p = new Player;
				}
				dra[numdragon]->db(j);
			}
		}
}

};
