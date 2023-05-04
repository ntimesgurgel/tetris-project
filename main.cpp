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

    int dx=0; bool rotate=0;
    float timer=0,delay=0.3; 

    sf::Clock clock;

    int pieceID = createTetromino();

    int highscore = getHighScore();
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
          Point p = current[1]; //center of rotation
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
          pieceID = createTetromino();
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
        else {Score++;}
    }

    dx=0; rotate=0; 
    delay = 0.3 - (Score * 0.01); // Difficulty based on current score
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
         s.setTextureRect(sf::IntRect(field[i][j]*18,0,18,18));
         s.setPosition(j*18,i*18);
         s.move(28,31); //offset
         window.draw(s);
       }

    for (int i=0;i<4;i++)
      {
        s.setTextureRect(sf::IntRect(pieceID*18,0,18,18));
        s.setPosition(current[i].x*18,current[i].y*18);
        s.move(28,31); //offset
        window.draw(s);
      }


    if (isGameOver()) {
      saveNewHighScore(Score);
      displayGameOver(window);
    }

    displayHighScore(window, highscore);

    
  // O programa tenta abrir o arquivo "arquivo.txt" utilizando a classe ifstream. 
  // Se o arquivo existir, o seu conteúdo é lido e armazenado na variável conteudo.
  // Se o arquivo não existir, o programa cria um novo arquivo utilizando a classe ofstream e escreve o valor 0 nele.

    displayScore(window, Score);
    window.display();
    }

    return 0;
}
