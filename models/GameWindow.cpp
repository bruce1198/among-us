#include "GameWindow.h"
#include <iostream>
#include <cmath>

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
    menu = new Menu();
    welcome = new Welcome();
    gmap = new Map();
    fstream config("config/fire.txt", ios_base::in);
    int x, y;
    while(config >> x >> y) {
        fires.push_back(new Fire(x, y));
    }


    std::string buffer;

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    icon = al_load_bitmap("assets/images/icon_small.png");
    background = al_load_bitmap("assets/images/map.png");
    vector<string> tmp = {"lobster", "lu", "egg", "rice", "tson", "meat", "broco", "carrot", "mushroom", "river", "water", "fried_rice", "lobster_cooked", "steak", "poop"};
    for(int i=0; i<tmp.size(); i++) {
        string buf = "assets/images/food/"+tmp[i]+".png";
        food_images[i] = al_load_bitmap(buf.c_str());
    }
    vector<string> tmp2 = {"flat", "normal", "clean"};
    for(int i=0; i<tmp2.size(); i++) {
        string buf = "assets/images/food/"+tmp2[i]+".png";
        pot_images[i] = al_load_bitmap(buf.c_str());
    }

    screen = al_get_target_bitmap();
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_NO_PRESERVE_TEXTURE);
    fbo = al_create_bitmap(fbo_scale*al_get_bitmap_width(background), fbo_scale*al_get_bitmap_height(background));

    al_set_display_icon(display, icon);
    al_reserve_samples(3);

    sample = al_load_sample("assets/audio/Arrow.wav");
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());

    sample = al_load_sample("assets/audio/BackgroundMusic.wav");
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());

    sample = al_load_sample("assets/audio/welcomeMusic.wav");
    welcomeSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(welcomeSound, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(welcomeSound, al_get_default_mixer());
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
    go_welcome();
    

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
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_RESIZABLE);
    display = al_create_display(width, 2*height);
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

    font = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf",12,0); // load small font
    Medium_font = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf",24,0); //load medium font
    Large_font = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf",36,0); //load large font
    Huge_font = al_load_ttf_font("assets/fonts/open-sans/OpenSans-Bold.ttf",72*height/900.0,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(second_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(food_timer));

    status = WELCOME;
    init();
}

void
GameWindow::go_welcome() {
    reset();
    begin();
    status = WELCOME;
}

void
GameWindow::play_game() {
    status = GAME;
    al_stop_sample_instance(welcomeSound);
    al_play_sample_instance(backgroundSound);
    al_start_timer(second_timer);
    al_start_timer(food_timer);
}

void
GameWindow::begin()
{
    draw();

    al_play_sample_instance(welcomeSound);

    al_start_timer(timer);
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
    // cout << "update" << endl;
    // welcome page
    if(status==WELCOME) 
        welcome->update(width, height, mouse_x, mouse_y);

    // models' status ex. moving
    else if(status==GAME) {
        bool dead1 = crew1->update(width, height);
        bool dead2 = crew2->update(width, height);
        if(dead1 || dead2) {
            go_welcome();
        }
        for(auto pot: pots) {
            vector<Food*> shouldRemove = pot->update();
            for(auto food: shouldRemove) {
                int fid = food->get_id();
                if(fid==-1) { //new food
                    food->set_id(food_pk);
                    food_pk++;
                    foods.push_back(food);
                }
                else { //old food
                    for(auto it=foods.begin(); it!=foods.end(); ) {
                        if((*it)->get_id()==fid) {
                            it = foods.erase(it);
                        }
                        else {
                            it++;
                        }
                    }
                }
            }
        }
    }
    return GAME_CONTINUE;
}

void
GameWindow::reset()
{
    int c1 = rand()%14;
    int c2;
    while((c2=rand()%14)==c1);
    if(crew1)
        delete crew1;
    if(crew2)
        delete crew2;
    crew1 = new Crew(0, (Color)c1);
    crew2 = new Crew(1, (Color)c2);
    mute = false;
    redraw = false;

    for(auto food: foods) {
        if(food)
            delete food;
    }
    for(auto pot: pots) {
        if(pot)
            delete pot;
    }
    foods.clear();
    pots.clear();
    food_pk = 0;
    for(int i=0; i<20; i++) {
        Gredient g = (Gredient)(i%10);
        foods.push_back(new Food(g, food_pk));
        food_pk++;
    }

    pots.push_back(new Pot(FLAT));
    pots.push_back(new Pot(FLAT));
    pots.push_back(new Pot(NORMAL));
    pots.push_back(new Pot(NORMAL));
    pots.push_back(new Pot(CLEAN));

    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(welcomeSound);
    al_stop_sample_instance(startSound);

    // stop timer
    al_stop_timer(timer);
    al_stop_timer(second_timer);
    al_stop_timer(food_timer);
}

void
GameWindow::destroy()
{
    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(welcomeSound);
    al_stop_sample_instance(startSound);

    // stop timer
    al_stop_timer(timer);
    al_stop_timer(second_timer);
    al_stop_timer(food_timer);

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);
    al_destroy_font(Huge_font);

    al_destroy_timer(timer);
    al_destroy_timer(second_timer);
    al_destroy_timer(food_timer);

    for(auto food: foods) {
        if(food)
            delete food;
    }
    for(auto pot: pots) {
        if(pot)
            delete pot;
    }
    for(auto fire: fires)
        delete fire;

    for(auto image: food_images)
        al_destroy_bitmap(image.second);
    for(auto image: pot_images)
        al_destroy_bitmap(image.second);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);
    // al_destroy_bitmap(screen);
    // cout << "hihi screen" << endl;
    al_destroy_bitmap(fbo);

    al_destroy_sample(sample);
    al_destroy_sample_instance(welcomeSound);
    al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(backgroundSound);

    delete crew1;
    delete crew2;
    delete gmap;
    delete menu;
    delete welcome;

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
            crew1->time_elapsed();
            crew2->time_elapsed();
            for(auto pot: pots) pot->time_ellapsed();
        }
        else if(event.timer.source == food_timer) {
            if(foods.size()<20) {
                Gredient g = (Gredient)(rand()%10);
                foods.push_back(new Food(g, food_pk));
                food_pk++;
            }
        }
        else {

        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
        al_acknowledge_resize(display);
        width = event.display.width;
        height = event.display.height/2;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        float min_dist = 400;
        Object* target;
        float x1 = (*crew1)['x'];
        float y1 = (*crew1)['y'];
        float x2 = (*crew2)['x'];
        float y2 = (*crew2)['y'];
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                menu->toggle();
                break;
            case ALLEGRO_KEY_C:
                reset();
                begin();
                play_game();
                break;
            case ALLEGRO_KEY_Q:
                return GAME_EXIT;
            case ALLEGRO_KEY_UP:
                crew1->set_direction(UP);
                break;
            case ALLEGRO_KEY_DOWN:
                crew1->set_direction(DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                crew1->set_direction(LEFT);
                break;
            case ALLEGRO_KEY_RIGHT:
                crew1->set_direction(RIGHT);
                break;
            case ALLEGRO_KEY_W:
                crew2->set_direction(UP);
                break;
            case ALLEGRO_KEY_S:
                crew2->set_direction(DOWN);
                break;
            case ALLEGRO_KEY_A:
                crew2->set_direction(LEFT);
                break;
            case ALLEGRO_KEY_D:
                crew2->set_direction(RIGHT);
                break;
            case ALLEGRO_KEY_R:
                if(crew2->ableToPick()) {
                    for(auto& food: foods) {
                        int dist = pow(food->get_x()-(*crew2)['x'], 2)+pow(food->get_y()-(*crew2)['y'], 2);
                        if(dist<min_dist && !food->is_in_pot()) {
                            min_dist = dist;
                            target = food;
                        }
                    }
                    for(auto& pot: pots) {
                        int dist = pow(pot->get_x()-(*crew2)['x'], 2)+pow(pot->get_y()-(*crew2)['y'], 2);
                        if(dist<min_dist) {
                            min_dist = dist;
                            target = pot;
                        }
                    }
                    if(min_dist!=400) {
                        if(target->get_type()==POT) {
                            ((Pot*)target)->set_ready(false);
                        }
                        crew2->pick(target);
                    }
                }
                else {
                    Object* obj = crew2->put();
                    if(obj) {
                        if(obj->get_type()==FOOD) {
                            Pot* target_pot;
                            float pot_min_dist = 400;
                            for(auto& pot: pots) {
                                int dist = pow(pot->get_x()-obj->get_x(), 2)+pow(pot->get_y()-obj->get_y(), 2);
                                if(dist<pot_min_dist) {
                                    pot_min_dist = dist;
                                    target_pot = pot;
                                }
                            }
                            if(pot_min_dist!=400) {
                                target_pot->insert((Food*&)obj);
                            }
                        }
                        if(obj->get_type()==POT) {
                            Fire* target_fire;
                            float fire_min_dist = 400;
                            for(auto fire: fires) {
                                int dist = pow(fire->get_x()-obj->get_x(), 2)+pow(fire->get_y()-obj->get_y(), 2);
                                if(dist<fire_min_dist) {
                                    fire_min_dist = dist;
                                    target_fire = fire;
                                }
                            }
                            if(fire_min_dist!=400) {
                                Pot*& tmp = (Pot*&)obj;
                                tmp->set_pos(target_fire->get_x(), target_fire->get_y()-20);
                                tmp->set_ready(true);
                            }
                            else {
                                Pot*& tmp = (Pot*&)obj;
                                tmp->set_ready(false);
                            }
                        }
                    }
                }
                break;
            case ALLEGRO_KEY_E:
                {
                    int fid = crew2->eat();
                    for(auto it=foods.begin(); it!=foods.end(); ) {
                        if((*it)->get_id()==fid) {
                            it = foods.erase(it);
                        }
                        else {
                            it++;
                        }
                    }
                }
                break;
            case ALLEGRO_KEY_P:
                if(status == WELCOME) {
                    play_game();
                    break;
                }
                if(crew1->ableToPick()) {
                    for(auto& food: foods) {
                        int dist = pow(food->get_x()-x1, 2)+pow(food->get_y()-y1, 2);
                        if(dist<min_dist && !food->is_in_pot()) {
                            min_dist = dist;
                            target = food;
                        }
                    }
                    for(auto& pot: pots) {
                        int dist = pow(pot->get_x()-x1, 2)+pow(pot->get_y()-y1, 2);
                        if(dist<min_dist) {
                            min_dist = dist;
                            target = pot;
                        }
                    }
                    if(min_dist!=400) {
                        if(target->get_type()==POT) {
                            ((Pot*)target)->set_ready(false);
                        }
                        crew1->pick(target);
                    }
                }
                else {
                    Object* obj = crew1->put();
                    if(obj) {
                        if(obj->get_type()==FOOD) {
                            Pot* target_pot;
                            float pot_min_dist = 400;
                            for(auto& pot: pots) {
                                int dist = pow(pot->get_x()-obj->get_x(), 2)+pow(pot->get_y()-obj->get_y(), 2);
                                if(dist<pot_min_dist) {
                                    pot_min_dist = dist;
                                    target_pot = pot;
                                }
                            }
                            if(pot_min_dist!=400) {
                                target_pot->insert((Food*&)obj);
                            }
                        }
                        if(obj->get_type()==POT) {
                            Fire* target_fire;
                            float fire_min_dist = 400;
                            for(auto fire: fires) {
                                int dist = pow(fire->get_x()-obj->get_x(), 2)+pow(fire->get_y()-obj->get_y(), 2);
                                if(dist<fire_min_dist) {
                                    fire_min_dist = dist;
                                    target_fire = fire;
                                }
                            }
                            if(fire_min_dist!=400) {
                                Pot*& tmp = (Pot*&)obj;
                                tmp->set_pos(target_fire->get_x(), target_fire->get_y()-20);
                                tmp->set_ready(true);
                            }
                            else {
                                Pot*& tmp = (Pot*&)obj;
                                tmp->set_ready(false);
                            }
                        }
                    }
                }
                break;
            case ALLEGRO_KEY_O:
                {
                    int fid = crew1->eat();
                    for(auto it=foods.begin(); it!=foods.end(); ) {
                        if((*it)->get_id()==fid) {
                            it = foods.erase(it);
                        }
                        else {
                            it++;
                        }
                    }
                }
                break;
            case ALLEGRO_KEY_L:
                gmap->load_lines();
                crew1->reload();
                crew2->reload();
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
                crew1->remove_direction(UP);
                break;
            case ALLEGRO_KEY_DOWN:
                crew1->remove_direction(DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                crew1->remove_direction(LEFT);
                break;
            case ALLEGRO_KEY_RIGHT:
                crew1->remove_direction(RIGHT);
                break;
            case ALLEGRO_KEY_W:
                crew2->remove_direction(UP);
                break;
            case ALLEGRO_KEY_S:
                crew2->remove_direction(DOWN);
                break;
            case ALLEGRO_KEY_A:
                crew2->remove_direction(LEFT);
                break;
            case ALLEGRO_KEY_D:
                crew2->remove_direction(RIGHT);
                break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if(event.mouse.button == 1) {

        }
        else if(event.mouse.button == 2) {

        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if(event.mouse.button == 1) {
            if(status==WELCOME) {
                int r = welcome->click(width, height, event.mouse.x, event.mouse.y);
                if(r==0) play_game();
            }
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
    if(status == WELCOME) {
        welcome->draw(width, height);
    }
    else if(status == GAME) {
        // draw to fbo
        al_set_target_bitmap(fbo);
        al_clear_to_color(BLACK);

        ALLEGRO_TRANSFORM trans, prev_trans;
        al_copy_transform(&prev_trans, al_get_current_transform());
        al_identity_transform(&trans);
        al_scale_transform(&trans, fbo_scale, fbo_scale);
        al_use_transform(&trans);
        al_hold_bitmap_drawing(true);
        al_draw_bitmap(background, 0, 0, 0);
        for(auto line: gmap->tiles) {
            al_draw_line(line.x1, line.y1, line.x2, line.y2, BLUE, 2);
        }
        al_hold_bitmap_drawing(false);
        float scale = 3;
        crew1->draw(width, height, scale);
        crew2->draw(width, height, scale);
        for(auto fire: fires) {
            fire->draw(width, height, scale);
        }
        for(auto food: foods) {
            food->draw(width, height, scale);
        }
        for(auto pot: pots) {
            pot->draw(width, height, scale);
        }
        al_use_transform(&prev_trans);


        float scale_factor = scale*height/1080;
        // draw to screen
        al_set_target_bitmap(screen);
        al_clear_to_color(BACKGROUND);
        al_draw_filled_rectangle(0, height/2, width, 3*height/2, BLACK);
        /*===============================crew1's view=====================================*/
        // draw crew1 and shadow
        al_draw_scaled_bitmap(fbo, fbo_scale*((*crew1)['x']-width/(4*scale_factor)), fbo_scale*((*crew1)['y']-height/(2*scale_factor)), fbo_scale*width/(2*scale_factor), fbo_scale*height/scale_factor, width/2, height/2, width/2, height, 0);
        al_draw_scaled_bitmap(crew1->getShadow(), (*crew1)['x']-width/(4*scale_factor), (*crew1)['y']-height/(2*scale_factor), width/(2*scale_factor), height/scale_factor, width/2, height/2, width/2, height, 0);
        /*===============================crew2's view=====================================*/
        al_draw_scaled_bitmap(fbo, fbo_scale*((*crew2)['x']-width/(4*scale_factor)), fbo_scale*((*crew2)['y']-height/(2*scale_factor)), fbo_scale*width/(2*scale_factor), fbo_scale*height/scale_factor, 0, height/2, width/2, height, 0);
        al_draw_scaled_bitmap(crew2->getShadow(), (*crew2)['x']-width/(4*scale_factor), (*crew2)['y']-height/(2*scale_factor), width/(2*scale_factor), height/scale_factor, 0, height/2, width/2, height, 0);
        /*=============================draw split line====================================*/
        al_draw_filled_rectangle(width/2-5, height/2, width/2+5, 3*height/2, al_map_rgb(255, 0, 0));
        /*===============================draw status======================================*/
        al_copy_transform(&prev_trans, al_get_current_transform());
        al_identity_transform(&trans);
        al_scale_transform(&trans, height/900.0, height/900.0);
        al_use_transform(&trans);
        // crew1
        al_draw_text(Large_font, BLACK, 1800, 3*height/2+50, ALLEGRO_ALIGN_RIGHT, "ENERGY: ");
        al_draw_text(Large_font, BLACK, 1800, 3*height/2+120, ALLEGRO_ALIGN_RIGHT, "WATER CONTENT: ");
        al_draw_text(Large_font, BLACK, 1800, 3*height/2+190, ALLEGRO_ALIGN_RIGHT, "INTOXICOTION: ");
        al_draw_filled_rounded_rectangle(1800, 3*height/2+50, 1800+width/3, 3*height/2+100, 5, 5, ENERGY_BK);
        if(crew1->get_energy()!=0) al_draw_filled_rounded_rectangle(1800, 3*height/2+50, 1800+crew1->get_energy()*width/300, 3*height/2+100, 5, 5, ENERGY);
        al_draw_filled_rounded_rectangle(1800, 3*height/2+120, 1800+width/3, 3*height/2+170, 5, 5, WATER_BK);
        if(crew1->get_water()!=0) al_draw_filled_rounded_rectangle(1800, 3*height/2+120, 1800+crew1->get_water()*width/300, 3*height/2+170, 5, 5, WATER);
        if(crew1->get_poison()!=0) al_draw_filled_rounded_rectangle(1800, 3*height/2+190, 1800+crew1->get_poison()*width/300, 3*height/2+240, 5, 5, POISON);
        // crew2
        al_draw_text(Large_font, BLACK, 360, 3*height/2+50, ALLEGRO_ALIGN_RIGHT, "ENERGY: ");
        al_draw_text(Large_font, BLACK, 360, 3*height/2+120, ALLEGRO_ALIGN_RIGHT, "WATER CONTENT: ");
        al_draw_text(Large_font, BLACK, 360, 3*height/2+190, ALLEGRO_ALIGN_RIGHT, "INTOXICOTION: ");
        al_draw_filled_rounded_rectangle(360, 3*height/2+50, width/8+width/3, 3*height/2+100, 5, 5, ENERGY_BK);
        if(crew2->get_energy()!=0) al_draw_filled_rounded_rectangle(width/8, 3*height/2+50, 360+crew2->get_energy()*width/300, 3*height/2+100, 5, 5, ENERGY);
        al_draw_filled_rounded_rectangle(360, 3*height/2+120, width/8+width/3, 3*height/2+170, 5, 5, WATER_BK);
        if(crew2->get_water()!=0) al_draw_filled_rounded_rectangle(360, 3*height/2+120, 360+crew2->get_water()*width/300, 3*height/2+170, 5, 5, WATER);
        if(crew2->get_poison()!=0) al_draw_filled_rounded_rectangle(360, 3*height/2+190, 360+crew2->get_poison()*width/300, 3*height/2+240, 5, 5, POISON);
        // pot
        int i=0;
        al_draw_rounded_rectangle(20, 20, 900, 450 - 20, 10, 10, BLACK, 10);
        for(auto pot: pots) {
            al_draw_scaled_bitmap(pot->get_image(), 0, 0, 100, 100, 100+150*i, 200-50, 100, 100, 0);
            if(pot->get_status()==1 && pot->is_ready()) { // cooking
                al_draw_text(Large_font, BLACK, 150+150*i, 250, ALLEGRO_ALIGN_CENTER, to_string(pot->get_remain_time()).c_str());
            }
            else {
                al_draw_text(Large_font, BLACK, 150+150*i, 250, ALLEGRO_ALIGN_CENTER, "IDLE");
            }
            i++;
        }
        // food
        map<int, int> counts;
        for(auto food: foods) {
            counts[food->get_food_type()]++;
        }
        i=0;
        al_draw_rounded_rectangle(950, 20, 2100, 450 - 20, 10, 10, BLACK, 10);
        for(auto g: counts) {
            int h;
            if(i<5) h = 50;
            else if(i<10) h = 170;
            else h = 290;
            al_draw_scaled_bitmap(food_images[g.first], 0, 0, 100, 100, 1000+220*(i%5), h, 100, 100, 0);
            al_draw_text(Large_font, BLACK, 1100+220*(i%5), h+30, ALLEGRO_ALIGN_LEFT, ("*"+to_string(g.second)).c_str());
            i++;
        }
        //recipe
        //lobster
        al_draw_scaled_bitmap(food_images[0], 0, 0, 100, 100, 2250, 50, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[1], 0, 0, 100, 100, 2350, 50, 100, 100, 0);
        al_draw_scaled_bitmap(pot_images[1], 0, 0, 100, 100, 2450, 50, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[12], 0, 0, 100, 100, 2700, 50, 100, 100, 0);
        //fried rice
        al_draw_scaled_bitmap(food_images[2], 0, 0, 100, 100, 2200, 170, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[3], 0, 0, 100, 100, 2300, 170, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[4], 0, 0, 100, 100, 2400, 170, 100, 100, 0);
        al_draw_scaled_bitmap(pot_images[0], 0, 0, 100, 100, 2500, 170, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[11], 0, 0, 100, 100, 2700, 170, 100, 100, 0);
        //steak
        al_draw_scaled_bitmap(food_images[5], 0, 0, 100, 100, 2150, 290, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[6], 0, 0, 100, 100, 2250, 290, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[7], 0, 0, 100, 100, 2350, 290, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[8], 0, 0, 100, 100, 2450, 290, 100, 100, 0);
        al_draw_scaled_bitmap(pot_images[0], 0, 0, 100, 100, 2550, 290, 100, 100, 0);
        al_draw_scaled_bitmap(food_images[13], 0, 0, 100, 100, 2700, 290, 100, 100, 0);

        menu->draw(width, height);
        al_use_transform(&prev_trans);
    }

    al_flip_display();
}
