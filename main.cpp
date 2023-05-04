#include <SFML/Graphics.hpp>
#include <time.h>
#include <iomanip>
#include <fstream> // leitura de arquivo
#include <iostream>
#include "model.cpp"
#include "controller.cpp"


int main()
{
    srand(time(0));     

    sf::RenderWindow window(sf::VideoMode(320, 480), "The Game!");
    sf::Texture t1,t2;
    t1.loadFromFile("assets/tiles2.png");
    t2.loadFromFile("assets/background1.png");
    sf::Sprite s(t1), background(t2);

    int pieceID = createTetromino();
    int highscore = getHighScore();

    int dx=0; bool rotate=0;
    float timer=0,delay=0.3; 
    sf::Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        sf::Event e;
        
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyPressed)
              if (e.key.code==sf::Keyboard::Up || e.key.code==sf::Keyboard::Space) rotate=true;
              else if (e.key.code==sf::Keyboard::Left) dx=-1;
              else if (e.key.code==sf::Keyboard::Right) dx=1;
        }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay=0.05;
    if (!isGameOver()){
      //// <- Move -> ///
      for(int i=0;i<4;i++){
        next[i]=current[i];
        current[i].x+=dx;
      }
      if (!check()) {
        for(int i=0;i<4;i++){
          current[i]=next[i];
        }
      }

      //////Rotate//////
      if (rotate)
        {
          Point p = current[1]; // Center of rotation
          for (int i=0;i<4;i++)
            {
              int x = current[i].y-p.y;
              int y = current[i].x-p.x;
              current[i].x = p.x - x;
              current[i].y = p.y + y;
            }
            if (!check()) for (int i=0;i<4;i++) current[i]=next[i];
        }

      ///////Tick//////
      if (timer>delay){
          for (int i=0;i<4;i++) { next[i]=current[i]; current[i].y+=1; }
          if (!check())
          {
          for (int i=0;i<4;i++){
            field[next[i].y][next[i].x]=pieceID;
          }
          pieceID = createTetromino(); // Gera uma nova peça
          }

          timer=0;
        }
   }

    ///////check lines//////////
    int k=HEIGHT-1;
    for (int i=HEIGHT-1;i>0;i--)
    {
        int count=0;
        for (int j=0;j<WIDTH;j++){
            if (field[i][j]) count++;
            field[k][j]=field[i][j];
        }
        if (count<WIDTH) k--;
        else {score++;}
    }

    dx=0; rotate=0; 
    delay = 0.3 - (score * 0.01); // Difficulty based on current score
    if (delay < 0.1) {
      delay = 0.1; // Set a minimum delay of 0.1 seconds
    }

    /////////draw//////////
    window.clear(sf::Color::White);    
    window.draw(background);
          
    for (int i=0;i<HEIGHT;i++)
     for (int j=0;j<WIDTH;j++)
       {
         if (field[i][j]==0) continue;
         s.setTextureRect(sf::IntRect(field[i][j]*SIZE,0,SIZE,SIZE));
         s.setPosition(j*SIZE,i*SIZE);
         s.move(28,31); //offset
         window.draw(s);
       }

    for (int i=0;i<4;i++)
      {
        s.setTextureRect(sf::IntRect(pieceID*SIZE,0,SIZE,SIZE));
        s.setPosition(current[i].x*SIZE,current[i].y*SIZE);
        s.move(28,31); //offset
        window.draw(s);
      }


    if (isGameOver()) {
      saveNewHighScore(score);
      displayGameOver(window);
      highscore = getHighScore();
    }

    displayHighScore(window, highscore);
    displayScore(window, score);
    window.display();
    }

    return 0;
}
