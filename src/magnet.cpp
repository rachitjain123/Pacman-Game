#include "magnet.h"
#include "main.h"
#include <stdlib.h>

Magnet::Magnet(float x, float y, color_t color,float size)
{
    this->rotation = 0;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    // speedx = -0.01+(rand()/(float) RAND_MAX)*0.02;
    speedx = 0.0;
    speedy = 0.0;
    accx   = 0.0;
    accy   = 0.0;

    float r = size;

    int n = 10000;
    // float r = 0.5;

    float theta = 3.14159/n;

    GLfloat vertex_buffer_data[180000];

    int i,j;
    for(i=0; i<n; i++)
    {
        float ab=theta*i;
        if(x<0)
            ab+=1.57;
        else
            ab-=1.57;
        float angle=ab+theta;

        float x=r*cos(angle);
        float y=r*sin(angle);
        float z=0.0f;

        float x1=r*cos(ab);
        float y1=r*sin(ab);
        float z1=0.0f;

        float x2=0.0f;
        for(j=0;j<3;j++)
            vertex_buffer_data[i*9+j]=x2;
        vertex_buffer_data[i*9+3]=x1;
        vertex_buffer_data[i*9+4]=y1;
        vertex_buffer_data[i*9+5]=z1;
        vertex_buffer_data[i*9+6]=x;
        vertex_buffer_data[i*9+7]=y;
        vertex_buffer_data[i*9+8]=z;

    }
    r=0.1;
    for(i=n; i<2*n; i++)
    {
        float ab=theta*i;
        if(x<0)
            ab+=1.57;
        else
            ab-=1.57;
        float angle=ab+theta;

        float x=r*cos(angle);
        float y=r*sin(angle);
        float z=0.0f;

        float x1=r*cos(ab);
        float y1=r*sin(ab);
        float z1=0.0f;

        float x2=0.0f;
        for(j=0;j<3;j++)
            vertex_buffer_data[i*9+j]=x2;
        vertex_buffer_data[i*9+3]=x1;
        vertex_buffer_data[i*9+4]=y1;
        vertex_buffer_data[i*9+5]=z1;
        vertex_buffer_data[i*9+6]=x;
        vertex_buffer_data[i*9+7]=y;
        vertex_buffer_data[i*9+8]=z;

    }
    this->object = create3DObject(GL_TRIANGLES, 60000, vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
void Magnet::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}
bounding_box_t Magnet::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
