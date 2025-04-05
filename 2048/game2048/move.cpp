#include "support.h"
#include "library.h"
#include "dataStruct.h"

/* value vs movement:
1: UP 
2: DOWN
3: RIGHT
4: LEFT
*/

void left(int**& grid, int size, int& run, int& score) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (grid[i][j] != 0) {
                int vt = findFirstElementNotZero(grid, size, i, 1, j + 1, size);
                if (vt == -1) break;
                if (grid[i][vt] == grid[i][j]) {
                    grid[i][j] += grid[i][vt];
                    score += grid[i][j];
                    grid[i][vt] = 0;
                    run = true;
                }
            }
        }
        // dich het sang trai
        for (int j = 0; j < size; ++j) {
            if (grid[i][j] == 0) {
                int vt = findFirstElementNotZero(grid, size, i, 1, j, size);
                if (vt == -1) break;
                grid[i][j] = grid[i][vt];
                grid[i][vt] = 0;
                run = true;
            }
        }
    }
}

void right(int**& grid, int size, int& run, int& score) {
    for (int i = 0; i < size; ++i) {
        for (int j = size - 1; j >= 0; --j) {
            if (grid[i][j] != 0) {
                int vt = findFirstElementNotZero(grid, size, i, 2, 0, j - 1);
                if (vt == -1) break;
                if (grid[i][vt] == grid[i][j]) {
                    grid[i][j] += grid[i][vt];
                    score += grid[i][j];
                    grid[i][vt] = 0;
                    run = true;
                }
            }
        }
        // dich het sang phai
        for (int j = size - 1; j >= 0; --j) {
            if (grid[i][j] == 0) {
                int vt = findFirstElementNotZero(grid, size, i, 2, 0, j);
                if (vt == -1) break;
                grid[i][j] = grid[i][vt];
                grid[i][vt] = 0;
                run = true;
            }
        }
    }
}

void up(int**& grid, int size, int& run, int& score) {
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            if (grid[i][j] != 0) {
                int vt = findFirstElementNotZero(grid, size, j, 3, i + 1, size);
                if (vt == -1) break;
                if (grid[vt][j] == grid[i][j]) {
                    grid[i][j] += grid[vt][j];
                    score += grid[i][j];
                    grid[vt][j] = 0;
                    run = true;
                }
            }
        }
        // dich het len tren
        for (int i = 0; i < size; ++i) {
            if (grid[i][j] == 0) {
                int vt = findFirstElementNotZero(grid, size, j, 3, i, size);
                if (vt == -1) break;
                grid[i][j] = grid[vt][j];
                grid[vt][j] = 0;
                run = true;
            }
        }
    }
}

void down(int**& grid, int size, int& run, int& score) {
    for (int j = 0; j < size; ++j) {
        for (int i = size - 1; i > 0; --i) {
            if (grid[i][j] != 0) {
                int vt = findFirstElementNotZero(grid, size, j, 4, 0, i - 1);
                if (vt == -1) break;
                if (grid[vt][j] == grid[i][j]) {
                    grid[i][j] += grid[vt][j];
                    score += grid[i][j];
                    grid[vt][j] = 0;
                    run = true;
                }
            }
        }
        // dich het xuong duoi
        for (int i = size - 1; i > 0; --i) {
            if (grid[i][j] == 0) {
                int vt = findFirstElementNotZero(grid, size, j, 4, 0, i);
                if (vt == -1) break;
                grid[i][j] = grid[vt][j];
                grid[vt][j] = 0;
                run = true;
            }
        }
    }
}

void shift(int dir, game *&game2048) {
    int size = game2048->size, score = game2048->gameScore;
    int** board;
    board = copyToNewGrid(game2048->move->tail->board, size);

    int run = 0, preScore = 0;

    if (dir == 1) { // up
        up(board, size, run, score);
    }
    else if (dir == 2) { // down
        down(board, size, run, score);
    }
    else if (dir == 3) { // right
        right(board, size, run, score);
    }
    else if (dir == 4) { // left 
        left(board, size, run, score);
    }

    if (run) {
        randomBlock(board, size, (rand() % 2 + 1) * 2);
        node* newMove = game2048->move->createNewNode(board, score - game2048->gameScore);
        game2048->move->addNode(newMove);
        game2048->gameScore = score;

        if (game2048->redo != NULL) releaseRedoList(game2048);
    }
    else deleteBoard(board, size);
}

void undo(game*& game2048) {
    if (game2048->move->head->next != NULL) { // con undo duoc
        int** tmp = copyToNewGrid(game2048->move->tail->board, game2048->size);
        node* newUndoNode = game2048->redo->createNewNode(tmp, game2048->move->tail->moveScore);
        game2048->redo->addNode(newUndoNode);
        game2048->gameScore -= game2048->move->tail->moveScore;
        game2048->move->deleteTailNode();
    }
}

void redo(game*& game2048) {
    if (game2048->redo->tail != NULL) {
        int** tmp = copyToNewGrid(game2048->redo->tail->board, game2048->size);
        node* newRedoNode = game2048->redo->createNewNode(tmp, game2048->redo->tail->moveScore);
        game2048->move->addNode(newRedoNode);
        game2048->gameScore += game2048->redo->tail->moveScore;
        game2048->redo->deleteTailNode();
    }
}