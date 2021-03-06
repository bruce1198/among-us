#include "Crew.h"
#include <cmath>
#include <iostream>

const int draw_frequency = 3;
Map game_map;

Crew::Crew(int id, Color color) {
    obj_type = CREW;
    this->color = color;
    if(id==0) {
        pos_x = 1440;
        pos_y = 986;
    }
    else if(id==1) {
        pos_x = 2040;
        pos_y = 616;
    }
    speed = 3;
    counter = 0;
    direction = 0;
    for(int i=0; i<4; i++) direction_ary[i] = false;

    fullness = 100;
    waterness = 100;
    poisoness = 0;
    canPick = true;
    
    load_images();
}

Crew::~Crew() {
    for(auto image: images) {
        al_destroy_bitmap(image);
    }
    if(shadow_buffer!=NULL)
        al_destroy_bitmap(shadow_buffer);
    if(pickedup!=NULL) {
        delete pickedup;
    }
}

float Crew::get_energy() {
    return fullness;
}

float Crew::get_water() {
    return waterness;
}

float Crew::get_poison() {
    return poisoness;
}

void Crew::load_images() {
    string buf = "assets/images/";
    switch (color) {
    case BLACK:
        buf += "black";
        break;
    case BLUE:
        buf += "blue";
        break;
    case BROWN:
        buf += "brown";
        break;
    case CYAN:
        buf += "cyan";
        break;
    case FORTEGREEN:
        buf += "fortegreen";
        break;
    case GREEN:
        buf += "green";
        break;
    case LIME:
        buf += "lime";
        break;
    case ORANGE:
        buf += "orange";
        break;
    case PINK:
        buf += "pink";
        break;
    case PURPLE:
        buf += "purple";
        break;
    case RED:
        buf += "red";
        break;
    case TAN:
        buf += "tan";
        break;
    case WHITE:
        buf += "white";
        break;
    case YELLOW:
        buf += "yellow";
        break;
    }
    for(int i=1; i<=23; i++) {
        string path = buf + "/"+to_string(i)+".png";
        ALLEGRO_BITMAP *img;
        img = al_load_bitmap(path.c_str());
        if(img)
            images.push_back(img);
    }
}

map<char, int> Crew::getPosition() {
    map<char, int> m;
    m.insert(pair<char, int>('x', pos_x));
    m.insert(pair<char, int>('y', pos_y));
    return m;
}

ostream& operator<<(ostream &os, Crew &hero) {
    return os;
}

void Crew::draw(int width, int height, int scale) {
    int w, h;
    float scale_factor;
    if(this->direction==0 || this->direction==15 || this->direction==3 || this->direction==12) {
        w = al_get_bitmap_width(images[22]);
        h = al_get_bitmap_height(images[22]);
        scale_factor = 0.38*0.25;
        al_draw_scaled_bitmap(images[22], 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, flag);
    }
    else {
        w = al_get_bitmap_width(images[sprite_pos]);
        h = al_get_bitmap_height(images[sprite_pos]);
        scale_factor = 0.2*0.25;
        al_draw_scaled_bitmap(images[sprite_pos], 0, 0, w, h, pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, w*scale_factor, h*scale_factor, flag);
        al_draw_rectangle(pos_x - w*scale_factor/2, pos_y - h*scale_factor/2, pos_x + w*scale_factor/2, pos_y + h*scale_factor/2, al_map_rgb(255, 0, 0), 2);
    }


    scale_factor = scale*height/1080;
    float view_x1 = pos_x - width/(4*scale_factor)-1;
    float view_x2 = pos_x + width/(4*scale_factor);
    float view_y1 = pos_y - height/(2*scale_factor)-1;
    float view_y2 = pos_y + height/(2*scale_factor);

    buffer = al_get_target_bitmap();
    if(!initBuffer) {
        shadow_buffer = al_create_bitmap(al_get_bitmap_width(buffer), al_get_bitmap_height(buffer));
        initBuffer = true;
    }
    // draw shadow to shadow_buffer
    al_set_target_bitmap(shadow_buffer);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
    for(auto line: game_map.tiles) {
        if(!line.transparent) {
            if(((view_x1<line.x1&&line.x1<view_x2) || (view_x1<line.x2&&line.x2<view_x2)) 
                && ((view_y1<line.y1&&line.y1<view_y2) || (view_y1<line.y2&&line.y2<view_y2))) {
                // wall in viewport
                if((line.side==1||line.side==0) && pos_y>=line.y1) {
                    float ratio1 = (line.y1-pos_y)/(line.x1-pos_x);
                    float point_y1 = view_y1;
                    float point_x1 = (point_y1-pos_y)/ratio1+pos_x;
                    float ratio2 = (line.y2-pos_y)/(line.x2-pos_x);
                    float point_y2 = view_y1;
                    float point_x2 = (point_y2-pos_y)/ratio2+pos_x;
                    float vertices[] = {
                        point_x1, point_y1,
                        (float)line.x1, (float)line.y1,
                        (float)line.x2, (float)line.y2,
                        point_x2, point_y2
                    };
                    al_draw_filled_polygon(vertices, 4, al_map_rgba(0, 0, 0, 200));
                }
                else if((line.side==1||line.side==0) && pos_y<=line.y1) {
                    float ratio1 = (line.y1-pos_y)/(line.x1-pos_x);
                    float point_y1 = view_y2;
                    float point_x1 = (point_y1-pos_y)/ratio1+pos_x;
                    float ratio2 = (line.y2-pos_y)/(line.x2-pos_x);
                    float point_y2 = view_y2;
                    float point_x2 = (point_y2-pos_y)/ratio2+pos_x;
                    float vertices[] = {
                        (float)line.x1, (float)line.y1,
                        point_x1, point_y1,
                        point_x2, point_y2,
                        (float)line.x2, (float)line.y2,
                    };
                    al_draw_filled_polygon(vertices, 4, al_map_rgba(0, 0, 0, 200));
                }
                else if((line.side==2||line.side==3) && pos_x<=line.x1) {
                    float ratio1 = (line.y1-pos_y)/(line.x1-pos_x);
                    float point_x1 = view_x2;
                    float point_y1 = (point_x1-pos_x)*ratio1+pos_y;
                    float ratio2 = (line.y2-pos_y)/(line.x2-pos_x);
                    float point_x2 = view_x2;
                    float point_y2 = (point_x2-pos_x)*ratio2+pos_y;
                    float vertices[] = {
                        (float)line.x1, (float)line.y1,
                        (float)line.x2, (float)line.y2,
                        point_x2, point_y2,
                        point_x1, point_y1,
                    };
                    al_draw_filled_polygon(vertices, 4, al_map_rgba(0, 0, 0, 200));
                }
                else if((line.side==2||line.side==3) && pos_x>=line.x1) {
                    float ratio1 = (line.y1-pos_y)/(line.x1-pos_x);
                    float point_x1 = view_x1;
                    float point_y1 = (point_x1-pos_x)*ratio1+pos_y;
                    float ratio2 = (line.y2-pos_y)/(line.x2-pos_x);
                    float point_x2 = view_x1;
                    float point_y2 = (point_x2-pos_x)*ratio2+pos_y;
                    float vertices[] = {
                        point_x1, point_y1,
                        point_x2, point_y2,
                        (float)line.x2, (float)line.y2,
                        (float)line.x1, (float)line.y1,
                    };
                    al_draw_filled_polygon(vertices, 4, al_map_rgba(0, 0, 0, 200));
                    // al_draw_line(line.x1, line.y1, point_x1, point_y1, al_map_rgb(255, 0, 0), 2);
                    // al_draw_line(line.x2, line.y2, point_x2, point_y2, al_map_rgb(255, 0, 0), 2);
                }
            }
        }
    }
    
    al_set_target_bitmap(buffer);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
}
ALLEGRO_BITMAP* Crew::getShadow() {
    return this->shadow_buffer;
}

void Crew::time_elapsed() {
    fullness -= 0.3;
    if(fullness<=0) fullness=0;
    waterness -= 0.45;
    if(waterness<=0) waterness=0;
}

int Crew::eat() {
    if(pickedup!=NULL) {
        if(pickedup->get_type()==FOOD) {
            Food* tmp = (Food*)pickedup;
            fullness += tmp->get_ounce();
            if(fullness>100) fullness = 100;
            waterness += tmp->get_water();
            if(waterness>100) waterness = 100;
            poisoness += tmp->get_poison();
            return tmp->get_id();
        }
    }
    return -1;
}

void Crew::pick(Object*& food) {
    // pickup
    if(canPick) {
        pickedup = food;
        canPick = false;
    }
}

Object* Crew::put() {
    if(!canPick) {
        pickedup->set_pos(pos_x, pos_y);
        Object* tmp = pickedup;
        pickedup = NULL;
        canPick = true;
        return tmp;
    }
    return nullptr;
}

bool Crew::update(int width, int height) {

    // update crew status
    int w, h;
    float scale_factor = 0.2*0.25;
    w = al_get_bitmap_width(images[sprite_pos])*scale_factor;
    h = al_get_bitmap_height(images[sprite_pos])*scale_factor;
    counter++;
    if(direction_ary[LEFT] && !direction_ary[RIGHT]) flag = 1;
    if(!direction_ary[LEFT] && direction_ary[RIGHT]) flag = 0;
    if(this->direction==0 || this->direction==15 || this->direction==3 || this->direction==12) {

    }
    else {
        sprite_pos = (sprite_pos + 1) % 22;
        float scale = sqrt(2); 
        // int y1_offset = 25;
        // int y2_offset = 30;
        // int x1_offset = 25;
        // int x2_offset = 15;
        int y1_offset = 0;
        int y2_offset = 0;
        int x1_offset = 0;
        int x2_offset = 0;
        int stuck_up = false;
        int stuck_down = false;
        int stuck_left = false;
        int stuck_right = false;
        for(auto line: game_map.tiles) {
            if(line.side==0) {// face down
                if(pos_y+h/2-10 >= line.y1-speed && pos_y+h/2-10<=line.y1 && pos_x+w/2>line.x1 && pos_x-w/2<line.x2
                    && (this->direction==4 || this->direction==5 || this->direction==6 || this->direction==7)) {
                    pos_y = line.y1-h/2+10;
                    stuck_up = true;
                }
            }
            if(line.side==1) {// face up
                if(pos_y+h/2 <= line.y1+speed && pos_y+h/2>=line.y1 && pos_x+w/2>line.x1 && pos_x-w/2<line.x2
                    && (this->direction==8 || this->direction==9 || this->direction==10 || this->direction==11)) {
                    pos_y = line.y1-h/2;
                    stuck_down = true;
                }
            }
            if(line.side==2) {// face left
                if(line.outside) {
                    if(pos_x+w/2 <= line.x1+speed && pos_x+w/2>=line.x1 && pos_y+h/2>line.y1 && pos_y+h/2-10<line.y2
                        && (this->direction==2 || this->direction==6 || this->direction==10 || this->direction==14)) {
                        pos_x = line.x1-w/2;
                        stuck_right = true;
                    }
                }
                else {
                    if(pos_x+w/2 <= line.x1+speed && pos_x+w/2>=line.x1 && pos_y+h/2>line.y1 && pos_y-h/2<line.y2
                        && (this->direction==2 || this->direction==6 || this->direction==10 || this->direction==14)) {
                        pos_x = line.x1-w/2;
                        stuck_right = true;
                    }
                }
            }
            if(line.side==3) {// face right
                if(line.outside) {
                    if(pos_x-w/2 >= line.x1-speed && pos_x-w/2<=line.x1 && pos_y+h/2>line.y1 && pos_y+h/2-10<line.y2
                        && (this->direction==1 || this->direction==5 || this->direction==9 || this->direction==13)) {
                        pos_x = line.x1+w/2;
                        stuck_left = true;
                    }
                }
                else {
                    if(pos_x-w/2 >= line.x1-speed && pos_x-w/2<=line.x1 && pos_y+h/2>line.y1 && pos_y-h/2<line.y2
                        && (this->direction==1 || this->direction==5 || this->direction==9 || this->direction==13)) {
                        pos_x = line.x1+w/2;
                        stuck_left = true;
                    }
                }
            }
        }
        switch(this->direction) {
            case 1: if(!stuck_left) pos_x -= speed; break;
            case 2: if(!stuck_right) pos_x += speed; break;
            case 4: if(!stuck_up) pos_y -= speed; break;
            case 5: 
                if(!stuck_left) pos_x -= speed/scale; 
                if(!stuck_up) pos_y -= speed/scale; 
            break;
            case 6: 
                if(!stuck_right) pos_x += speed/scale; 
                if(!stuck_up) pos_y -= speed/scale; 
            break;
            case 7: if(!stuck_up) pos_y -= speed; break;
            case 8: if(!stuck_down) pos_y += speed; break;
            case 9: 
                if(!stuck_left) pos_x -= speed/scale; 
                if(!stuck_down) pos_y += speed/scale; 
            break;
            case 10: 
                if(!stuck_right) pos_x += speed/scale; 
                if(!stuck_down) pos_y += speed/scale; 
            break;
            case 11: if(!stuck_down) pos_y += speed; break;
            case 13: if(!stuck_left) pos_x -= speed; break;
            case 14: if(!stuck_right) pos_x += speed; break;
        }
    }

    // update pickedup food position
    if(pickedup)
        pickedup->set_pos(pos_x, pos_y-30);

    return poisoness>=100 || fullness <=0 || waterness <= 0;
}

void Crew::set_direction(int dir) {
    direction_ary[dir] = true;
    int sum = 0;
    for(int i=0; i<4; i++) {
        if(direction_ary[i]) sum+=1<<i;
    }
    this->direction = sum;
}
void Crew::remove_direction(int dir) {
    direction_ary[dir] = false;
    int sum = 0;
    for(int i=0; i<4; i++) {
        if(direction_ary[i]) sum+=1<<i;
    }
    this->direction = sum;
}

int Crew::get_direction() {
    return -1;
}

void Crew::reload() {
    game_map.load_lines();
}