#include "Food.h"

Food::Food(Gredient g) {
    pos_x = rand()%2500+100;
    pos_y = rand()%1200+600;
    type = g;
    switch (type) {
    case LOBSTER:
        color = al_map_rgb(10, 10, 10);
        break;
    case LU:
        color = al_map_rgb(0, 255, 0);
        break;
    case RICE:
        color = al_map_rgb(255, 255, 255);
        break;
    case TSON:
        color = al_map_rgb(0, 100, 0);
        break;
    case MEAT:
        color = al_map_rgb(255, 0, 0);
        break;
    case BROCO:
        color = al_map_rgb(0, 150, 0);
        break;
    case CARROT:
        color = al_map_rgb(255, 128, 0);
        break;
    case MUSH:
        color = al_map_rgb(153, 76, 0);
        break;
    case RIVER:
        color = al_map_rgb(0, 0, 255);
        break;
    default:
        break;
    }
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

    al_draw_filled_circle(pos_x, pos_y, 10, color);

}

void Food::set_pos(int x, int y) {
    pos_x = x;
    pos_y = y;
}