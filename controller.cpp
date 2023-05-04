void setText(sf::RenderWindow& window, int value, int x, int y) {
    // Formatando o objeto
    sf::Font font;
    font.loadFromFile("assets/Tetris.ttf");
    sf::Text text(std::to_string(value), font, 16);
    text.setFillColor(sf::Color(139, 87, 96, 255));
    text.setPosition(x, y);
    // Formatando o valor para que tenha 03 dígitos
    std::ostringstream ss;
    ss << std::setw(3) << std::setfill('0') << value;
    text.setString(ss.str());
    // Desenha
    window.draw(text);
}

void displayHighScore(sf::RenderWindow& window, int highscore)
{
    setText(window, highscore, 261, 74);
}

void displayScore(sf::RenderWindow& window, int score)
{
    setText(window, score, 260, 114);
}

void displayGameOver(sf::RenderWindow& window)
{
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