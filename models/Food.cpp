#include "Food.h"

Food::Food(Gredient g, int id) {
    this->fid = id;
    obj_type = FOOD;
    pos_x = rand()%2500+100;
    pos_y = rand()%1200+600;
    // pos_x = 1050 + 20*id;
    // pos_y = 800;
    type = g;
    inPot = false;
    switch (type) {
    case LOBSTER:
        image = al_load_bitmap("assets/images/food/lobster.png");
        color = al_map_rgb(10, 10, 10);
        ounce = 20;
        water = 10;
        poison = 20;
        break;
    case LU:
        image = al_load_bitmap("assets/images/food/lu.png");
        color = al_map_rgb(0, 255, 0);
        ounce = 5;
        water = 15;
        poison = 5;
        break;
    case EGG:
        image = al_load_bitmap("assets/images/food/egg.png");
        color = al_map_rgb(0, 255, 0);
        ounce = 10;
        water = 10;
        poison = 5;
        break;
    case RICE:
        image = al_load_bitmap("assets/images/food/rice.png");
        color = al_map_rgb(255, 255, 255);
        ounce = 10;
        water = 0;
        poison = 5;
        break;
    case TSON:
        image = al_load_bitmap("assets/images/food/tson.png");
        color = al_map_rgb(0, 100, 0);
        ounce = 5;
        water = 0;
        poison = 5;
        break;
    case MEAT:
        image = al_load_bitmap("assets/images/food/meat.png");
        color = al_map_rgb(255, 0, 0);
        ounce = 20;
        water = 0;
        poison = 20;
        break;
    case BROCO:
        image = al_load_bitmap("assets/images/food/broco.png");
        color = al_map_rgb(0, 150, 0);
        ounce = 10;
        water = 10;
        poison = 5;
        break;
    case CARROT:
        image = al_load_bitmap("assets/images/food/carrot.png");
        color = al_map_rgb(255, 128, 0);
        ounce = 10;
        water = 10;
        poison = 30;
        break;
    case MUSH:
        image = al_load_bitmap("assets/images/food/mushroom.png");
        color = al_map_rgb(153, 76, 0);
        ounce = 10;
        water = 10;
        poison = 50;
        break;
    case RIVER:
        image = al_load_bitmap("assets/images/food/river.png");
        color = al_map_rgb(0, 0, 255);
        ounce = 0;
        water = 60;
        poison = 30;
        break;
    case LOBSTERCOOKED:
        image = al_load_bitmap("assets/images/food/lobster_cooked.png");
        color = al_map_rgb(0, 0, 255);
        ounce = 20;
        water = 30;
        poison = 0;
        break;
    case FRIED:
        image = al_load_bitmap("assets/images/food/fried_rice.png");
        color = al_map_rgb(0, 0, 255);
        ounce = 40;
        water = 10;
        poison = 0;
        break;
    case STEAK:
        image = al_load_bitmap("assets/images/food/steak.png");
        color = al_map_rgb(0, 0, 255);
        ounce = 40;
        water = 20;
        poison = 0;
        break;
    case WATER:
        image = al_load_bitmap("assets/images/food/water.png");
        color = al_map_rgb(0, 0, 255);
        ounce = 0;
        water = 50;
        poison = 0;
        break;
    case POOP:
        image = al_load_bitmap("assets/images/food/poop.png");
        color = al_map_rgb(0, 0, 0);
        ounce = 0;
        water = 0;
        poison = 100;
    default:
        break;
    }
}

Food::~Food() {
    if(image!=NULL)
        al_destroy_bitmap(image);
}

void Food::drawOnScreen(int x, int y) {
    int w, h;
    float scale_factor;
    scale_factor = 0.5;
    switch (type) {
    case LOBSTER:
        break;
    case LU:
        break;
    case RICE:
        break;
    case TSON:
        break;
    case MEAT:
        break;
    case BROCO:
        break;
    case CARROT:
        break;
    case MUSH:
        break;
    case RIVER:
        break;
    default:
        break;
    }

    w = al_get_bitmap_width(image);
    h = al_get_bitmap_height(image);
    al_draw_scaled_bitmap(image, 0, 0, w, h, x - w*scale_factor/2, y - h*scale_factor/2, w*scale_factor, h*scale_factor, 0);
}

void Food::draw(int width, int height, int scale) {
    int w, h;
    float scale_factor = 1.9*0.25;

    if(image != NULL) {
        w = al_get_bitmap_width(image);
        h = al_get_bitmap_height(image);
        al_draw_scaled_bitmap(image, 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, 0);
    }
    else
        al_draw_filled_circle(pos_x, pos_y, 10, color);

}