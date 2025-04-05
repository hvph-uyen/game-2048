#pragma once
#include "library.h"

struct node {
	int** board;
	int moveScore;
	node* next;
};

struct sllist {
	node* head;
	node* tail;
	int spt;

	sllist() {
		this->head = NULL;
		this->tail = NULL;
		this->spt = 0;
	}

	node* createNewNode(int** val, int inc) {
		node* newNode = new node();
		newNode->board = val;
		newNode->moveScore = inc;
		newNode->next = NULL;
		return newNode;
	}

	void addNode(node* add) {
		if (this->head == NULL) {
			this->head = add;
			this->tail = add;
		}
		else {
			this->tail->next = add;
			this->tail = add;
			add->next = NULL;
		}
		this->spt++;
	}

	void deleteTailNode() { 
		if (this->head->next != NULL) { // > 1 node
			node* tmp = this->head;
			while (tmp->next != this->tail) tmp = tmp->next;
			this->tail = tmp;
			delete this->tail->next;
			tmp->next = NULL;
		}
		else { // 1 node
			node* tmp = this->head;
			this->tail = NULL;
			this->head = NULL;
			delete tmp;
		}
		this->spt--;
	}
};

struct game {
	int size;
	int gameScore;
	int time;
	sllist* move;
	sllist* redo;

	game(int size) {
		this->gameScore = 0;
		this->size = size;
		this->time = 0;
		this->move = new sllist();
		this->redo = new sllist();
	}
};

struct top {
	string name;
	int scores;
	int time;
};

struct account {
	string username;
	string pass;
};