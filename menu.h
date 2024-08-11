#include "game.h"
class Menu{
public:

//add menu attributes here
Sprite background; //Game background sprite
Texture bg_texture;
Font font;
Game* g; 


//
Menu()
{
//constructors body
bg_texture.loadFromFile("img/menubackground.png");
background.setTexture(bg_texture);
background.setScale(2, 1.6);
font.loadFromFile("chrusty-rock-font/ChrustyRock-ORLA.ttf");

//
}

void display_menu()
{
    delete g;
    g = new Game;
    g->ene[0]->timera =0;
    g->ene[0]->timerb =0;
    g->ene[0]->timerg =0;
    g->ene[0]->timerm =0;
    g->ene[0]->lasertimer =0;
    //

    background.setTexture(bg_texture);

    //
    Text play("PLAY", font, 50);
    play.setPosition(250, 100);
    Text level("LEVEL", font, 50);
    level.setPosition(250, 200);
    Text highscore("HIGHEST SCORE", font, 50);
    highscore.setPosition(250, 300);
    Text instruction("INSTRUCTION", font, 50);
    instruction.setPosition(250, 400);
    Text gameover("GAME OVER", font, 50);
    gameover.setPosition(250, 600);

//display menu screen here
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

            // Check if the play button was clicked
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=382 && mousePositiony>=115 && mousePositiony<=154)
                {
                    window.close();
                    g->start_game();
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=420 && mousePositiony>=215 && mousePositiony<=255)
                {
                    window.close();
                    show_level();
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=663 && mousePositiony>=315 && mousePositiony<=355)
                {
                    window.close();
                    g->displayhighscore();
                    display_menu();
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=604 && mousePositiony>=415 && mousePositiony<=455)
                {
                    window.close();
                    show_instruction();
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=545 && mousePositiony>=615 && mousePositiony<=654)
                {
                     window.close();
                }
            }
            
        }

        if(g->health<=0)
        {
            window.close();
            g->takescore();
            display_menu();
        }


	    window.clear(Color::Black); //clears the screen
	    window.draw(background);  // setting background
        window.draw(play);
        window.draw(level);
        window.draw(highscore);
        window.draw(instruction);
        window.draw(gameover);
	    window.display();  //Displying all the sprites

    }

}

void show_level()
{
    //

    Texture bg;
    bg.loadFromFile("img/levelbackground.jpg");
    background.setTexture(bg);
    
    //

	Text level1("LEVEL I", font, 50);
    level1.setPosition(250, 100);
    Text level2("LEVEL II", font, 50);
    level2.setPosition(250, 200);
    Text level3("LEVEL III", font, 50);
    level3.setPosition(250, 300);
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

            // Check if the play button was clicked
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=445 && mousePositiony>=115 && mousePositiony<=154)
                {
                    window.close();
                    g->start_game(1,0);
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=463 && mousePositiony>=215 && mousePositiony<=255)
                {
                    window.close();
                    g->start_game(2,0);
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=481 && mousePositiony>=315 && mousePositiony<=355)
                {
                    window.close();
                    g->start_game(3,0);
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=630 && mousePositiony>=615 && mousePositiony<=654)
                {
                    window.close();
                    display_menu();
                }
            }
        }


	    window.clear(Color::Black); //clears the screen
	    window.draw(background);  // setting background
        window.draw(level1);
        window.draw(level2);
        window.draw(level3);
        window.draw(returnback);
	    window.display();  //Displying all the sprites

    }
}

void show_instruction()
{
    //

    Texture bg;
    bg.loadFromFile("img/instructionbackground.png");
    background.setTexture(bg);
    
    //

    Text heading("Read the Instructions below", font, 30);
    heading.setPosition(250, 100);
    Text up("Press Up Key to move up", font, 20);
    up.setPosition(250, 150);
    Text down("Press Down Key to move down", font, 20);
    down.setPosition(250, 200);
    Text right("Press Right Key to move right", font, 20);
    right.setPosition(250, 250);
    Text left("Press left Key to move left", font, 20);
    left.setPosition(250, 300);
    Text space("Press Space Key to shoot", font, 20);
    space.setPosition(250, 350);
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

            // Check if the play button was clicked
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                float mousePositionx= Mouse::getPosition(window).x;
                float mousePositiony= Mouse::getPosition(window).y;
                if (mousePositionx>=250 && mousePositionx<=630 && mousePositiony>=615 && mousePositiony<=654)
                {
                    window.close();
                    display_menu();
                }
            }
        }


	    window.clear(Color::Black); //clears the screen
	    window.draw(background);  // setting background
        window.draw(heading);
        window.draw(up);
        window.draw(down);
        window.draw(right);
        window.draw(left);
        window.draw(space);
        window.draw(returnback);
	    window.display();  //Displying all the sprites

    }
}


};
