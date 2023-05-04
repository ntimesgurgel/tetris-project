#include <SFML/Graphics.hpp>
#include <time.h>
#include <iomanip>
#include <fstream> // leitura de arquivo
#include <iostream>

// using namespace std;

const int HEIGHT = 20;
const int WIDTH = 10;
int Score = 0;

int field[HEIGHT][WIDTH] = {0};

struct Point
{int x,y;} current[4], next[4];

int figures[7][4] =
{
    1,3,5,7,
    0,2,3,5,
    1,3,2,4,
    1,3,2,5,
    0,1,3,5,
    1,3,5,4,
    0,1,2,3,
};

bool check()
{
   for (int i=0;i<4;i++){
      if (current[i].x<0 || current[i].x>=WIDTH|| current[i].y>=HEIGHT){
        return 0;
      } else if (field[current[i].y][current[i].x]){
        return 0;
      }
    }

   return 1;
};


void displayHighScore(sf::RenderWindow& window, int highscore)
{
    // Load font
    sf::Font font;
    font.loadFromFile("assets/Tetris.ttf");

    // Create text object
    sf::Text text(std::to_string(highscore), font, 16);
    text.setFillColor(sf::Color(139, 87, 96, 255));
    text.setPosition(261, 74);

    // Format highscore with leading zeros
    std::ostringstream ss;
    ss << std::setw(3) << std::setfill('0') << highscore;
    text.setString(ss.str());

    // Draw text to window
    window.draw(text);
}

void displayScore(sf::RenderWindow& window, int score)
{
    // Load font
    sf::Font font;
    font.loadFromFile("assets/Tetris.ttf");

    // Create text object
    sf::Text text(std::to_string(score), font, 16);
    text.setFillColor(sf::Color(139, 87, 96, 255));
    text.setPosition(260, 114);

    // Format score with leading zeros
    std::ostringstream ss;
    ss << std::setw(3) << std::setfill('0') << score;
    text.setString(ss.str());

    // Draw text to window
    window.draw(text);
}

void displayGameOver(sf::RenderWindow& window)
{
    // Load font
    sf::Font font;
    font.loadFromFile("assets/Tetris.ttf");

    // Create text object
    sf::Text text("GAME OVER", font, 30);
    text.setFillColor(sf::Color(139, 87, 96, 255));
    text.setPosition(20, 425);
    // Draw text to window
    sf::RectangleShape rectangle(sf::Vector2f(320, 480));
    rectangle.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(rectangle);
    window.draw(text);

}


bool checkGameOver() {
  return !check() && current[0].y == 0;
}

int main()
{
    srand(time(0));     

    sf::RenderWindow window(sf::VideoMode(320, 480), "The Game!");

    sf::Texture t1,t2,t3;
    t1.loadFromFile("assets/tiles2.png");
    t2.loadFromFile("assets/background1.png");
    // t3.loadFromFile("assets/frame.png");

    sf::Sprite s(t1), background(t2);

    int dx=0; bool rotate=0; int colorNum=1;
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
    if (!checkGameOver()){
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
          for (int i=0;i<4;i++) field[next[i].y][next[i].x]=colorNum;

          colorNum=1+rand()%7;
          int n=rand()%7;
          for (int i=0;i<4;i++)
            {
              // Gerando nova peça
              current[i].x = figures[n][i] % 2 + (WIDTH / 2) - 1; // Must to be changed
              current[i].y = figures[n][i] / 2;
            }
          }

          timer=0;
        }
   }

    ///////check lines//////////
    int k=HEIGHT-1;
    for (int i=HEIGHT-1;i>0;i--)
    {
        int count=0;
        for (int j=0;j<WIDTH;j++)
        {
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
        s.setTextureRect(sf::IntRect(colorNum*18,0,18,18));
        s.setPosition(current[i].x*18,current[i].y*18);
        s.move(28,31); //offset
        window.draw(s);
      }

    //
    
    // Highscore
    int highscore;
    std::ifstream arquivo("arquivo.txt");
    if (arquivo.is_open()) {
        // Se o arquivo existe, mostra o seu conteúdo como inteiro
        arquivo >> highscore;
        arquivo.close();
    } else {
        // Se o arquivo não existe, cria um novo arquivo com o valor 0
        std::ofstream novo_arquivo("arquivo.txt");
        novo_arquivo << "0";
        novo_arquivo.close();
    }

    if (checkGameOver()) {
      displayGameOver(window);
      if (Score > highscore){
        std::ofstream novo_arquivo("arquivo.txt");
        novo_arquivo << Score;
        novo_arquivo.close();}
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
