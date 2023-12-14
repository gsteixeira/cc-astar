#include "astar.h"
#include <iostream>
#include <chrono>
#include <thread>


/*build an empty map*/
Map make_a_map(int height, int width) {
    Map maze;
    for (int row = 0; row < height; row++) {
        maze.push_back({});
        for (int col = 0; col < width; col++) {
            maze[row].push_back(0);
        }
    }
    return maze;
}

/* build a maze given 'width' and 'height'
 * then build N 'walls' on the map to make it a labyrinth.
 */
Map make_a_maze(int height, int width, int walls) {
    Map maze = make_a_map(height, width);
    srand(time(0));
    for (int i = 0; i < walls; i++) {
        int start_row, start_col, size, half;

        bool is_vertical = rand() & 1;
        if (is_vertical) {
            half = height / 2;
            size = rand() % height - half + 2;
            start_row = rand() % (height - size);
            start_col = rand() % width;
            int col = start_col;
            for (int row = start_row; row < start_row + size; row++) {
                maze[row][col] = eTileType::WALL;
            }
        }
        else {
            half = width / 2;
            size = rand() % width - half + 2;
            start_row = rand() % height;
            start_col = rand() % (width - size);
            int row = start_row;
            for (int col = start_col; col < start_col + size; col++) {
                maze[row][col] = eTileType::WALL;
            }
        }

        maze[0][0] = 0;
        maze[height-1][width-1] = eTileType::EMPTY;
    }
    return maze;
}

/*Show the map*/
void show_map(Map& maze) {
    for (int row = 0; row < maze.size(); row++) {
        for (int col = 0; col < maze[row].size(); col++) {
            switch (maze[row][col]) {
                case eTileType::EMPTY:
                    std::cout << " ";
                    break;
                case eTileType::WALL:
                    std::cout << "#";
                    break;
                case eTileType::PATH:
                    std::cout << ".";
                    break;
                case eTileType::HERO:
                    std::cout << "*";
                    break;
            }
        }
        std::cout << "|\n";
    }
    std::cout << "-----------------\n";
}

/*Animate the path*/
void walk_through(Map maze, std::vector<Tile> path) {
    if (!path.size()) {
        std::cout << "path was not found\n";
    }
    using namespace std::chrono_literals;
    for (Tile& t : path) {
        maze[t.x][t.y] = eTileType::HERO;
        show_map(maze);
        std::this_thread::sleep_for(5ms);
        maze[t.x][t.y] = eTileType::PATH;
    }
}


int main() {
    int width = 64;
    int height = 16;
    int walls = 15;
    Map m = make_a_maze(height, width, walls);

    show_map(m);

    AStar a = AStar(m);
    std::vector<Tile> path = a.search(0, 0, height-1, width-1);

    walk_through(m, path);
}
