#include "include/game.hpp"
#include "include/utils.hpp"

#include <iostream>
#include <fstream>
#include <string>

const std::string HELP_TEXT = R"---(
PONG GAME TERMINAL
------------------
Usage:
  -h, --help      Show this help message
  --width <val>   Set canvas width (default: 160)
  --height <val>  Set canvas height (default: 36)

Controls:
  W / UP          - Move up
  S / DOWN        - Move down
  Q / CTRL + C    - Quit game
)---";

void printHelp() {
    std::cout << HELP_TEXT << std::endl;
}

int main(int argc, char *argv[]) {
    // default values
    int width = 160;
    int height = 36;

    // argument parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }

        if (arg == "--width" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        } else if (arg == "--height" && i + 1 < argc) {
            height = std::stoi(argv[++i]);
        }
    }

    // game itself
    game *g = new game({width, height}, 5);

    clearTerminalScreen();
    hideCursor();

    g->run(); // run

    showCursor();

    delete g;
    return 0;
}
