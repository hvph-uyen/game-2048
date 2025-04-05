#include "library.h"
#include "dataStruct.h"

void loadResourceFiles(sf::Texture img[], sf::Font& font) {
	font.loadFromFile("arial.ttf");

	int imgVal = 1;
	for (int i = 0; i < 14; ++i) {
		string link = "images/";
		if (i == 0) {
			link = link + "blank" + ".png";
		}
		else {
			imgVal *= 2;
			link = link + to_string(imgVal) + ".png";
		}
		img[i].loadFromFile(link);
	}
	img[14].loadFromFile("images/logo.png");
}

int** initNewMatrix(int size) {
	int** matrix = new int* [size];
	for (int i = 0; i < size; ++i) {
		matrix[i] = new int[size];
		for (int j = 0; j < size; ++j) matrix[i][j] = 0;
	}
	return matrix;
}

void randomBlock(int**& matrix, int size, int val) {
	while (true) {
		int x = rand() % size;
		int y = rand() % size;
		if (matrix[x][y] == 0) {
			matrix[x][y] = val;
			break;
		}
	}
}

int findFirstElementNotZero(int** grid, int size, int line, int dir, int st, int en) {
    if (dir == 1) {// tim vi tri khac 0 dau tien tu trai -> phai
        for (int i = st; i < en; ++i)
            if (grid[line][i] != 0) return i;
    }
    else
        if (dir == 2) { // tim vi tri khac 0 dau tien tu phai -> trai
            for (int i = en; i >= st; --i)
                if (grid[line][i] != 0) return i;
        }
        else
            if (dir == 3) { // tim vi tri khac 0 dau tien tu tren -> duoi
                for (int i = st; i < en; ++i)
                    if (grid[i][line] != 0) return i;
            }
            else { // tim vi tri khac 0 dau tien tu duoi -> tren
                for (int i = en; i >= st; --i)
                    if (grid[i][line] != 0) return i;
            }
    return -1;
}

int** copyToNewGrid(int** grid, int size) {
	int** newGrid = new int* [size];
	for (int i = 0; i < size; ++i) {
		newGrid[i] = new int[size];
		for (int j = 0; j < size; ++j) newGrid[i][j] = grid[i][j];
	}
	return newGrid;
}

int loseGame(int** grid, int size) {
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			if (grid[i][j] == 0) return 0;
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j) {
			if (i == 0 && j != 0 && j != size - 1) {
				if (grid[i][j] == grid[i][j + 1] || grid[i][j] == grid[i][j - 1]) return 0;
			}
			else if (i == size - 1 && j != 0 && j != size - 1) {
					if (grid[i][j] == grid[i][j + 1] || grid[i][j] == grid[i][j - 1]) return 0;
			}
			else if (j == 0 && i != 0 && i != size - 1) {
					if (grid[i][j] == grid[i + 1][j] || grid[i][j] == grid[i - 1][j]) return 0;
			}
			else if (j == size - 1 && i != 0 && i != size - 1) {
					if (grid[i][j] == grid[i + 1][j] || grid[i][j] == grid[i - 1][j]) return 0;
			}
			else if (i != 0 && i != size - 1 && j != 0 && j != size - 1) {
					if (grid[i][j] == grid[i][j + 1] || grid[i][j] == grid[i][j - 1] || grid[i - 1][j] == grid[i][j] || grid[i][j] == grid[i + 1][j])  return 0;
			}
		}
	return 1;
}

void deleteBoard(int** &board, int size) { // delete a board
	for (int i = 0; i < size; ++i)
		delete[]board[i];
	delete[]board;
}

void deleteList(sllist *&list, int boardsize) {
	while (list->head != NULL && list->head->next != NULL) {
		node* tmp = list->head;
		list->head = list->head->next;
		tmp->next = NULL;
		delete tmp;
	}
	node* tmp2 = list->head;
	list->head = NULL;
	delete tmp2;
}

void deleteListBoard(game*& game2048) { // delete boards in lists
	if (!(game2048->move->head == NULL)) {
		node* tmp = game2048->move->head;
		while (tmp->next != NULL) {
			deleteBoard(tmp->board, game2048->size);
			tmp = tmp->next;
		}
		deleteBoard(tmp->board, game2048->size);
	}
	if (!(game2048->redo->head == NULL)) {
		node* tmp = game2048->redo->head;
		while (tmp->next != NULL) {
			deleteBoard(tmp->board, game2048->size);
			tmp = tmp->next;
		}
		deleteBoard(tmp->board, game2048->size);
	}
}

void releaseRedoList(game *&game2048) {
	if (game2048->redo != NULL) {
		deleteList(game2048->redo, game2048->size);
		game2048->redo->head = NULL;
		game2048->redo->tail = NULL;
	}
}

void deleteGameMemory(game *&game2048) {
	deleteListBoard(game2048);
	deleteList(game2048->move, game2048->size);
	deleteList(game2048->redo, game2048->size);
}

bool topSortCondition(top a, top b) {
	if (a.scores > b.scores) return true;
	if (a.scores < b.scores) return false;
	if (a.time <= b.time) return true;
	if (a.time > b.time) return false;
	return false;
}

void getTop20Data(top top20[], string fileName) {
	ifstream inp;
	inp.open(fileName, ios::binary | ios::in);
	for (int i = 0; i < 20; ++i) {
		inp.read((char*)&top20[i].name, sizeof(string));
		inp.read((char*)&top20[i].scores, sizeof(int));
		inp.read((char*)&top20[i].time, sizeof(int));
	}
	inp.close();
}

void getAccountData(account list[], string fileName) {
	ifstream inp;
	inp.open(fileName, ios::binary | ios::in);

	int numOfAccount = 0;
	inp.read((char*)&numOfAccount, sizeof(int));
	for (int i = 0; i < numOfAccount; ++i) {
		inp.read((char*)&list[i].username, sizeof(string));
		inp.read((char*)&list[i].pass, sizeof(string));
	}
	inp.close();
}

void writeToAccountFile(account list[], int numOfAccount, string fileName) {
	ofstream out;
	out.open(fileName, ios::binary | ios::out);

	out.write((char*)&numOfAccount, sizeof(int));
	for (int i = 0; i < numOfAccount; ++i) {
		out.write((char*)&list[i].username, sizeof(string));
		out.write((char*)&list[i].pass, sizeof(string));
	}
	out.close();
}

int rankInTop(top top20[], int score, int time) {
	for (int i = 0; i < 20; ++i) {
		if (score > top20[i].scores) return i;
		else if (score == top20[i].scores) {
			if (time <= top20[i].time) return i;
			else return i + 1;
		}
	}
	return -1;
}

int isThereData(string fileName) {
	ifstream file;
	file.open(fileName, ios::in | ios::binary);
	if (file.peek() == EOF) return 0;
	return 1;
}

bool validName(char c) {
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57)) return true;
	return false;
}

string correctName(string s) {
		// xu li ki tu dac biet (! ki tu va ! so)
		// (s[i] >= 65 && s[i] <= 90) || (s[i] >= 97 && s[i] <=122) || (s[i] >= 48 && s[i] <= 57))
	// >= 33 <= 90 >= 97 <=122
	for (int i = 0; i<s.length(); ++i) 
		if (!validName(s[i])) {
			s.erase(i, 1);
		}
	return s;
}

string correctNum(string s) {
	for (int i = 0; i < s.length(); ++i)
		if (!(s[i] >= 48 && s[i] <= 57)) {
			s.erase(i, 1);
		}
	return s;
}

bool correctSize(string s) {
	if (s.length() == 1) { // 1 chu so
		if (s[s.length() - 1] >= '4' && s[s.length() - 1] <= '9') return true;
		else return false;
	}
	else if (s.length() == 2) { // 2 chu so
		if (s[s.length() - 1] == '0' && s[s.length() - 2] == '1') return true;
		else return false;
	}
}

void releaseSavedGameData() { // ham chi dung de test
	ofstream file;
	file.open("savedGame.bin", ios::binary | ios::out);
	file.close();

	ofstream file1;
	file1.open("userAccount.bin", ios::binary | ios::out);
	file1.close();

	ofstream file2;
	top top20[25];
	file2.open("top20.bin", ios::binary | ios::out);
	for (int i = 0; i < 20; ++i) {
		top20[i].name = " ";
		top20[i].scores = 0;
		top20[i].time = 0;
		file2.write((char*)&top20[i].name, sizeof(string));
		file2.write((char*)&top20[i].scores, sizeof(int));
		file2.write((char*)&top20[i].time, sizeof(int));
	}
	file2.close();
}