#include "Welcome.h"

Welcome::Welcome() {
    image = al_load_bitmap("assets/images/menu.jpg");
    font = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf", 200,0);
    hover = -1;
}

Welcome::~Welcome() {
    al_destroy_bitmap(image);
    al_destroy_font(font);
}

int Welcome::click(int width, int height, int mouse_x, int mouse_y) {
    if(mouse_x>=width/2-250 && mouse_x<=width/2+250 && mouse_y>=height+100 && mouse_y<=height+200) {
        return 0;
    }
    else return -1;
}

void Welcome::update(int width, int height, int mouse_x, int mouse_y) {
    if(mouse_x>=width/2-250 && mouse_x<=width/2+250 && mouse_y>=height+100 && mouse_y<=height+200) {
        hover = 0;
    }
    else hover = -1;
}

void Welcome::draw(int width, int height) {
    al_clear_to_color(WHITE_COLOR);
    float w, h;
    w = al_get_bitmap_width(image);
    h = al_get_bitmap_height(image);
    al_draw_scaled_bitmap(image, 0, 0, w, h, (width-2*height/h*w)/2, 0, 2*height/h*w, 2*height, 0);

    al_draw_text(font, SHADOW_COLOR, width/2+10, 510, ALLEGRO_ALIGN_CENTER, "Among Us: Survival");
    al_draw_text(font, WHITE_COLOR, width/2, 500, ALLEGRO_ALIGN_CENTER, "Among Us: Survival");
    al_draw_rounded_rectangle(width/2-250, height+100, width/2+250, height+200, 5, 5, hover==0?HOVER_COLOR:WHITE_COLOR, 10);
}