#include "logic.h"
#include <cstdlib>
#include <iostream>

using std::to_string;

bool **create_board(const int height, const int width)
{
    bool **board = (bool **)malloc(sizeof(bool *) * height);
    for (int i = 0; i < height; i++) {
        board[i] = (bool *)malloc(sizeof(bool) * width);
    }
    return board;
}

void destroy_board(bool **board, const int height)
{
    for (int i = 0; i < height; ++i) {
        free(board[i]);
    }
    free(board);
}

void place_ships(bool **board, const int shipcount, const int width,
        const int height)
{
    int placed = 0;

    // initialize board
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j] = false;
        }
    }

    // place ships on free spaces until all are placed
    while (placed < shipcount) {
        int a = rand() % height;
        int b = rand() % width;
        
        if (!board[a][b]) {
            board[a][b] = true;
            placed++;
        }
    }
}

Coordinates* find_ships(bool **board, const int width, const int height, const int ship_count)
{
    Coordinates *c = (Coordinates *)malloc(sizeof(Coordinates) * ship_count);
    // Coordinates c[ship_count];
    int found = 0;
    int guesses = 0;

    while(found < ship_count) {
        guesses++;

        int a = rand() % height;
        int b = rand() % width;

        switch(guess(board, a, b, height, width)) {
            case ResponseType::HIT:
                std::cout << "HIT AT (" + to_string(a) + "," + to_string(b) + ")" << std::endl;
                c[found] = {a, b};      // add coordinates of found ship to array
                board[a][b] = false;    // prevent ship from being detected twice by removing it
                found++;
                break;
            case ResponseType::MISS:
                std::cout << "MISS AT (" + to_string(a) + "," + to_string(b) + ")" << std::endl;
                break;
            case ResponseType::NEAR_MISS:
                std::cout << "NEAR MISS AT (" + to_string(a) + "," + to_string(b) + ")" << std::endl;
                break;
        }
    }

    std::cout << to_string(guesses) + " guesses needed" << std::endl;

    return c;
}

int edge(const int n, const int min, const int max)
{
    if (n < min) return min;
    else if (n > max) return max;
    else return n;
}

bool adjacent(bool **board, const int x, const int y, const int height, const int width)
{
    int x_lb = edge(x - 1, 0, height - 1);
    int x_ub = edge(x + 1, 0, height - 1);
    int y_lb = edge(y - 1, 0, width - 1);
    int y_ub = edge(y + 1, 0, width - 1);

    return (board[x_lb][y] || board[x_lb][y_ub] || board[x_lb][y_lb] ||
            board[x][y_lb] || board[x][y_ub] ||
            board[x_ub][y] || board[x_ub][y_ub] || board[x_ub][y_lb]);
}

ResponseType guess(bool **board, const int x, const int y, const int height, const int width)
{
    if (board[x][y]) {
        return ResponseType::HIT;
    } else if (adjacent(board, x, y, height, width)) {
        return ResponseType::NEAR_MISS;
    } else {
        return ResponseType::MISS;
    }
}

void print_board(bool **board, const int width, const int height)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
}
