#ifndef _CREW_H_
#define _CREW_H_

#include "Map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <map>
#include <vector>
#include <string>

using namespace std;
enum {LEFT=0, RIGHT, UP, DOWN};

class Crew {
private:
    vector<ALLEGRO_BITMAP *> images;
    int id;
    int counter;
    int sprite_pos = 0;
    int radius;
    bool direction_ary[4];
    int direction = 1;
    int flag = 1;
    float pos_x;
    float pos_y;
    int speed;
    string color;
public:
    Crew() {};
    Crew(int);
    ~Crew();
    map<char, int> getPosition();
    void update(int, int);
    void draw(int, int);
    void set_direction(int);
    void remove_direction(int);
    int get_direction();
    void load_images();
    friend ostream& operator<<(ostream &os, Crew &hero);
};

#endif
