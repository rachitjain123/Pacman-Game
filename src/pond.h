#include "main.h"

#ifndef POND_H
#define POND_H


class Pond {
public:
    Pond() {}
    Pond(float x, float y, color_t color,float size);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedx,speedy;
    double accx, accy;
    void change_speed();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // POND_H
