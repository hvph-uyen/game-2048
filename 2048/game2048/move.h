#pragma once
#include "dataStruct.h"

void left(int**& grid, int size, int& run, int& score);
void right(int**& grid, int size, int& run, int& score);
void up(int**& grid, int size, int& run, int& score);
void down(int**& grid, int size, int& run, int& score);

void shift(int dir, game*& game2048);
void undo(game*& game2048);
void redo(game*& game2048);