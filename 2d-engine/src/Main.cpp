#include "Game.h"

int main(int argc, char* argv[]) {

    Game game; // Si no se utiliza "new" se guarda en el stack y se borra al acabar el scope

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}