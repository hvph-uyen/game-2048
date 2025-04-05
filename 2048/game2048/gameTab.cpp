#include "support.h"
#include "game.h"
#include "file.h"
#include "display.h"

void menu(game* listgame[], int &listSPT, game*& game2048, sf::Texture img[], sf::Font font);
void savedAccountListTab(sf::Font font, sf::Texture img[], game* listgame[], int& listSPT, game*& game2048, int& del);

void enterPIN(sf::Font font, sf::Texture img[], game* listgame[], int &listSPT, game*& game2048, int accountID, string name) {
    sf::RectangleShape coverInput(sf::Vector2f(300, 50)); 
    coverInput.setFillColor(sf::Color(244, 164, 96, 150));

    sf::Text enter("Enter password to protect your game: ", font, 20);
    enter.setFillColor(sf::Color::Black);

    sf::String inp = "";
    sf::Text input("", font, 30);
    input.setFillColor(sf::Color::Black);

    int repeat = 0, enterPress = 0;
    account list[10];
    getAccountData(list, "userAccount.bin");

    sf::RenderWindow enterPass(sf::VideoMode(400, 200), "Enter password");
    while (enterPass.isOpen())
    {
        sf::Event event;
        while (enterPass.pollEvent(event))
        {
            if (event.key.code == sf::Keyboard::BackSpace) {
                if (inp.getSize() > 1) {
                    inp = inp.substring(0, inp.getSize() - 1);
                }
                else inp = "";
                enterPress = 0;
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                enterPress = 1;
                if (accountID == -1) {
                    list[listSPT - 1].username = name;
                    list[listSPT - 1].pass = inp;
                }
                else {// from the 6th
                    list[accountID - 1].pass = inp;
                    list[accountID - 1].username = name;
                }
                writeToAccountFile(list, listSPT, "userAccount.bin");
                enterPass.close();
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.key.code != sf::Keyboard::Enter && event.text.unicode != '\r' && event.text.unicode != '\n') {
                    inp += event.text.unicode;
                }
                input.setString(inp);
                enterPress = 0;
            }
        }
        enterPass.clear(sf::Color::White);

        enter.setPosition(50, 20);
        enterPass.draw(enter);

        coverInput.setPosition(50, 50);
        enterPass.draw(coverInput);

        input.setPosition(50, 50);
        enterPass.draw(input);

        enterPass.display();
    }
}

void enterNameTab(sf::Font font, sf::Texture img[], game* listgame[], int &listSPT, game*& game2048) {
    sf::RectangleShape coverInput(sf::Vector2f(300, 50));
    coverInput.setFillColor(sf::Color(244, 164, 96, 150));

    sf::Text enter("Enter your name: ", font, 20);
    enter.setFillColor(sf::Color::Black);

    sf::String inp = "";
    sf::Text input("", font, 30);
    input.setFillColor(sf::Color::Black);

    sf::Text sub("Name already existed \nPlease enter another name", font, 15);
    sub.setFillColor(sf::Color::Red);

    sf::Text sub2("(Press 'Enter' to continue)", font, 15);
    sub2.setFillColor(sf::Color::Black);

    bool exist = 0;
    top top20[25];
    getTop20Data(top20, "top20.bin");
 
    account accountList[10];
    getAccountData(accountList, "userAccount.bin");

    sf::RenderWindow enterName(sf::VideoMode(400, 200), "Enter name");
    while (enterName.isOpen())
    {
        sf::Event event;
        while (enterName.pollEvent(event))
        {
            if (event.key.code == sf::Keyboard::BackSpace) {
                if (inp.getSize() > 1) {
                    inp = inp.substring(0, inp.getSize() - 1);
                }
                else inp = "";
                exist = 0;
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.key.code != sf::Keyboard::Enter && event.key.code != sf::Keyboard::BackSpace) {
                    inp += event.text.unicode;
                }
                inp = correctName(inp);
                input.setString(inp);
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                for (int i = 0; i < 20; ++i)
                    if (top20[i].name == inp) {
                        exist = 1;
                    }
                for (int i = 0; i < listSPT; ++i) {
                    if (inp == accountList[i].username) exist = 1;
                }
                if (!exist && inp != "") {
                    enterName.close();
                    
                    initGame(game2048, SIZE); 
                    int save = 0;
                    GamePlay(game2048, img, font, save); 
                    if (game2048->gameScore > top20[19].scores) {
                        top play;
                        play.name = inp;
                        play.scores = game2048->gameScore;
                        play.time = game2048->time;
                        addToTop20File(play, "top20.bin", game2048->time, listSPT);
                    }
                    if (save) {
                        if (listSPT < 5) {
                            listgame[listSPT] = game2048;
                            listSPT++;

                            enterPIN(font, img, listgame, listSPT, game2048, -1, inp);
                            enterName.close();
                        }
                        else { // = 5 TK -> replace 1 in 5
                            int del = 1;
                            savedAccountListTab(font, img, listgame, listSPT, game2048, del);
                            enterPIN(font, img, listgame, listSPT, game2048, del, inp);
                            game* tmp = listgame[del - 1];
                            deleteGameMemory(tmp); // xoa data game truoc do o vi tri del-1
                            listgame[del - 1] = game2048;
                        }
                    }
                    else deleteGameMemory(game2048);
                    menu(listgame, listSPT, game2048, img, font);
                }
            }
            else if (event.type == sf::Event::Closed) {
                enterName.close();
                menu(listgame, listSPT, game2048, img, font);
            }
        }
        enterName.clear(sf::Color::White);

        enter.setPosition(50, 20);
        enterName.draw(enter);

        coverInput.setPosition(50, 50);
        enterName.draw(coverInput);

        input.setPosition(50, 50);
        enterName.draw(input);

        if (exist) {
            sub.setPosition(50, 110);
            enterName.draw(sub);
        }

        sub2.setPosition(100, 160);
        enterName.draw(sub2);

        enterName.display();
    }
}

void enterPassTab(sf::Font font, sf::Texture img[], game* listgame[], int &listSPT, game*& game2048, int accountID) {
    sf::RectangleShape coverInput(sf::Vector2f(300, 50)); 
    coverInput.setFillColor(sf::Color(244, 164, 96, 150));

    sf::Text enter("Enter password: ", font, 20);
    enter.setFillColor(sf::Color::Black);

    sf::String inp = "";
    sf::Text input("", font, 30);
    input.setFillColor(sf::Color::Black);

    sf::Text sub("Password incorrect\nPlease enter again", font, 15);
    sub.setFillColor(sf::Color::Red);

    sf::Text sub2("(Press 'Enter' to continue)", font, 15);
    sub2.setFillColor(sf::Color::Black);

    int correct = 0, enterPress = 0;
    account list[10];
    getAccountData(list, "userAccount.bin");

    sf::RenderWindow enterPass(sf::VideoMode(400, 200), "Enter password");
    while (enterPass.isOpen())
    {
        sf::Event event;
        while (enterPass.pollEvent(event))
        {
            if (event.key.code == sf::Keyboard::BackSpace) {
                if (inp.getSize() > 1) {
                    inp = inp.substring(0, inp.getSize() - 1);
                }
                else inp = "";
                enterPress = 0;
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.key.code != sf::Keyboard::Enter && event.key.code != sf::Keyboard::BackSpace) {
                    inp += event.text.unicode;
                }
                input.setString(inp);
                enterPress = 0;
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                enterPress = 1;
                if (inp == list[accountID - 1].pass) {
                    enterPass.close();
                    correct = 1;
                    game2048 = listgame[accountID - 1];

                    int save = 0;
                    GamePlay(game2048, img, font, save);

                    top top20[25];
                    getTop20Data(top20, "top20.bin");
                    if (game2048->gameScore > top20[19].scores) {
                        top play;
                        play.name = list[accountID-1].username;
                        play.scores = game2048->gameScore;
                        play.time = game2048->time;
                        addToTop20File(play, "top20.bin", game2048->time, listSPT);
                    }
                    
                    if (save) {
                        listgame[accountID - 1] = game2048; /////////////////////////
                    }

                    menu(listgame, listSPT, game2048, img, font);
                } 
            }
            else if (event.type == sf::Event::Closed) {
                enterPass.close();
                int del = 0;
                savedAccountListTab(font, img, listgame, listSPT, game2048, del);
            }
        }
        enterPass.clear(sf::Color::White);

        enter.setPosition(50, 20);
        enterPass.draw(enter);

        coverInput.setPosition(50, 50);
        enterPass.draw(coverInput);

        input.setPosition(50, 50);
        enterPass.draw(input);

        if (!correct && enterPress) {
            sub.setPosition(50, 110);
            enterPass.draw(sub);
        }

        sub2.setPosition(100, 160);
        enterPass.draw(sub2);

        enterPass.display();
    }
}

void savedAccountListTab(sf::Font font, sf::Texture img[], game* listgame[], int &listSPT, game*& game2048, int &del) {
    string notice;
    if (!del) notice = "Please choose 1 of the \nabove accounts to resume\n(Press 1-5)";
    else notice = "Please choose 1 of the \nabove accounts to delete\n(Press 1-5)";

    sf::Text sub(notice, font, 25);
    sub.setFillColor(sf::Color::Black);

    sf::RectangleShape cover(sf::Vector2f(300, 50));
    cover.setFillColor(sf::Color(244, 164, 96, 150));

    account list[10];
    getAccountData(list, "userAccount.bin");

    int option = 0;
    sf::RenderWindow accountListTab(sf::VideoMode(400, 500), "Account list");
    while (accountListTab.isOpen())
    {
        sf::Event event;
        while (accountListTab.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1) option = 1;
                    if (event.key.code == sf::Keyboard::Num2) option = 2;
                    if (event.key.code == sf::Keyboard::Num3) option = 3;
                    if (event.key.code == sf::Keyboard::Num4) option = 4;
                    if (event.key.code == sf::Keyboard::Num5) option = 5;
                }
            }
            else if (option != 0 && !del && listSPT>0 && option <=listSPT) {
                accountListTab.close();
                enterPassTab(font, img, listgame, listSPT, game2048, option);
            }
            else if (option > 0 && del && option <=listSPT) { // replace 1 trong 5 TK saved for resume
                accountListTab.close();
                del = option;
            }

            else if (event.type == sf::Event::Closed && del == 0) {
                accountListTab.close();
                menu(listgame, listSPT, game2048, img, font);
            }
        }
        accountListTab.clear(sf::Color::White);

        sub.setPosition(50, 30);
        accountListTab.draw(sub);

        for (int i = 0; i < listSPT; ++i) {
            if (i % 2 == 0) {
                cover.setPosition(50, 140 + i * 50);
                accountListTab.draw(cover);
            }

            sf::Text name(to_string(i + 1) + ". " + list[i].username, font, 40);
            name.setFillColor(sf::Color::Black);
            name.setPosition(50, 135 + i * 50);
            accountListTab.draw(name);
        }

        for (int i = listSPT; i < 5; ++i) {
            if (i % 2 == 0) {
                cover.setPosition(50, 140 + i * 50);
                accountListTab.draw(cover);
            }

            sf::Text name(to_string(i + 1) + ". " + list[i].username, font, 40);
            name.setFillColor(sf::Color(0, 0, 0, 125));
            name.setPosition(50, 135 + i * 50);
            accountListTab.draw(name);
        }
        accountListTab.display();
    }
}

void top20Tab(game*& game2048, game* listgame[], int &listSPT, sf::Texture img[], sf::Font font, top rank[]) {
    sf::Text top20("TOP 20", font, 50);
    top20.setFillColor(sf::Color::Black);
    top20.setStyle(sf::Text::Bold);

    sf::RectangleShape cover(sf::Vector2f(430, 35));
    cover.setFillColor(sf::Color(244, 164, 96, 150));

    sf::RenderWindow top20Tab(sf::VideoMode(500, 750), "TOP 20");
    while (top20Tab.isOpen())
    {
        sf::Event event;
        while (top20Tab.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                top20Tab.close();
                menu(listgame, listSPT, game2048, img, font);
            }
        }
        top20Tab.clear(sf::Color::White);

        top20.setPosition(160, 20);
        top20Tab.draw(top20);

        for (int i = 0; i < 20; ++i) {
                if (i % 2 == 0) {
                    cover.setPosition(30, 100 + i * 30);
                    top20Tab.draw(cover);
                }
                if (rank[i].scores > 0) {
                    sf::Text name(to_string(i + 1) + ". " + rank[i].name, font, 25);
                    name.setFillColor(sf::Color::Black);
                    name.setPosition(30, 100 + i * 30);
                    top20Tab.draw(name);

                    sf::Text score(to_string(rank[i].scores), font, 25);
                    score.setFillColor(sf::Color::Black);
                    score.setPosition(200, 100 + i * 30);
                    top20Tab.draw(score);

                    sf::Text time(to_string(rank[i].time) + " s", font, 25);
                    time.setFillColor(sf::Color::Black);
                    time.setPosition(350, 100 + i * 30);
                    top20Tab.draw(time);
                }
        }
        top20Tab.display();
    }
}

void settingTab(game*& game2048, game* listgame[], int& listSPT, sf::Texture img[], sf::Font font, int& size) {
    sf::RectangleShape coverInput(sf::Vector2f(300, 50)); 
    coverInput.setFillColor(sf::Color(244, 164, 96, 150));

    sf::Text enter("1/ GAME SIZE NxN (4-10): ", font, 20);
    enter.setFillColor(sf::Color::Black);
    enter.setStyle(sf::Text::Bold);

    sf::Text sub("Please enter a number between 4 and 10", font, 15);
    sub.setFillColor(sf::Color::Red);

    sf::Text undo("2/ UNDO MODE", font, 20);
    undo.setFillColor(sf::Color::Black);
    undo.setStyle(sf::Text::Bold);

    sf::Text onButton("ON", font, 20);
    onButton.setFillColor(sf::Color::Black);

    sf::Text offButton("OFF", font, 20);
    offButton.setFillColor(sf::Color::Black);

    sf::RectangleShape coverButton(sf::Vector2f(50, 30));
    coverButton.setFillColor(sf::Color(255, 128, 0, 150));

    sf::Text sub2("(Press 'S' to save settings)", font, 15);
    sub2.setFillColor(sf::Color::Black);

    sf::Text sub3("Cannot save settings", font, 20);
    sub3.setFillColor(sf::Color::Red);

    bool wrong = 0, pressSave = 0;

    sf::String inp = to_string(SIZE);
    sf::Text input(inp, font, 30);
    input.setFillColor(sf::Color::Black);

    int button;
    if (!UNDO_ON) button = 2;
    else button = 1;

    sf::RenderWindow settingTab(sf::VideoMode(400, 300), "Settings");
    while (settingTab.isOpen())
    {
        sf::Event event;
        while (settingTab.pollEvent(event))
        {
            if (event.key.code == sf::Keyboard::Right) {
                if (button < 2) button++;
            }
            else if (event.key.code == sf::Keyboard::Left) {
                if (button > 1) button--;
            }
            else if (event.key.code == sf::Keyboard::BackSpace) {
                if (inp.getSize() > 1) {
                    inp = inp.substring(0, inp.getSize() - 1);
                }
                else inp = "";
                wrong = 0;
                pressSave = 0;
            }
            else if (event.key.code == sf::Keyboard::S) {
                if (!wrong) { // save settings
                    // choose size
                    if (inp.getSize() == 1) size = inp[inp.getSize() - 1] - '0';
                    else size = 10;
                    // undo on-off
                    if (button == 1) UNDO_ON = 1;

                    settingTab.close();
                    menu(listgame, listSPT, game2048, img, font); 
                }
                else { /// cannot save setting
                    pressSave = 1;
                }
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.key.code != sf::Keyboard::Enter && event.text.unicode != '\r' && event.text.unicode != '\n') {
                    inp += event.text.unicode;
                }
                inp = correctNum(inp);
                wrong = !correctSize(inp);
                input.setString(inp);
            }
            else if (event.type == sf::Event::Closed) {
                settingTab.close();
                menu(listgame, listSPT, game2048, img, font);
            }
        }
        settingTab.clear(sf::Color::White);

        coverButton.setPosition(78 * button, 178);
        settingTab.draw(coverButton);

        enter.setPosition(50, 20);
        settingTab.draw(enter);

        undo.setPosition(50, 150);
        settingTab.draw(undo);

        coverInput.setPosition(50, 60);
        settingTab.draw(coverInput);

        input.setPosition(50, 60);
        settingTab.draw(input);

        onButton.setPosition(80, 180);
        settingTab.draw(onButton);

        offButton.setPosition(160, 180);
        settingTab.draw(offButton);

        sub2.setPosition(100, 230);
        settingTab.draw(sub2);

        if (wrong) {
            sub.setPosition(50, 110);
            settingTab.draw(sub);

            if (pressSave) {
                sub3.setPosition(100, 250);
                settingTab.draw(sub3);
            }
        }
        settingTab.display();
    }
}

void menu(game* listgame[], int& listSPT, game*& game2048, sf::Texture img[], sf::Font font) {
    top rank[25];
    getTop20Data(rank, "top20.bin");

    sf::Sprite logo(img[14]);

    sf::Text newGame("NEW GAME", font, 30);
    newGame.setFillColor(sf::Color::Black);
    newGame.setStyle(sf::Text::Bold);

    sf::Text settings("SETTINGS", font, 30);
    settings.setFillColor(sf::Color::Black);
    settings.setStyle(sf::Text::Bold);

    sf::Text top20("TOP 20", font, 30);
    top20.setFillColor(sf::Color::Black);
    top20.setStyle(sf::Text::Bold);

    sf::Text resume("RESUME", font, 30);
    resume.setFillColor(sf::Color::Black);
    resume.setStyle(sf::Text::Bold);

    sf::RectangleShape buttonCover(sf::Vector2f(200, 35));
    buttonCover.setFillColor(sf::Color(244, 164, 96, 150));

    int button = 0, resu = 1;

    sf::RenderWindow menuTab(sf::VideoMode(400, 500), "2048");
    while (menuTab.isOpen())
    {
        sf::Event event;
        while (menuTab.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (button == 0) { // new game button
                        menuTab.close();
                        enterNameTab(font, img, listgame, listSPT, game2048);
                    }
                    else if (button == 1) { // settings
                        menuTab.close();
                        settingTab(game2048, listgame, listSPT, img, font, SIZE);
                    }
                    else if (button == 2) { // resume button
                        menuTab.close();
                        int del = 0;
                        savedAccountListTab(font, img, listgame, listSPT, game2048, del);
                    }
                    else if (button == 3) { // top 20 button
                        menuTab.close();
                        top20Tab(game2048, listgame, listSPT, img, font, rank);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                    if (button < 3) button++;
                    if (button == 2 && !resu) button++;
                }
                else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                    if (button > 0) button--;
                    if (button == 2 && !resu) button--;
                }
            }
            else if (event.type == sf::Event::Closed) {
                menuTab.close();
            }
        }
        menuTab.clear(sf::Color::White);

        logo.setPosition(100, 20);
        menuTab.draw(logo);

        buttonCover.setPosition(100, 230 + button * 60);
        menuTab.draw(buttonCover);

        newGame.setPosition(115, 230);
        menuTab.draw(newGame);

        settings.setPosition(123, 290);
        menuTab.draw(settings);

        top20.setPosition(140, 410);
        menuTab.draw(top20);

        resume.setFillColor(sf::Color::Black);
        resume.setPosition(133, 350);
        menuTab.draw(resume);

        menuTab.display();
    }
}