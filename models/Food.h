#ifndef _FOOD_H_
#define _FOOD_H_

#include <allegro5/allegro_primitives.h>
#include <time.h>
#include "Object.h"

enum Gredient { LOBSTER=0, LU, EGG, RICE, TSON, MEAT, BROCO, CARROT, MUSH, RIVER, WATER, FRIED, LOBSTERCOOKED, STEAK, POOP};

class Food: public Object {
private:
    ALLEGRO_BITMAP* image;
    int fid;
    int ounce;
    int water;
    int poison;
    bool cooked;
    int timeToCook;
    int type;
    bool inPot;

    ALLEGRO_COLOR color;
public:
    Food(Gredient, int);
    ~Food();
    void draw(int, int, int);
    void drawOnScreen(int, int);
    int get_food_type() { return type; };
    int get_id() { return fid; };
    int get_ounce() { return ounce; };
    int get_water() { return water; };
    int get_poison() { return poison; };
    bool is_in_pot() { return inPot; };
    void put_in_pot() { inPot = true; };
    void set_id(int id) { fid=id; };
};

#endif