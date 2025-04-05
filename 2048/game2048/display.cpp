#include "library.h"

void winNotice(sf::RenderWindow& window, sf::Font font, int size, int inTop) {
    string notice;
    if (inTop>=0) notice = "You win!You rank #" + to_string(inTop+1) + "\nContinue playing?\n(y/n)";
    else notice = "You win!\nContinue playing?\n(y/n)";

    sf::Text winText(notice, font, size*5);
    winText.setFillColor(sf::Color::White);
    winText.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = winText.getLocalBounds();
    winText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    winText.setPosition(100 + size * TILESIZE / 2, 200 + (size * TILESIZE / 2));

    sf::RectangleShape textBG(sf::Vector2f(size * 65, size * 65));
    textBG.setFillColor(sf::Color(255, 165, 0, 100)); // Semi-transparent orange
    textBG.setPosition(100, 200);

    window.draw(textBG);
    window.draw(winText);
}

void loseNotice(sf::RenderWindow& window, sf::Font font, int size, int inTop) {
    string notice;
    string tmp;
    if (!UNDO_ON) tmp = "(n)";
    else tmp = "(y/n)";
    if (inTop >= 0) notice = "You lose!You rank #" + to_string(inTop + 1) + "\nContinue playing?\n" + tmp;
    else notice = "You lose!\nContinue playing?\n" + tmp;

    sf::Text loseText(notice, font, size*5);
    loseText.setFillColor(sf::Color::White);
    loseText.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = loseText.getLocalBounds();
    loseText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    loseText.setPosition(100 + size * TILESIZE / 2, 200 + (size * TILESIZE / 2));

    sf::RectangleShape textBG(sf::Vector2f(size * 65, size * 65));
    textBG.setFillColor(sf::Color(0, 0, 0, 125)); // Semi-transparent black
    textBG.setPosition(100, 200);

    window.draw(textBG);
    window.draw(loseText);
}

void scoreDisplay(sf::RenderWindow& window, int score, int high) {
    sf::RectangleShape rec1(sf::Vector2f(100, 75)); // place for best score
    rec1.setFillColor(sf::Color(244, 164, 96, 255));
    rec1.setPosition(100, 30);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text best("Best", font, 20);
    best.setFillColor(sf::Color::White);
    best.setStyle(sf::Text::Bold);
    best.setPosition(130, 30);

    sf::Text bestVal(to_string(high), font, 20);
    bestVal.setFillColor(sf::Color::White);
    bestVal.setStyle(sf::Text::Bold);
    bestVal.setPosition(100, 60);

    sf::RectangleShape rec2(sf::Vector2f(100, 75)); // place for score
    rec2.setFillColor(sf::Color(210, 180, 140, 255));
    rec2.setPosition(100, 110);

    sf::Text point("Scores", font, 20);
    point.setFillColor(sf::Color::White);
    point.setStyle(sf::Text::Bold);
    point.setPosition(120, 110);

    sf::Text pointValue(to_string(score), font, 20);
    pointValue.setFillColor(sf::Color::White);
    pointValue.setStyle(sf::Text::Bold);
    pointValue.setPosition(100, 140);

    window.draw(rec1);
    window.draw(best);
    window.draw(bestVal);

    window.draw(rec2);
    window.draw(point);
    window.draw(pointValue);
}

void timeDisplay(sf::RenderWindow& window, sf::Font font, int time, int size) {
    sf::RectangleShape rec(sf::Vector2f(100, 75)); 
    rec.setFillColor(sf::Color(244, 164, 96, 255));
    rec.setPosition(size*TILESIZE+10, 30);

    sf::Text timer("Time", font, 20);
    timer.setFillColor(sf::Color::White);
    timer.setStyle(sf::Text::Bold);
    timer.setPosition(size * TILESIZE + 40, 30);

    sf::Text second(to_string(time), font, 20);
    second.setFillColor(sf::Color::White);
    second.setStyle(sf::Text::Bold);
    second.setPosition(size * TILESIZE + 10, 60);

    window.draw(rec);
    window.draw(timer);
    window.draw(second);
}

void drawGameBoard(sf::RenderWindow& window, sf::Texture img[], int** board, int size, int& win, int afterwin) {
    sf::Sprite block[15];
    for (int i = 0; i < 14; ++i) {
        block[i] = sf::Sprite(img[i]);
    }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == 0)
            {
                block[0].setPosition(j * TILESIZE + 100, i * TILESIZE + 200);
                window.draw(block[0]);
            }
            else if (board[i][j] == 2048)
            {
                block[11].setPosition(j * TILESIZE + 100, i * TILESIZE + 200);
                window.draw(block[11]);
                if(!afterwin) win = 1;
            }
            else {
                int index = int(log2(board[i][j]));
                block[index].setPosition(j * TILESIZE + 100, i * TILESIZE + 200);
                window.draw(block[index]);
            }
        }
}

void exitNotice(sf::RenderWindow& window, sf::Font font, int size) {
    int width = size * TILESIZE + 200, height = size * TILESIZE + 250;
    
    string notice;
    notice = "You are exiting!\nDo you want to save the game?\n(y/n/'B' to back to game)";

    sf::Text exitText(notice, font, size * 5);
    exitText.setFillColor(sf::Color::Black);
    exitText.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = exitText.getLocalBounds();
    exitText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    exitText.setPosition(100 + size * TILESIZE / 2, 100 + (size * TILESIZE / 2));

    sf::RectangleShape textBG(sf::Vector2f(width, height));
    textBG.setFillColor(sf::Color(0, 0, 0, 125)); // Semi-transparent black
    textBG.setPosition(0, 0);

    window.draw(textBG);
    window.draw(exitText);
}