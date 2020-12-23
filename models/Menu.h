#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>

class Menu {
private:
    /* data */
    bool visibility;

public:
    Menu(/* args */);
    ~Menu();
    void draw(int, int);
    void hide();
    void show();
    void toggle();
};
