#include "astar.h"

Tile::Tile(int px, int py, int pweight) {
    x = px;
    y = py;
    weight = pweight;
    distance = 0;
}

Tile::Tile() {}

void Tile::update_origin(Tile* from) {
    came_from = from;
    distance = from->distance + weight;
}

int Tile::id() {
    return cantor_pairing(x, y);
}

bool Tile::operator== (const Tile& other) const {
    return (x == other.x && y == other.y);
}
