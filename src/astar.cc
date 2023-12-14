#include "astar.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <set>


AStar::AStar(Map& p_world) {
    world = p_world;
}

/*main search function*/
std::vector<Tile> AStar::search(int start_x, int start_y, int goal_x, int goal_y) {
    Tile start = Tile(start_x, start_y);
    Tile target = Tile(goal_x, goal_y);

    known_tiles[start.id()] = start;

    while (has_open_tiles()) {
        Tile& current = get_closer_tile();

        if (current == target) {
            std::cout << "finish\n";
            return rebuild_path(&current, &start);
        }
        search_nearby_tiles(current);
        close_tile(current);

        show_map(world);
    }
    return {};
}

/*Search around current position for walkable paths*/
void AStar::search_nearby_tiles(Tile& from) {
    for (std::tuple<int, int> coord : get_neighbors(from)) {
        int x = std::get<0>(coord);
        int y = std::get<1>(coord);
        int other_id = cantor_pairing(x, y);

        if (!known_tiles.contains(other_id)) {
            Tile other = Tile(x, y);
            other.update_origin(&from);
            other.is_open = true;

            known_tiles[other_id] = other;
        }
        else {
            Tile& other = known_tiles.at(other_id);
            // if this other has gone a farthest distance before
            //    then we just found a new and shortest way to it.
            if (other.distance > from.distance) {
                other.update_origin(&from);
                if (!other.is_open) {
                    reopen_tile(other);
                }
            }

        }
    }
}

/*return walkable nearby tiles. eg: skip walls*/
std::vector<std::tuple<int, int>> AStar::get_neighbors(Tile& from) {
    int min_x = std::max(0, from.x - 1);
    int height = world.size();
    int width = world[0].size();
    int max_x = std::min(from.x + 1, height - 1);
    int min_y = std::max(0, from.y - 1);
    int max_y = std::min(from.y + 1, width -1);

    std::set<std::tuple<int, int>> available_tiles;
    available_tiles.emplace(std::make_tuple(min_x, from.y));
    available_tiles.emplace(std::make_tuple(max_x, from.y));
    available_tiles.emplace(std::make_tuple(from.x, min_y));
    available_tiles.emplace(std::make_tuple(from.x, max_y));

    std::vector<std::tuple<int, int>> neighbors;
    for (std::tuple<int, int> coord : available_tiles) {
        int x = std::get<0>(coord);
        int y = std::get<1>(coord);
        if (x == from.x && y == from.y){
            continue;
        }
        if (world[x][y] == eTileType::EMPTY) {
            neighbors.push_back(coord);
        }
    }
    return neighbors;
}


/*wether there is remaining open tiles*/
bool AStar::has_open_tiles() {
    for (auto it : known_tiles) {
        if (it.second.is_open) {
            return true;
        }
    }
    return false;
}

/*Reinstate a tile in the open list*/
void AStar::reopen_tile(Tile& tile) {
    tile.is_open = true;
}

/*Remove tile from open_tiles, as we are done testing it*/
void AStar::close_tile(Tile& tile) {
    tile.is_open = false;
}

/*Return the open tile with the shortest distance*/
Tile& AStar::get_closer_tile() {
    float dist = std::numeric_limits<float>::infinity();
    int closest_id = 0;
    for (auto it : known_tiles) {
        Tile& t = it.second;
        if (!it.second.is_open)
            continue;

        if (it.second.distance < dist) {
            dist = it.second.distance;
            closest_id = it.first;
        }
    }
    Tile& closer = known_tiles.at(closest_id);
    return known_tiles.at(closest_id);
}

/*Rebuild the path from each tile*/
std::vector<Tile> AStar::rebuild_path(Tile* current, Tile* start) {
    std::vector<Tile> path;
    int i = 0;

    while (current != NULL) {
        path.push_back(*current);
        current = current->came_from;
        i++;
    }
    path.push_back(*start);

    std::reverse(path.begin(), path.end());
    return path;
}

/*Return an unique value for the X, Y pair*/
int cantor_pairing(int x, int y) {
    return (std::pow(x, 2) + 3 * x + 2 * x * y + y + std::pow(y, 2)) / 2;
}

