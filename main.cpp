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
		
		for (int i = 0; i < ship_count; i++) {
        std::cout << c[i].x;
        std::cout << ":";
        std::cout << c[i].y << std::endl;
			}

    destroy_board(board, height);
}
