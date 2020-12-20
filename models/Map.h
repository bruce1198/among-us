#ifndef _MAP_H_
#define _MAP_H_
#include <vector>

using namespace std;

class Line {
public:
    Line(int x1, int y1, int x2, int y2, int side, bool transparent, bool outside) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
        this->side = side;
        this->transparent = transparent;
        this->outside = outside;
    }
    ~Line() {

    }
    int x1;
    int y1;
    int x2;
    int y2;
    int side;
    bool transparent;
    bool outside;
};

class Map {
private:
    /* data */
public:
    Map(/* args */);
    ~Map();
    vector<Line> tiles;
    void load_lines();
};

#endif