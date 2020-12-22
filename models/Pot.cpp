#include "Pot.h"
#include <iostream>

Pot::Pot(PotType type) {
    obj_type = POT;
    // pos_x = rand()%2500+100;
    // pos_y = rand()%1200+600;
        pos_x = 1440;
        pos_y = 986;
    this->type = type;
    status = 0;
    switch (type) {
        case FLAT:
            image = al_load_bitmap("assets/images/food/flat.png");
            color = al_map_rgb(96, 96, 96);
            maxContain = 4;
            break;
        case NORMAL:
            image = al_load_bitmap("assets/images/food/normal.png");
            color = al_map_rgb(224, 224, 224);
            maxContain = 4;
            break;
        case CLEAN:
            image = al_load_bitmap("assets/images/food/clean.png");
            color = al_map_rgb(204, 255, 255);
            maxContain = 1;
            break;
    }
}

Pot::~Pot() {
}

vector<Food*> Pot::update() {
    vector<Food*> tmp;
    if(should_remove) {
        tmp = contains;
        Food* newFood = new Food(STEAK, -1);
        newFood->set_pos(this->get_x()-40, this->get_y()+40);
        tmp.push_back(newFood);
        contains.empty();
        should_remove = false;
        status = 0;
    }
    switch (contains.size()) {
    case 1:
        contains[0]->set_pos(pos_x, pos_y-25);
        break;
    case 2:
        contains[0]->set_pos(pos_x-10, pos_y-25);
        contains[1]->set_pos(pos_x+10, pos_y-25);
        break;
    case 3:
        contains[0]->set_pos(pos_x-20, pos_y-25);
        contains[1]->set_pos(pos_x, pos_y-25);
        contains[2]->set_pos(pos_x+20, pos_y-25);
        break;
    case 4:
        contains[0]->set_pos(pos_x-10, pos_y-45);
        contains[1]->set_pos(pos_x+10, pos_y-45);
        contains[2]->set_pos(pos_x-10, pos_y-25);
        contains[3]->set_pos(pos_x+10, pos_y-25);
        break;
    }
    return tmp;
}

void Pot::time_ellapsed() {
    if(status == 1 && ready) {
        cout << "remain time: " << cook_time << endl; 
        cook_time--;
        if(cook_time==0) {
            status = 2;
            should_remove = true;
        }
    }
}


void Pot::insert(Food*& food) {
    cout << type << ": insert food!" << endl;
    if(contains.empty()) {
        status = 1;
        switch (type) {
        case FLAT:
            cook_time = 10;
            break;
        case NORMAL:
            cook_time = 10;
            break;
        case CLEAN:
            cook_time = 5;
            break;
        default:
            break;
        }
    }
    if(contains.size()<4) {
        contains.push_back(food);
        food->put_in_pot();
    }
}

void Pot::draw(int width, int height, int scale) {

    float scale_factor = 0.38*0.3*height/1080;
    if(image != NULL) {
        int w, h;
        w = al_get_bitmap_width(image);
        h = al_get_bitmap_height(image);
        al_draw_scaled_bitmap(image, 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, 0);
    }
    else
        al_draw_filled_rectangle(pos_x-10, pos_y-15, pos_x+10, pos_y+15, color);
}