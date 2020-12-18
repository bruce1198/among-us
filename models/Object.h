#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <allegro5/allegro_acodec.h>

class Object {
protected:
    float pos_x;
    float pos_y;
    float angle;
    int speed;
    ALLEGRO_COLOR color;
public:
    Object();
    ~Object();
};


#endif