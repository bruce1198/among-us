#include "Pot.h"
#include <iostream>

Pot::Pot(PotType type) {
    obj_type = POT;
    pos_x = rand()%2500+100;
    pos_y = rand()%1200+600;
    // pos_x = 1440;
    // pos_y = 986;
    this->type = type;
    status = 0;
    switch (type) {
        case FLAT:
            image = al_load_bitmap("assets/images/food/flat.png");
            color = al_map_rgb(96, 96, 96);
            maxContain = 4;
            recipes.push_back({EGG, RICE, TSON});
            recipes.push_back({MEAT, BROCO, CARROT, MUSH});
            recipes_target.push_back(FRIED);
            recipes_target.push_back(STEAK);
            break;
        case NORMAL:
            image = al_load_bitmap("assets/images/food/normal.png");
            color = al_map_rgb(224, 224, 224);
            recipes.push_back({LOBSTER, LU});
            recipes_target.push_back(LOBSTERCOOKED);
            maxContain = 4;
            break;
        case CLEAN:
            image = al_load_bitmap("assets/images/food/clean.png");
            color = al_map_rgb(204, 255, 255);
            recipes.push_back({RIVER});
            recipes_target.push_back(WATER);
            maxContain = 1;
            break;
        for(auto& v: recipes) {
            v.sort();
        }
    }
}

Pot::~Pot() {
    if(image!=NULL)
        al_destroy_bitmap(image);
    for(auto food: contains) {
        if(food) {
            delete food;
        }
    }
}

bool compare (Food*& first, Food*& second) {
    return first->get_food_type() < second->get_food_type();
}


vector<Food*> Pot::update() {
    vector<Food*> tmp;
    if(should_remove) {
        tmp = contains;
        bool match = false;
        int i=-1;
        int target;
        for(auto v: recipes) {
            i++;
            if(tmp.size() != v.size()) continue;
            sort(tmp.begin(), tmp.end(), compare);
            match = true;
            target = i;
            int j=0;
            for(auto g: v) {
                if(tmp[j]->get_food_type()!=g) {
                    match = false;
                    break;
                }
                j++;
            }
        }
        Food* newFood;
        if(match) {
            newFood = new Food(recipes_target[target], -1);
        }
        else {
            newFood = new Food(POOP, -1);
        }
        newFood->set_pos(this->get_x()-40, this->get_y()+40);
        tmp.push_back(newFood);
        contains.clear();
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
    if(food->get_food_type()==STEAK || food->get_food_type()==FRIED
        || food->get_food_type()==LOBSTERCOOKED || food->get_food_type()==WATER)
        return;
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

    float scale_factor = 0.57*height/1080;
    if(image != NULL) {
        int w, h;
        w = al_get_bitmap_width(image);
        h = al_get_bitmap_height(image);
        al_draw_scaled_bitmap(image, 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, 0);
    }
    else
        al_draw_filled_rectangle(pos_x-10, pos_y-15, pos_x+10, pos_y+15, color);
}