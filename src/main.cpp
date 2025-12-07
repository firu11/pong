#include "include/game.hpp"
#include "include/utils.hpp"

int main() {
    game *g = new game({160, 36}, 5);

    clearTerminalScreen();
    hideCursor();

    g->run(); // run

    showCursor();

    return 0;
}
