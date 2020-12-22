#ifndef _POT_H_
#define _POT_H_

#include "Food.h"
#include "Object.h"
#include <list>
#include <vector>

using namespace std;

enum PotType { FLAT=0, NORMAL, CLEAN };

class Pot: public Object {
private:
    ALLEGRO_BITMAP* image;
    ALLEGRO_COLOR color;
    PotType type;
    int status = 0; // idle:0, cooking:1, cooked:2
    vector<Food*> contains;
    int maxContain;
    vector<list<Food>> recipes;
    int cook_time;
    bool ready = false;
    bool should_remove = false;
public:
    Pot(PotType);
    ~Pot();
    void draw(int width, int height, int scale);
    void insert(Food*&);
    vector<Food*> update();
    void time_ellapsed();
    void set_ready(bool ready) { this->ready = ready; };
    int get_status() { return status; };
    int get_remain_time() { return cook_time; };
};

#endif