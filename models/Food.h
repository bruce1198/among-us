#ifndef _FOOD_H_
#define _FOOD_H_

#include <allegro5/allegro_primitives.h>
#include <time.h>

enum Gredient { LOBSTER=0, LU, EGG, RICE, TSON, MEAT, BROCO, CARROT, MUSH, RIVER, WATER, FRIED, LOBSTERCOOKED, STEAK};

class Food {
private:
    ALLEGRO_BITMAP* uncooked_image;
    ALLEGRO_BITMAP* cooked_image;
    int ounce;
    int water;
    int poison;
    bool cooked;
    int timeToCook;
    int type;

    ALLEGRO_COLOR color;

    int pos_x;
    int pos_y;
public:
    Food(Gredient);
    ~Food();
    void draw(int, int, int);
    void set_pos(int, int);
    int get_x() { return pos_x; };
    int get_y() { return pos_y; };
};

#endif