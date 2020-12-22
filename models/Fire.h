#include "Object.h"

class Fire: public Object {
private:
    ALLEGRO_BITMAP* image;
public:
    Fire();
    Fire(int, int);
    ~Fire();
    void draw(int, int, int);
};
