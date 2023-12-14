#include <vector>
#include <unordered_map>

typedef std::vector<std::vector<int>> Map;

enum eTileType {
    EMPTY,
    WALL,
    HERO,
    PATH,
};


class Tile {
public:
    int x, y;
    int distance;
    int weight;
    Tile* came_from = NULL;
    bool is_open = true;

    Tile(int x, int y, int weight = 1);
    Tile();

    bool operator== (const Tile& t) const;
    void update_origin(Tile* from);
    int id();
};


class AStar {
public:
    AStar(Map& pworld);
    std::vector<Tile> search(int start_x, int start_y, int goal_x, int goal_y);

private:
    Map world;
    std::unordered_map<int, Tile> known_tiles;
    Tile& get_closer_tile();
    void search_nearby_tiles(Tile& from);

    std::vector<std::tuple<int, int>> get_neighbors(Tile& from);
    void reopen_tile(Tile& tile);
    void close_tile(Tile& tile);
    bool has_open_tiles();
    std::vector<Tile> rebuild_path(Tile* current, Tile* start);
};

void show_map(Map& maze);
int cantor_pairing(int x, int y);
