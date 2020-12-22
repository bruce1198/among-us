#include "Fire.h"

Fire::Fire() {
    
}

Fire::Fire(int x, int y){
    pos_x = x;
    pos_y = y;
    image = al_load_bitmap("assets/images/food/fire.png");
}

Fire::~Fire()
{
}

void Fire::draw(int width, int height, int scale) {

    float scale_factor = 0.38*0.3*height/1080;
    
    if(image != NULL) {
        int w, h;
        w = al_get_bitmap_width(image);
        h = al_get_bitmap_height(image);
        al_draw_scaled_bitmap(image, 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, 0);
    }
    else
    al_draw_filled_triangle(pos_x, pos_y-5, pos_x-5, pos_y+5, pos_x+5, pos_y+5, al_map_rgb(255, 0, 0));
}