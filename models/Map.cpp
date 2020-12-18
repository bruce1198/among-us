#include "Map.h"
#include <fstream>

using namespace std;

Map::Map() {
    load_lines();
}

void Map::load_lines() {
    fstream config("config/map.txt", ios_base::in);
    int x1, y1, x2, y2, side, transparent;
    tiles.clear();
    while(config >> x1 >> y1 >> x2 >> y2 >> side >> transparent) {
        tiles.push_back(Line(x1, y1, x2, y2, side, 0));
    }
}

Map::~Map()
{
}