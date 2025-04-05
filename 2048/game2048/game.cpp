#include "library.h"
#include "move.h"
#include "dataStruct.h"
#include "support.h"
#include "display.h"
#include "file.h"
#include "gameTab.h"

void initGame(game *&game2048, int size){
    int** board = initNewMatrix(size);
    randomBlock(board, size, (rand() % 2 + 1) * 2);
    randomBlock(board, size, (rand() % 2 + 1) * 2);

    game2048 = new game(size);
    node* newMove = game2048->move->createNewNode(board, 0);
    game2048->move->addNode(newMove);
}

void GamePlay(game*&game2048, sf::Texture img[], sf::Font font, int& save) {
    // chu thich phim
    sf::Text sub1("Exit ('E') Restart ('K')", font, 20);
    sub1.setFillColor(sf::Color(210, 180, 140, 255));
    sub1.setStyle(sf::Text::Bold);

    sf::Text sub2("Undo ('U')\nRedo ('R')", font, 20);
    sub2.setFillColor(sf::Color(210, 180, 140, 255));
    sub2.setStyle(sf::Text::Bold);

    // bien cho game
    int size = game2048->size;
    int width = size * TILESIZE + 200, height = size * TILESIZE + 250;
    int score = 0, win = 0, lose = 0, cont = 0, exit = 0, afterwin = 0, winExit = 0, loseExit = 0;
    top top20[25];
    getTop20Data(top20, "top20.bin");

    // bien cho phan timer
    auto start = std::chrono::steady_clock::now();
    int time = 0, wait = 0, init = game2048->time;
    auto now = std::chrono::steady_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::seconds>(now - start);
    auto tmptime = std::chrono::steady_clock::now();
    auto stoptime = std::chrono::duration_cast<std::chrono::seconds>(now - now);

    sf::RenderWindow window(sf::VideoMode(width, height), "2048");
    while (window.isOpen())
    {
        if (((!lose && !win) || (cont && lose)) && !exit) { // timer
            wait += stoptime.count();
            stoptime = std::chrono::duration_cast<std::chrono::seconds>(now - now);
            now = std::chrono::steady_clock::now();
            runtime = std::chrono::duration_cast<std::chrono::seconds>(now - start);
            time = runtime.count() - wait + init;
        }
        else {
            tmptime = std::chrono::steady_clock::now();
            stoptime = std::chrono::duration_cast<std::chrono::seconds>(tmptime - now);
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (!win && !lose && !exit) {
                    if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
                        shift(4, game2048);
                    else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                        shift(3, game2048);
                    else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
                        shift(1, game2048);
                    else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                        shift(2, game2048);
                    else if (event.key.code == sf::Keyboard::U && UNDO_ON) {  // undo
                        undo(game2048);
                        cont = 0;
                    }
                    else if (event.key.code == sf::Keyboard::R && UNDO_ON) // redo
                        redo(game2048);
                    else if (event.key.code == sf::Keyboard::E) { // exit
                        exit = 1;
                    }
                    else if (event.key.code == sf::Keyboard::K) { // restart
                        game2048->gameScore = 0;
                        game2048->time = 0;
                        deleteGameMemory(game2048);
                        initGame(game2048, size);

                        // bien cho game
                        size = game2048->size;
                        width = size * TILESIZE + 200; height = size * TILESIZE + 250;
                        score = 0; win = 0; lose = 0; cont = 0; exit = 0; afterwin = 0; winExit = 0; loseExit = 0;
                
                        // bien cho phan timer
                        start = std::chrono::steady_clock::now();
                        time = 0; wait = 0; init = game2048->time;
                        now = std::chrono::steady_clock::now();
                        runtime = std::chrono::duration_cast<std::chrono::seconds>(now - start);
                        tmptime = std::chrono::steady_clock::now();
                        stoptime = std::chrono::duration_cast<std::chrono::seconds>(now - now);
                    }

                }
                else if (event.key.code == sf::Keyboard::B && exit) { // back to game from exit
                    exit = 0;
                }
                else if (event.key.code == sf::Keyboard::B && winExit) { // back to game from exit at win
                    winExit = 0;
                }
                else if (event.key.code == sf::Keyboard::B && loseExit) { // back to game from exit at win
                    loseExit = 0;
                }
                else if (lose && !loseExit) {
                    if (event.key.code == sf::Keyboard::Y && UNDO_ON) {
                        cont = 1;
                    }
                    else if (event.key.code == sf::Keyboard::N) {
                        loseExit = 1;
                    }
                }
                else if (win && !winExit) {
                    if (event.key.code == sf::Keyboard::N) { // exit
                        winExit = 1;
                    }
                    else if (event.key.code == sf::Keyboard::Y) { // continue play
                        win = 0;
                        afterwin = 1;
                    }
                }
                else if (exit || winExit || loseExit) { // save or not when exit
                    if (event.key.code == sf::Keyboard::Y) {
                        save = 1;
                        window.close();
                    }
                    else if (event.key.code == sf::Keyboard::N) {
                        save = 0;
                        window.close();
                    }
                }
            }
        }
        window.clear(sf::Color::White);

        sub1.setPosition(size * TILESIZE - 30, 115);
        window.draw(sub1);
        if (UNDO_ON) {
            sub2.setPosition(size * TILESIZE + 10, 140);
            window.draw(sub2);
        }

        if (((!lose && !win) || (cont && lose)) && !exit) game2048->time = time;
        timeDisplay(window, font, game2048->time, size);

        int** board = game2048->move->tail->board;
        //board[0][0] = 1024;

        drawGameBoard(window, img, board, size, win, afterwin);

        if (win && !winExit) {
            int inTop = rankInTop(top20, game2048->gameScore, game2048->time);
            winNotice(window, font, size, inTop); // display "You win"
        }

        if (loseGame(board, size) && !cont) lose = 1; // check Game over?
        else lose = 0;

        if (lose && !cont && !loseExit) {
            int inTop = rankInTop(top20, game2048->gameScore, game2048->time);
            loseNotice(window, font, size, inTop); // display "You lose"
        }

        int highest = 0;
        if (top20[0].scores > game2048->gameScore) highest = top20[0].scores;
        else highest = game2048->gameScore;
        scoreDisplay(window, game2048->gameScore, highest);  // display score
        if (exit || winExit || loseExit) exitNotice(window, font, size);

        window.display();
    }
}