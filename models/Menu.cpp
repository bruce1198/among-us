#include "Menu.h"

Menu::Menu(/* args */) {
    visibility = false;
}

Menu::~Menu() {

}

void Menu::draw(int width, int height) {
    if(visibility)
        al_draw_filled_rounded_rectangle(width/2-200, height-150, width/2+200, height+150, 5, 5, al_map_rgb(255, 0, 0));
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