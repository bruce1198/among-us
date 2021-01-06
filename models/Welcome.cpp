#include "Welcome.h"

Welcome::Welcome() {
    image = al_load_bitmap("assets/images/menu.jpg");
    font = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf", 200,0);
    sfont = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf", 60,0);
    for(int i=0; i<2; i++) hover[i] = -1;
}

Welcome::~Welcome() {
    al_destroy_bitmap(image);
    al_destroy_font(font);
}

int Welcome::click(int width, int height, int mouse_x, int mouse_y) {
    for(int i=0; i<2; i++) {
        if(mouse_x>=width/2-250 && mouse_x<=width/2+250 && mouse_y>=height+100+150*i && mouse_y<=height+200+150*i) {
            return i;
        }
    }
    return -1;
}

void Welcome::update(int width, int height, int mouse_x, int mouse_y) {
    for(int i=0; i<2; i++) {
        if(mouse_x>=width/2-250 && mouse_x<=width/2+250 && mouse_y>=height+100+150*i && mouse_y<=height+200+150*i) {
            hover[i] = 0;
        }
        else hover[i] = -1;
    }
}

void Welcome::draw(int width, int height) {
    al_clear_to_color(WHITE_COLOR);
    float w, h;
    w = al_get_bitmap_width(image);
    h = al_get_bitmap_height(image);
    al_draw_scaled_bitmap(image, 0, 0, w, h, 0, 0, width, h*width/w, 0);

    al_draw_text(font, SHADOW_COLOR, width/2+10, 510, ALLEGRO_ALIGN_CENTER, "Among Us: Survival");
    al_draw_text(font, WHITE_COLOR, width/2, 500, ALLEGRO_ALIGN_CENTER, "Among Us: Survival");
    char text[][10] = {"Play", "Exit"};
    for(int i=0; i<2; i++) {
        al_draw_rounded_rectangle(width/2-250, height+100+150*i, width/2+250, height+200+150*i, 5, 5, hover[i]==0?HOVER_COLOR:WHITE_COLOR, 10);
        al_draw_text(sfont, hover[i]==0?HOVER_COLOR:WHITE_COLOR, width/2, height+105+150*i, ALLEGRO_ALIGN_CENTER, text[i]);
    }
}