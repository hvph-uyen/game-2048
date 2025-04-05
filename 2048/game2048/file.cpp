#include "library.h"
#include "dataStruct.h"
#include "support.h"
#include "encrypt.h"

void saveGameToFile(game* listgame2048[], int spt, string fileName) {
	string data = "";
	for (int k = 0; k < spt; ++k) {
		if (k != spt - 1) data = data + gameDataToString(listgame2048[k]) + " ";
		else data = data + gameDataToString(listgame2048[k]);
		data += "|";
	}
	string encrypt = encryptGameData(data);
	ofstream file;
	file.open(fileName, ios::binary | ios::out);
	file.write((char*)&spt, sizeof(int));
	file.write(encrypt.c_str(), encrypt.size());
	file.close();
}

void retriveGameFromFile(game* listgame2048[], int& spt, string fileName) {
	ifstream kFile;
	kFile.open("key.bin", ios::binary | ios::in);

	kFile.seekg(0, ios::end);
	streampos fileSize = kFile.tellg();
	kFile.seekg(0, ios::beg);
	string key(fileSize, ' ');
	kFile.read(&key[0], fileSize);

	int c = 0;
	string keyString = "";
	while (c < key.length()) {
		if (key[c] >= 'A' && key[c] <= 'Z') keyString += key[c];
		c++;
	}
	kFile.close();

	ifstream file;
	file.open(fileName, ios::binary | ios::in);

	file.read((char*)&spt, sizeof(int));

	file.seekg(0, ios::end);
	fileSize = file.tellg();
	file.seekg(0, ios::beg);
	string data(fileSize, ' ');
	file.read(&data[0], fileSize);

	c = 0;
	string str = "";
	while (c < data.length()) {
		if (data[c] >= 'A' && data[c] <= 'Z') str += data[c];
		c++;
	}
	str = decryptGameData(str, keyString);
	int pre = 0, cur = 0;
	for (int i = 0; i < spt; ++i) {
		cur = str.find_first_of("|", pre);
		string info = str.substr(pre, cur - pre);
		pre = cur + 1;
		stringToGameData(listgame2048[i], info);
	}
	file.close();
}

void addToTop20File(top player, string fileName, int time, int listSPT) {
	ifstream inp;
	top top20[25];
	inp.open(fileName, ios::binary | ios::in);
	for (int i = 0; i < 20; ++i) {
		inp.read((char*)&top20[i].name, sizeof(string));
		inp.read((char*)&top20[i].scores, sizeof(int));
		inp.read((char*)&top20[i].time, sizeof(int));
	}
	inp.close();

	account accountList[10];
	getAccountData(accountList, "userAccount.bin");
	
	int pos = -1;
	for (int i = 0; i < 20; ++i) {
		if (top20[i].name == player.name) { // check duplicate name in top 20
			pos = i;
			break;
		}
	}
	for (int i = 0; i < listSPT; ++i) { // check duplicate name in account list
		if (player.name == accountList[i].username) {
			pos = i;
			break;
		}
	}

	if (pos != -1) {
		if (top20[pos].scores < player.scores) {
			top20[pos].scores = player.scores;
			top20[pos].time = player.time;
		}
	}
	else { // name not in list
		int m = top20[0].scores, vt = 0; // find the lowest in rank in time & score
		for (int i = 1; i < 20; ++i)
			if (top20[i].scores < m) {
				m = top20[i].scores;
				vt = i;
			}
		/*m = top20[vt].time;
		int c = 0;
		for (int i = 0; i < 20; ++i) {
			if (top20[i].scores == top20[vt].scores && vt != i)
				if (top20[i].time < m) {
					m = top20[i].time;
					c = i;
				}
		}*/
		top20[vt].name = player.name; // replace
		top20[vt].scores = player.scores;
		top20[vt].time = player.time;
	}

	sort(top20, top20 + 20, &topSortCondition); // sort rank

	ofstream file;
	file.open(fileName, ios::binary | ios::out);

	for (int i = 0; i < 20; ++i) {
		file.write((char*)&top20[i].name, sizeof(string));
		file.write((char*)&top20[i].scores, sizeof(int));
		file.write((char*)&top20[i].time, sizeof(int));
	}
	file.close();
}

