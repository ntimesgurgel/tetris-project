template<typename T>
void setText(sf::RenderWindow& window, T value, int x, int y) {
    // Formatando o texto
    sf::Font font;
    font.loadFromFile("assets/Tetris.ttf");
    sf::Text text(std::to_string(value), font, 16);
    
    // Formatando o valor para que tenha 03 dígitos
    std::ostringstream ss;
    ss << std::setw(3) << std::setfill('0') << value;
    text.setString(ss.str());

    // Desenhando
    text.setFillColor(sf::Color(139, 87, 96, 255));
    text.setPosition(x, y);
    window.draw(text);
}

void displayHighScore(sf::RenderWindow& window, int highscore){
    setText(window, highscore, 261, 74);
}

void displayScore(sf::RenderWindow& window, int score){
    setText(window, score, 260, 114);
}

void displayGameOver(sf::RenderWindow& window){
    sf::Font font;
    font.loadFromFile("assets/Tetris.ttf");
    sf::Text text("GAME OVER", font, 30);
    text.setFillColor(sf::Color(139, 87, 96, 255));
    text.setPosition(20, 425);
    sf::RectangleShape rectangle(sf::Vector2f(320, 480));
    rectangle.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(rectangle);
    window.draw(text);

}

int getHighScore() {
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
    return highscore;
}

void saveNewHighScore(int value) {
    if (value > getHighScore()){
        std::ofstream arquivo("arquivo.txt");
        arquivo << Score;
        arquivo.close();}
}

bool check(){
   for (int i=0;i<4;i++){
      if (current[i].x<0 || current[i].x>=WIDTH|| current[i].y>=HEIGHT){
        return 0;
      } else if (field[current[i].y][current[i].x]){
        return 0;
      }
    }

   return 1;
};

bool isGameOver() {
  return !check() && current[0].y == 0;
}

int createTetromino() {
  int n=rand()%7;
  int pieceID= n + 1;
  for (int i=0;i<4;i++)
    {
      // Gerando nova peça
      current[i].x = figures[n][i] % 2 + (WIDTH / 2) - 1; // Must to be changed
      current[i].y = figures[n][i] / 2;
    }
    return pieceID;
}