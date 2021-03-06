#ifndef _CREW_H_
#define _CREW_H_

#include "Map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <list>
#include <map>
#include <string>
#include "Object.h"
#include "Food.h"

using namespace std;
enum {LEFT=0, RIGHT, UP, DOWN};

enum Color { BLACK=0, BLUE, BROWN, CYAN, FORTEGREEN, GREEN, LIME, ORANGE, PINK, PURPLE, RED, TAN, WHITE, YELLOW};

class Crew: public Object {
private:
    vector<ALLEGRO_BITMAP *> images;

    ALLEGRO_BITMAP* shadow_buffer = NULL;
    ALLEGRO_BITMAP* buffer = NULL;
    int id;
    int counter;
    int sprite_pos = 0;
    int radius;
    bool direction_ary[4];
    int direction = 1;
    int flag = 1;
    int speed;
    Color color;

    float fullness;
    float waterness;
    float poisoness;

    Object* pickedup = NULL;
    bool canPick;

    int health;

    bool initBuffer = false;
public:
    Crew() {};
    Crew(int, Color c);
    ~Crew();
    map<char, int> getPosition();
    bool update(int, int);
    void draw(int, int, int);
    void reload();
    ALLEGRO_BITMAP* getShadow();
    void set_direction(int);
    void remove_direction(int);
    int get_direction();
    void load_images();
    int eat();
    void pick(Object*&);
    Object* put();
    bool ableToPick() { return canPick; }
    friend ostream& operator<<(ostream &os, Crew &hero);
    void time_elapsed();
    float get_energy();
    float get_water();
    float get_poison();
};

#endif
