#pragma once
#include "dataStruct.h"
#include "library.h"

void loadResourceFiles(sf::Texture img[], sf::Font& font);

int** initNewMatrix(int size);
void randomBlock(int**& matrix, int size, int val);
int findFirstElementNotZero(int** grid, int size, int line, int dir, int st, int en);
int** copyToNewGrid(int** grid, int size);
int loseGame(int** grid, int size);

void deleteBoard(int** &board, int size);
void deleteList(sllist*& list, int boardsize);
void releaseRedoList(game*& game2048);
void deleteGameMemory(game*& game2048);

bool topSortCondition(top a, top b);
void getTop20Data(top top20[], string fileName);
void getAccountData(account list[], string fileName);
void writeToAccountFile(account list[], int numOfAccount, string fileName);
int rankInTop(top top20[], int score, int time);
int isThereData(string fileName);

string correctName(string s);
string correctNum(string s);
bool correctSize(string s);

void releaseSavedGameData();