#include "logic.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
int main() {
    srand(time(NULL));
    int height = 6;
    int width = 6;
    int ships = 12;

    bool **board = create_board(height, width);

    place_ships(board, ships, height, width);
    print_board(board, height, width);
    Coordinates* c = find_ships(board, height, width, ships);

    std::cout << std::endl;
    for (int i = 0; i < ships; i++) {
        std::cout << c[i].x << ":" << c[i].y << std::endl;
    }

    destroy_board(board, height);
}
