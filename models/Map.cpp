#include "Map.h"
#include <fstream>

using namespace std;

Map::Map() {
    fstream config("config/map.txt", ios_base::in);
    int x1, y1, x2, y2, side, transparent;
    while(config >> x1 >> y1 >> x2 >> y2 >> side >> transparent) {
        tiles.push_back(Line(x1, y1, x2, y2, side, 0));
    }
    // tiles.push_back(Line(154, 760, 787, 760, 0));
    // tiles.push_back(Line(483, 1090, 808, 1090, 0));
    // tiles.push_back(Line(1200, 875, 1560, 875, 0));
    // tiles.push_back(Line(1283, 1070, 1560, 1070, 0));//face down
    // tiles.push_back(Line(1283, 1025, 1560, 1025, 1));//face up
    // tiles.push_back(Line(1200, 830, 1560, 830, 1));
    // tiles.push_back(Line(1560, 840, 1560, 1025, 2));//face left
    // tiles.push_back(Line(1565, 840, 1565, 1025, 3));//face right
}

Map::~Map()
{
}