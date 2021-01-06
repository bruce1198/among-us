#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#define WHITE_COLOR al_map_rgb(255, 255, 255)
#define SHADOW_COLOR al_map_rgba(0, 0, 0, 200)
#define HOVER_COLOR al_map_rgb(255, 0, 0)

class Menu {
private:
    /* data */
    bool visibility;
    int hover[3];
    ALLEGRO_FONT *font = NULL;

public:
    Menu();
    ~Menu();
    void draw(int, int);
    void update(int, int, int, int);
    int click(int, int, int, int);
    void hide();
    void show();
    void toggle();
};
