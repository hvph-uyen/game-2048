#include "library.h"
#include "dataStruct.h"
#include "support.h"

string generateKey(string str, string key) {
	int c = 0;
	while (key.length() != str.length()) {
		if (str.length() == c) c = 0;
		key.push_back(key[c]);
		c++;
	}
	return key;
}

string encryptData(string str, string key) {
	string encrypt;
	for (int i = 0; i < str.length(); i++) {
		char c = (str[i] + key[i]) % 26;
		c += 'A';
		encrypt.push_back(c);
	}
	return encrypt;
}

string decryptData(string str, string key) {
	string decrypt;
	for (int i = 0; i < str.length(); i++) {
		char c = (str[i] - key[i] + 26) % 26;
		c += 'A';
		decrypt.push_back(c);
	}
	return decrypt;
}

string gameDataToString(game* game2048) {
	string ans = "";
	ans = ans + to_string(game2048->size) + " ";
	ans = ans + to_string(game2048->gameScore) + " ";
	ans = ans + to_string(game2048->time) + " ";
	ans = ans + to_string(game2048->move->spt) + " ";
	ans = ans + to_string(game2048->redo->spt) + " ";

	node* tmp = game2048->move->head;
	while (tmp->next != NULL) {
		ans = ans + to_string(tmp->moveScore) + " ";
		for (int i = 0; i < game2048->size; ++i)
			for (int j = 0; j < game2048->size; ++j)
				ans = ans + to_string(tmp->board[i][j]) + " ";
		tmp = tmp->next;
	}
	ans = ans + to_string(tmp->moveScore) + " ";
	for (int i = 0; i < game2048->size; ++i)
		for (int j = 0; j < game2048->size; ++j)
			ans = ans + to_string(tmp->board[i][j]) + " ";

	tmp = game2048->redo->head;
	if (tmp != NULL) {
		while (tmp->next != NULL) {
			ans = ans + to_string(tmp->moveScore) + " ";
			for (int i = 0; i < game2048->size; ++i)
				for (int j = 0; j < game2048->size; ++j)
					ans = ans + to_string(tmp->board[i][j]) + " ";
			tmp = tmp->next;
		}
		ans = ans + to_string(tmp->moveScore) + " ";
		for (int i = 0; i < game2048->size; ++i)
			for (int j = 0; j < game2048->size; ++j)
				ans = ans + to_string(tmp->board[i][j]) + " ";
	}
	return ans;
}

string encryptGameData(string data) {
	string keyword = "LEMON";
	string key = generateKey(data, keyword);
	for (int i = 0; i < data.length(); ++i)  //  'A' = space, 0 = 'B', 'L' = |
		if (data[i] >= 48 && data[i] <= 57) data[i] = char(data[i] + 18);
		else if (data[i] == '|') data[i] = char(76);
		else data[i] = 'A';
	string encrypt = encryptData(data, key);

	// luu lai key trong file
	ofstream keyFile;
	keyFile.open("key.bin", ios::binary | ios::out);
	keyFile.write(key.c_str(), key.size());
	keyFile.close();

	return encrypt;
}

string decryptGameData(string code, string key) {
	string decode = decryptData(code, key);
	string ans = "";
	for (int i = 0; i < decode.length(); ++i)
		if (decode[i] >= 66 && decode[i] != 76)
			ans += char(decode[i] - 18);
		else if (decode[i] == 76) ans += "|";
		else ans += " ";
	return ans;
}

void stringToGameData(game*& game2048, string data) {
	int pre = 0, cur = 0;
	cur = data.find_first_of(' ', pre); // size
	string tmp = data.substr(pre, cur - pre);

	int size = stoi(tmp);
	game2048 = new game(size);

	pre = cur + 1;
	cur = data.find_first_of(' ', pre); // gameScore
	tmp = data.substr(pre, cur - pre);
	game2048->gameScore = stoi(tmp);

	pre = cur + 1;
	cur = data.find_first_of(' ', pre); // time
	tmp = data.substr(pre, cur - pre);
	game2048->time = stoi(tmp);

	pre = cur + 1;
	cur = data.find_first_of(' ', pre); // move spt
	tmp = data.substr(pre, cur - pre);
	game2048->move->spt = stoi(tmp);

	pre = cur + 1;
	cur = data.find_first_of(' ', pre); // redo spt
	tmp = data.substr(pre, cur - pre);
	game2048->redo->spt = stoi(tmp);

	for (int i = 0; i < game2048->move->spt; ++i) {
		pre = cur + 1;
		cur = data.find_first_of(' ', pre);
		tmp = data.substr(pre, cur - pre);
		int scoreMove = stoi(tmp);

		int** matrix;
		matrix = initNewMatrix(size);
		for (int j = 0; j < size; ++j) {
			for (int k = 0; k < size; ++k) {
				pre = cur + 1;
				cur = data.find_first_of(' ', pre);
				string tmp2 = data.substr(pre, cur - pre);

				int val = stoi(tmp2);
				matrix[j][k] = val;
			}
		}
		node* add = game2048->move->createNewNode(matrix, scoreMove);
		game2048->move->addNode(add);
		game2048->move->spt--;
	}
	for (int i = 0; i < game2048->redo->spt; ++i) {
		pre = cur + 1;
		cur = data.find_first_of(' ', pre);
		tmp = data.substr(pre, cur - pre);
		int scoreMove = stoi(tmp);

		int** matrix;
		matrix = initNewMatrix(size);
		for (int j = 0; j < size; ++j) {
			for (int k = 0; k < size; ++k) {
				pre = cur + 1;
				cur = data.find_first_of(' ', pre);
				string tmp2 = data.substr(pre, cur - pre);

				int val = stoi(tmp2);
				matrix[j][k] = val;
			}
		}
		node* add = game2048->redo->createNewNode(matrix, scoreMove);
		game2048->redo->addNode(add);
	}
}