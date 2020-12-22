#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

enum ObjType {CREW, FOOD, POT};

class Object {
protected:
    float pos_x;
    float pos_y;
    ObjType obj_type;
public:
    Object();
    ~Object();
    void set_pos(float x, float y) {pos_x=x; pos_y=y;};
    float get_x() { return pos_x; };
    float get_y() { return pos_y; };
    ObjType get_type() { return obj_type; };
    const float operator[](const char c) const {
        if(c=='x') return pos_x;
        else return pos_y;
    }
    float operator[](const char c) {
        if(c=='x') return pos_x;
        else return pos_y;
    }
};


#endif