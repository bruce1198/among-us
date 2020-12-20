#ifndef _CREW_H_
#define _CREW_H_

#include "Map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <list>
#include <map>
#include <string>
#include "Food.h"

using namespace std;
enum {LEFT=0, RIGHT, UP, DOWN};

class Crew {
private:
    vector<ALLEGRO_BITMAP *> images;

    ALLEGRO_BITMAP* shadow_buffer;
    ALLEGRO_BITMAP* buffer;
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

    int fullness;
    int waterness;
    int poisoness;

    Food pickedup;
    bool canPick;

    int health;

    bool initBuffer = false;
public:
    Crew() {};
    Crew(int);
    ~Crew();
    map<char, int> getPosition();
    void update(int, int);
    void draw(int, int, int);
    void reload();
    ALLEGRO_BITMAP* getShadow();
    void set_direction(int);
    void remove_direction(int);
    int get_direction();
    void load_images();
    void eat(Food&);
    void pickup(Food&);
    friend ostream& operator<<(ostream &os, Crew &hero);
    void time_elapsed();
    int get_energy();
    int get_water();
    int get_poison();
};

#endif
