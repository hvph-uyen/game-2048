#include "library.h"
#include "support.h"
#include "game.h"
#include "file.h"
#include "gameTab.h"
#include "encrypt.h"

int SIZE = 4, UNDO_ON = 0;

int main()
{
    srand(time(0));

    sf::Font font;
    sf::Texture img[15];
    loadResourceFiles(img, font);

    game* game2048 = NULL;
    game* listgame[5];
    int spt = 0;

    bool preCheck = isThereData("savedGame.bin");
    if (preCheck) retriveGameFromFile(listgame, spt, "savedGame.bin");
    menu(listgame, spt, game2048, img, font);
    saveGameToFile(listgame, spt, "savedGame.bin");

    //releaseSavedGameData(); 

    //---------------------------------
    for (int i = 0; i < spt; ++i) deleteGameMemory(listgame[i]);
    if (!(game2048 == NULL)) deleteGameMemory(game2048);

    return 0;
}