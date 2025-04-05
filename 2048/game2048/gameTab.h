#pragma once
#include "support.h"

void enterPIN(sf::Font font, sf::Texture img[], game* listgame[], int& listSPT, game*& game2048, int accountID, string name);
void savedAccountListTab(sf::Font font, sf::Texture img[], game* listgame[], int &listSPT, game*& game2048, int &del);
void top20Tab(game*& game2048, game* listgame[], int &listSPT, sf::Texture img[], sf::Font font, top rank[]);
void menu(game* listgame[], int &listSPT, game*& game2048, sf::Texture img[], sf::Font font);
void enterNameTab(sf::Font font, sf::Texture img[], game* listgame[], int &listSPT, game*& game2048);
void enterPassTab(sf::Font font, sf::Texture img[], game* listgame[], int &listSPT, game*& game2048, int accountID);
void settingTab(game*& game2048, game* listgame[], int& listSPT, sf::Texture img[], sf::Font font, int& size);