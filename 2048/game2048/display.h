#pragma once

void winNotice(sf::RenderWindow& window, sf::Font font, int size, int inTop);
void loseNotice(sf::RenderWindow& window, sf::Font font, int size, int inTop);
void exitNotice(sf::RenderWindow& window, sf::Font font, int size);
void scoreDisplay(sf::RenderWindow& window, int score, int high);
void timeDisplay(sf::RenderWindow& window, sf::Font font, int time, int size);
void drawGameBoard(sf::RenderWindow& window, sf::Texture img[], int** board, int size, int& win, int afterwin);
