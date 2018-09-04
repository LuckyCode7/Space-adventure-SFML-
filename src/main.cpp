#include <../inc/Game.h>
#include <ctime>

int main()
{
    setlocale(LC_ALL, "");
    srand(time(nullptr));

    std::unique_ptr<Game> game(new Game("Space adventure"));
    game->startGame();

    return 0;
}
