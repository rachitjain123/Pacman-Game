#include "ball.h"
#include "main.h"
#include <stdlib.h>

Ball::Ball(float x, float y, color_t color,float size)
{
	this->position = glm::vec3(x, y, 0);
	this->rotation = 0;

	speedx = -0.01+(rand()/(float) RAND_MAX)*0.03;
    // speedx = 0.01;
	speedy = 0.0;
	accx = 0.0 ;
	accy = 0.0;

	float r = size;

	float n = 10000;
    // float r = 0.5;

	float theta = 2*3.14159/n;

	GLfloat vertex_buffer_data[90000];

	int i,j;
	for(i=0; i<n; i++)
	{
		float ab=theta*i;
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

	this->object = create3DObject(GL_TRIANGLES, 30000, vertex_buffer_data, color, GL_FILL);
}

void Ball::change_speed()
{
	if(this->position.x>=-1 && this->position.x<=1 && this->position.y<=-1 && this->position.y>=-2)
	{
		if(abs(this->position.x)<=0.03)
			accx=-speedx;
	}
	speedx  += accx ;
	speedy  += accy ;
}

void Ball::draw(glm::mat4 VP)
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

void Ball::set_position(float x, float y)
{
	this->position = glm::vec3(x, y, 0);
}

void Ball::tick() 
{
	change_speed();
	if(this->position.x>=-1 && this->position.x<=1 && this->position.y<=-0.8 && this->position.y>=-2)
	{
		this->position.x += speedx;
		float newx=this->position.x;
		float oldy=this->position.y;
		float newyy=1-newx*newx;
		float newy;
		if(newyy<=0)
			newy=-1.2;
		else
			newy=-sqrt(newyy)-1;
		newy=newy+0.2;
		if(newy>-1)
			newy=-1;
		// std::cout<<speedy<<"\n";
		if(speedy>0)
			this->position.y+=speedy;
		else if((speedy <0 && this->position.y>newy)||(speedy==0))
			this->position.y=newy;
		else if(this->position.y<=newy)
		{
			accy=0;
			this->position.y=newy;
		}
		// this->position.y=speedy+newy;
	}
	else
	{
		this->position.x += speedx;
		this->position.y += speedy;
	}
}

bounding_box_t Ball::bounding_box()
{
	float x = this->position.x, y = this->position.y;
	bounding_box_t bbox = { x, y, 0.4, 0.4 };
	return bbox;
}
