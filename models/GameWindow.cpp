#include "GameWindow.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define BACKGROUND al_map_rgb(204, 229, 255)
#define GRAY al_map_rgb(50, 50, 50)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)
#define ENERGY_BK al_map_rgb(255, 204, 204)
#define ENERGY al_map_rgb(255, 0, 0)
#define WATER_BK al_map_rgb(153, 204, 255)
#define WATER al_map_rgb(0, 0, 255)
#define POISON_BK al_map_rgb(204, 153, 255)
#define POISON al_map_rgb(127, 0, 255)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))

using namespace std;

void
GameWindow::init()
{
    std::string buffer;

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    icon = al_load_bitmap("assets/images/icon.png");
    background = al_load_bitmap("assets/images/map.png");

    screen = al_get_target_bitmap();
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    fbo = al_create_bitmap(al_get_bitmap_width(background), al_get_bitmap_height(background));

    al_set_display_icon(display, icon);
    al_reserve_samples(3);

    sample = al_load_sample("assets/audio/Arrow.wav");
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());

    sample = al_load_sample("assets/audio/BackgroundMusic.ogg");
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());
}

bool
GameWindow::mouse_hover(int startx, int starty, int width, int height)
{
    if(mouse_x >= startx && mouse_x <= startx + width)
        if(mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}

void
GameWindow::play()
{
    int msg;

    srand(time(NULL));

    msg = -1;
    reset();
    begin();

    while(msg != GAME_EXIT)
    {
        msg = run();
    }

    show_err_msg(msg);
}

void
GameWindow::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...\n");
    else if(msg == GAME_EXIT)
        fprintf(stderr, "Game Exit...\n");
    else
        fprintf(stderr, "unexpected msg: %d\n", msg);

    destroy();
    exit(9);
}

GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);
    width = info.x2 - info.x1;
    height = info.y2 - info.y1;
    // width = 1920;
    height = height/2;
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    display = al_create_display(width, height);
    event_queue = al_create_event_queue();

    timer = al_create_timer(1.0 / FPS);

    second_timer = al_create_timer(1.0);

    food_timer = al_create_timer(10.0);

    if(second_timer == NULL)
        show_err_msg(-1);

    if(timer == NULL)
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = al_load_ttf_font("assets/fonts/Caviar_Dreams_Bold.ttf",12,0); // load small font
    Medium_font = al_load_ttf_font("assets/fonts/Caviar_Dreams_Bold.ttf",24,0); //load medium font
    Large_font = al_load_ttf_font("assets/fonts/Caviar_Dreams_Bold.ttf",36,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(second_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(food_timer));

    init();
}

void
GameWindow::begin()
{
    draw();

    // al_play_sample_instance(backgroundSound);

    al_start_timer(timer);
    al_start_timer(second_timer);
    al_start_timer(food_timer);
}

int
GameWindow::run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event();
    }
    return error;
}

int
GameWindow::update()
{
    // models' status ex. moving

    crew1.update(width, height);
    crew2.update(width, height);
    return GAME_CONTINUE;
}

void
GameWindow::reset()
{
    crew1 = Crew(0);
    crew2 = Crew(1);
    mute = false;
    redraw = false;

    for(int i=0; i<20; i++) {
        Gredient g = (Gredient)(i%10);
        foods.push_back(new Food(g));
    }

    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(startSound);

    // stop timer
    al_stop_timer(timer);
    al_stop_timer(second_timer);
    al_stop_timer(food_timer);
}

void
GameWindow::destroy()
{
    reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_timer(timer);
    al_destroy_timer(second_timer);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);

    al_destroy_sample(sample);
    al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(backgroundSound);

}

int
GameWindow::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer) {
            redraw = true;
        }
        else if(event.timer.source == second_timer) {
            crew1.time_elapsed();
            crew2.time_elapsed();
        }
        else if(event.timer.source == food_timer) {
            if(foods.size()<20) {
                Gredient g = (Gredient)(rand()%10);
                foods.push_back(new Food(g));
            }
        }
        else {

        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        int min_dist = 400;
        Food* target;
        int x = crew1.getPosition()['x'];
        int y = crew1.getPosition()['y'];
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                crew1.set_direction(UP);
                break;
            case ALLEGRO_KEY_DOWN:
                crew1.set_direction(DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                crew1.set_direction(LEFT);
                break;
            case ALLEGRO_KEY_RIGHT:
                crew1.set_direction(RIGHT);
                break;
            case ALLEGRO_KEY_W:
                crew2.set_direction(UP);
                break;
            case ALLEGRO_KEY_S:
                crew2.set_direction(DOWN);
                break;
            case ALLEGRO_KEY_A:
                crew2.set_direction(LEFT);
                break;
            case ALLEGRO_KEY_D:
                crew2.set_direction(RIGHT);
                break;
            case ALLEGRO_KEY_P:
                if(crew1.ableToPick()) {
                    for(auto& food: foods) {
                        int dist = pow(food->get_x()-x, 2)+pow(food->get_y()-y, 2);
                        if(dist<min_dist) {
                            min_dist = dist;
                            target = food;
                        }
                    }
                    if(min_dist!=400)
                        crew1.pick(target);
                }
                else {
                    crew1.put();
                }
                break;
            case ALLEGRO_KEY_L:
                gmap.load_lines();
                crew1.reload();
                crew2.reload();
                break;
            case ALLEGRO_KEY_M:
                mute = !mute;
                if(mute)
                    al_stop_sample_instance(backgroundSound);
                else
                    al_play_sample_instance(backgroundSound);
                break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_KEY_UP) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                crew1.remove_direction(UP);
                break;
            case ALLEGRO_KEY_DOWN:
                crew1.remove_direction(DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                crew1.remove_direction(LEFT);
                break;
            case ALLEGRO_KEY_RIGHT:
                crew1.remove_direction(RIGHT);
                break;
            case ALLEGRO_KEY_W:
                crew2.remove_direction(UP);
                break;
            case ALLEGRO_KEY_S:
                crew2.remove_direction(DOWN);
                break;
            case ALLEGRO_KEY_A:
                crew2.remove_direction(LEFT);
                break;
            case ALLEGRO_KEY_D:
                crew2.remove_direction(RIGHT);
                break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if(event.mouse.button == 1) {

        }
        else if(event.mouse.button == 2) {

        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;

    }

    if(redraw) {
        // update each object in game
        instruction = update();

        // Re-draw map
        draw();
        redraw = false;
    }

    return instruction;
}

void
GameWindow::draw()
{
    screen = al_get_target_bitmap();
    // draw to fbo
    
    al_set_target_bitmap(fbo);
    al_draw_bitmap(background, 0, 0, 0);
    for(auto line: gmap.tiles) {
        al_draw_line(line.x1, line.y1, line.x2, line.y2, BLUE, 2);
    }
    float scale = 3;
    for(auto food: foods) {
        food->draw(width, height, scale);
    }
    crew1.draw(width, height, scale);
    crew2.draw(width, height, scale);

    float scale_factor = scale*height/1080;
    // draw to screen
    al_set_target_bitmap(screen);
    al_clear_to_color(BACKGROUND);
    /*===============================crew1's view=====================================*/
    // draw crew1 and shadow
    al_draw_scaled_bitmap(fbo, crew1.getPosition()['x']-width/(4*scale_factor), crew1.getPosition()['y']-height/(2*scale_factor), width/(2*scale_factor), height/scale_factor, 0, height/2, width/2, height, 0);
    al_draw_scaled_bitmap(crew1.getShadow(), crew1.getPosition()['x']-width/(4*scale_factor), crew1.getPosition()['y']-height/(2*scale_factor), width/(2*scale_factor), height/scale_factor, 0, height/2, width/2, height, 0);
    // draw status
    al_draw_text(Large_font, BLACK, width/8, 3*height/2+50, ALLEGRO_ALIGN_RIGHT, "ENERGY: ");
    al_draw_text(Large_font, BLACK, width/8, 3*height/2+120, ALLEGRO_ALIGN_RIGHT, "WATER CONTENT: ");
    al_draw_text(Large_font, BLACK, width/8, 3*height/2+190, ALLEGRO_ALIGN_RIGHT, "INTOXICOTION: ");
    al_draw_filled_rectangle(width/8, 3*height/2+50, width/8+width/3, 3*height/2+100, ENERGY_BK);
    al_draw_filled_rectangle(width/8, 3*height/2+50, width/8+crew1.get_energy()*width/300, 3*height/2+100, ENERGY);
    al_draw_filled_rectangle(width/8, 3*height/2+120, width/8+width/3, 3*height/2+170, WATER_BK);
    al_draw_filled_rectangle(width/8, 3*height/2+120, width/8+crew1.get_water()*width/300, 3*height/2+170, WATER);
    // al_draw_filled_rectangle(width/8, 3*height/2+190, width/8+width/3, 3*height/2+240, POISON_BK);
    al_draw_filled_rectangle(width/8, 3*height/2+190, width/8+crew1.get_poison()*width/300, 3*height/2+240, POISON);
    /*===============================crew2's view=====================================*/
    al_draw_scaled_bitmap(fbo, crew2.getPosition()['x']-width/(4*scale_factor), crew2.getPosition()['y']-height/(2*scale_factor), width/(2*scale_factor), height/scale_factor, width/2, height/2, width/2, height, 0);
    al_draw_scaled_bitmap(crew2.getShadow(), crew2.getPosition()['x']-width/(4*scale_factor), crew2.getPosition()['y']-height/(2*scale_factor), width/(2*scale_factor), height/scale_factor, width/2, height/2, width/2, height, 0);
    // draw split line
    al_draw_filled_rectangle(width/2-5, 0, width/2+5, 2*height, BLACK);

    al_flip_display();

}
