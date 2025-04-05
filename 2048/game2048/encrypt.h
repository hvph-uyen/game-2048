#pragma once
#include "library.h"
#include "dataStruct.h"

string generateKey(string str, string key);
string encryptData(string str, string key);
string decryptData(string str, string key);
string gameDataToString(game* game2048);
string encryptGameData(string data);
string decryptGameData(string code, string key);
void stringToGameData(game*& game2048, string data);