#include <iostream>

#include "Game.hpp"

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [ip] [port] [numJoueur]" << std::endl;
        return EXIT_FAILURE;
    }

    Game game(argv);
    game.run();
    
    return EXIT_SUCCESS;
}
