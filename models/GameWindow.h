#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <fstream>
#include <ctime>
#include <vector>
#include <map>
#include <list>
#include <set>
#include "Welcome.h"
#include "Food.h"
#include "Pot.h"
#include "Crew.h"
#include "Fire.h"
#include "Menu.h"

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7

using namespace std;

enum Status { WELCOME, SETTING, GUIDE, GAME };

// clock rate
const float FPS = 60;

class GameWindow {
public:
    // constructor
    GameWindow();
    // each process of scene
    void init();
    void reset();
    void play();
    void play_game();
    void go_welcome();
    void begin();

    int run();
    int update();

    void show_err_msg(int msg);
    void destroy();

    void draw();

    // process of updated event
    int process_event();
    // detect if mouse hovers over a rectangle
    bool mouse_hover(int, int, int, int);

public:
    bool initial = true;

private:

    int width;
    int height;
    int fbo_scale = 2;

    Status status;

    ALLEGRO_BITMAP *icon = NULL;
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *screen = NULL;
    ALLEGRO_BITMAP *fbo = NULL;
    map<int, ALLEGRO_BITMAP*> food_images;
    map<int, ALLEGRO_BITMAP*> pot_images;

    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *Medium_font = NULL;
    ALLEGRO_FONT *Large_font = NULL;
    ALLEGRO_FONT *Huge_font = NULL;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *second_timer = NULL;
    ALLEGRO_TIMER *food_timer = NULL;

    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE *startSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *clearSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *failSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *welcomeSound = NULL;

    Welcome* welcome = NULL;
    Menu* menu = NULL;

    Crew* crew1 = NULL;
    Crew* crew2 = NULL;
    
    Map* gmap = NULL;
    vector<Fire*> fires;

    int food_pk = 0;
    list<Food*> foods;
    list<Pot*> pots;

    int mouse_x, mouse_y;

    bool redraw = false;
    bool mute = false;
};


#endif // MAINWINDOW_H_INCLUDED
