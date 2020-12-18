#include "models/GameWindow.h"

int main(int argc, char *argv[])
{
    GameWindow *game= new GameWindow();

    game->play();

    delete game;
    return 0;
}
