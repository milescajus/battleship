#include "logic.h"
#include <cstdlib>
#include <iostream>
#include <vector>

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

std::vector<Coordinates> adjacent(const Coordinates c,
                                  const int height, const int width)
{
    // initialize vector with all possible adjacents (including non-valid)
    std::vector<Coordinates> adj = {{c.x - 1, c.y}, {c.x - 1, c.y + 1}, {c.x - 1, c.y - 1},
                                    {c.x, c.y - 1}, {c.x, c.y + 1},
                                    {c.x + 1, c.y}, {c.x + 1, c.y + 1}, {c.x + 1, c.y - 1}};

    std::vector<Coordinates> adj_trim;  // will contain only valid adjacent coords

    for (size_t i = 0; i < adj.size(); i++) {
        // skip coord if out of bounds, else add to adj_trim

        if (adj[i].x < 0 || adj[i].x > width - 1 || adj[i].y < 0 || adj[i].y > height - 1) {
            continue;
        } else {
            adj_trim.push_back(adj[i]);
        }
    }

    return adj_trim;
}

ResponseType guess(bool **board, const Coordinates c, const int height, const int width)
{
    if (board[c.x][c.y]) {
        std::cout << "HIT AT (" << c.x << "," << c.y <<  ")";

        return ResponseType::HIT;
    } else {
        // find adjacents, if they contain a ship it's a near miss, else just a miss

        std::vector<Coordinates> adj = adjacent(c, height, width);

        for (auto it = adj.begin(); it != adj.end(); ++it) {
            if (board[it->x][it->y]) {
                std::cout << "NEAR MISS AT (" << c.x << "," << c.y << ")";
                return ResponseType::NEAR_MISS;
            }
        }

        std::cout << "MISS AT (" << c.x << "," << c.y << ")";
        return ResponseType::MISS;
    }
}

bool contains(std::vector<Coordinates> c_list, Coordinates c)
{
    // helper function to check if coord is in vector of coords

    for (size_t i = 0; i < c_list.size(); i++) {
        if (c == c_list[i]) {
            return true;
        }
    }

    return false;
}

void r_guess(bool **board, const Coordinates g, const int height, const int width,
             Coordinates* &found, int &count, std::vector<Coordinates> &visited,
             int &guesses)
{
    // recursive guessing function

    std::cout << '\t';
    guesses++;

    switch(guess(board, g, height, width)) {
        case ResponseType::HIT:
            found[count] = g;           // add coordinates of found ship to array
            board[g.x][g.y] = false;    // prevent ship from being detected twice by removing it
            count++;
            break;
        case ResponseType::MISS:
            break;                      // move along, nothing to see here
        case ResponseType::NEAR_MISS:
            // recursively check adjacent squares
            std::vector<Coordinates> adj = adjacent(g, height, width);

            for (size_t i = 0; i < adj.size(); i++) {
                if (contains(visited, adj[i])) { continue; } // skip visited coords -- prevents loops
                else {
                    visited.push_back(adj[i]);     // add current location to places seen
                    return r_guess(board, adj[i], height, width, found, count, visited, guesses);
                }
            }

            break;
    }

    std::cout << std::endl;

    return;
}

Coordinates *find_ships(bool **board, const int width, const int height, const int ships)
{
    // TODO: FIX MEMORY LEAK IN THIS FUNCTION

    std::vector<Coordinates> visited;   // keeps track of visited coords
    Coordinates *found = (Coordinates *)malloc(sizeof(Coordinates) * ships);

    int count = 0;
    int guesses = 0;

    while(count < ships) {
        Coordinates g = {rand() % height, rand() % width};      // start with random coords

        if (contains(visited, g)) { continue; }                 // re-roll if visited (doesn't count as guess)
        else { visited.push_back(g); }                          // add coords to visited list

        r_guess(board, g, height, width, found, count, visited, guesses);       // recursively guess for ships
        std::cout << std::endl;
    }

    std::cout << to_string(guesses) + " guesses needed" << std::endl;

    return found;
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
