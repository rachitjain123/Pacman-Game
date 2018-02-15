#include "trampoline.h"
#include "main.h"
#include <stdlib.h>

Trampoline::Trampoline(float x, float y, color_t color,float size)
{
    // this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    // speedx = -0.01+(rand()/(float) RAND_MAX)*0.02;
    speedx = 0.0;
    speedy = 0.0;
    accx = 0.0 ;
    accy = 0.0;

    float r = size;
    
    float n = 10000;
    // float r = 0.5;
    GLfloat vertex_buffer_data[] = {
        3.0f, -1.0f, 0.0f,
        3.1f, -1.0f, 0.0f,
        3.1f, -0.5f, 0.0f,

        3.0f, -1.0f, 0.0f,
        3.0f, -0.5f, 0.0f,
        3.1f, -0.5f, 0.0f,

        2.2f, -1.0f, 0.0f,
        2.3f, -1.0f, 0.0f,
        2.3f, -0.5f, 0.0f,

        2.2f, -1.0f, 0.0f,
        2.2f, -0.5f, 0.0f,
        2.3f, -0.5f, 0.0f,

        2.2f, -0.5f, 0.0f,
        3.1f, -0.5f, 0.0f,
        3.1f, -0.4f, 0.0f,

        2.2f, -0.5f, 0.0f,
        2.2f, -0.4f, 0.0f,
        3.1f, -0.4f, 0.0f,

        2.2f, -0.4f, 0.0f,
        3.1f, -0.4f, 0.0f,
        2.65f, -0.8f, 0.0f,

    };
    this->object = create3DObject(GL_TRIANGLES, 21, vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_t Trampoline::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
