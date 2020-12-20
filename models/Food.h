#ifndef _FOOD_H_
#define _FOOD_H_

#include <allegro5/allegro_primitives.h>

class Food {
private:
    ALLEGRO_BITMAP* uncooked_image;
    ALLEGRO_BITMAP* cooked_image;
    int ounce;
    int water;
    int poison;
    bool cooked;
    int timeToCook;

    int pos_x;
    int pos_y;
public:
    Food();
    ~Food();
    void draw(int, int, int);
    void set_pos(int, int);
    int get_x() { return pos_x; };
    int get_y() { return pos_y; };
};

#endif