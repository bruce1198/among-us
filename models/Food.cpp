#include "Food.h"

Food::Food() {
    pos_x = 1440;
    pos_y = 986;
}

Food::~Food() {

}

void Food::draw(int width, int height, int scale) {
    // int w, h;
    // float scale_factor;
    // if(cooked) {
    //     w = al_get_bitmap_width(cooked_image);
    //     h = al_get_bitmap_height(cooked_image);
    //     scale_factor = 0.38*0.3*height/1080;
    //     al_draw_scaled_bitmap(cooked_image, 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, 0);
    // }
    // else {
    //     w = al_get_bitmap_width(uncooked_image);
    //     h = al_get_bitmap_height(uncooked_image);
    //     scale_factor = 0.38*0.3*height/1080;
    //     al_draw_scaled_bitmap(uncooked_image, 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, 0);
    // }

    al_draw_filled_circle(pos_x, pos_y, 10, al_map_rgb(0, 255, 0));

}

void Food::set_pos(int x, int y) {
    pos_x = x;
    pos_y = y;
}