#include "Menu.h"

Menu::Menu() {
    visibility = false;
    font = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf", 60,0);
}

Menu::~Menu() {

}

int Menu::click(int width, int height, int mouse_x, int mouse_y) {
    visibility = false;
    for(int i=0; i<3; i++) {
        if(mouse_x>=width/2-250 && mouse_x<=width/2+250 && mouse_y>=height-200+150*i && mouse_y<=height-100+150*i) {
            return i;
        }
    }
    return -1;
}

void Menu::update(int width, int height, int mouse_x, int mouse_y) {
    for(int i=0; i<3; i++) {
        if(mouse_x>=width/2-250 && mouse_x<=width/2+250 && mouse_y>=height-200+150*i && mouse_y<=height-100+150*i) {
            hover[i] = 0;
        }
        else hover[i] = -1;
    }
}

void Menu::draw(int width, int height) {
    if(visibility) {
        al_draw_filled_rounded_rectangle(width/2-300, height-250, width/2+305, height+255, 5, 5, al_map_rgba(0, 0, 0, 150));
        al_draw_filled_rounded_rectangle(width/2-300, height-250, width/2+300, height+250, 5, 5, al_map_rgb(153, 204, 255));
        
        char text[][10] = {"Resume", "Restart", "Exit"};
        for(int i=0; i<3; i++) {
            al_draw_rounded_rectangle(width/2-250, height-200+150*i, width/2+250, height-100+150*i, 5, 5, hover[i]==0?HOVER_COLOR:WHITE_COLOR, 10);
            al_draw_text(font, hover[i]==0?HOVER_COLOR:WHITE_COLOR, width/2, height-195+150*i, ALLEGRO_ALIGN_CENTER, text[i]);
        }
    }
}
void Menu::hide() {
    visibility = false;
}
void Menu::show() {
    visibility = true;
}
void Menu::toggle() {
    visibility = !visibility;
}