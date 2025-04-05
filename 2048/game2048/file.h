#pragma once
#include "dataStruct.h"

void saveGameToFile(game* listgame2048[], int spt, string fileName);
void retriveGameFromFile(game* listgame2048[], int& spt, string fileName);
void addToTop20File(top player, string fileName, int time, int listSPT);