#include "./Game/Game.h"

int main(int argc, char* argv[]) {

    // Si no se utiliza "new" al crear la instancia, se guarda
    // en el stack y se borra de la memoria al acabar el scope
    Game game; 

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}