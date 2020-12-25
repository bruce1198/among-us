#ifndef _WELCOME_H_
#define _WELCOME_H_

#define WHITE_COLOR al_map_rgb(255, 255, 255)
#define SHADOW_COLOR al_map_rgba(0, 0, 0, 200)
#define HOVER_COLOR al_map_rgb(255, 0, 0)

#include <iostream>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

using namespace std;

class Welcome {
private:
    ALLEGRO_BITMAP* image;
    ALLEGRO_FONT *font = NULL;
    int hover;
public:
    Welcome(/* args */);
    ~Welcome();
    void draw(int, int);
    void update(int, int, int, int);
    int click(int, int, int, int);
};

#endif
