#include "main.h"
#include "timer.h"
#include "ball.h"
#include "pond.h"
#include "ground.h"
#include "magnet.h"
#include "trampoline.h"
#include "kaatein.h"
#include "ground2.h"
#include "plank.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;
GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;


/**************************
* Customizable functions *
**************************/


class timer {
private:
	unsigned long begTime;
public:
	void start() {
		begTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long) clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}
};
float sz[15];
Ball ball[12];
Ball mainball;
Pond pond;
Plank plank[2];
Ground ground;
Ground2 ground2;
Trampoline trampoline;
Kaatein kaatein[4];
Magnet magnet;
timer tt;

int score;
int lives;
int level;
int numballs;

extern bool cannon_keyboard_input, drag_pan;
extern double drag_oldx, drag_oldy;
int flag[15]={0};
int fl;
int t11,t22;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() 
{
    // clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	reset_screen();
    // use the loaded shader program
    // Don't change unless you know what you are doing
	glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!

    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ground.draw(VP);
    ground2.draw(VP);
    pond.draw(VP);
    trampoline.draw(VP);
    mainball.draw(VP);
    for(int i=0;i<numballs;i++)
    	ball[i].draw(VP);
    for(int i=0;i<2;i++)
    	plank[i].draw(VP);
    magnet.draw(VP);
    for(int i=0;i<4;i++)
    	kaatein[i].draw(VP);
}

void tick_input(GLFWwindow *window)
{
	int l1  = glfwGetKey(window, GLFW_KEY_A);
	int r1 = glfwGetKey(window, GLFW_KEY_D);
	int up = glfwGetKey(window, GLFW_KEY_UP);

	int left = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);

	if(left) 
	{
		if(mainball.position.x>=-1 && mainball.position.x<=1 && mainball.position.y<=-0.8 && mainball.position.y>=-2)
		{
			mainball.accx-=0.0002;
			mainball.set_position(mainball.position.x-0.01,mainball.position.y);

		}
		else if(mainball.position.x>=2.2 && mainball.position.x<=3.1 && mainball.position.y>=-1 && mainball.position.y<=-0.5)
		{

		}
		else
		{
			if(mainball.position.x>=-3.9)
				mainball.set_position(mainball.position.x-0.1,mainball.position.y);
		}
	}
	
	else if(right)
	{
		if(mainball.position.x>=-1 && mainball.position.x<=1 && mainball.position.y<=-1 && mainball.position.y>=-2)
		{
			mainball.accx += 0.0002;
			mainball.set_position(mainball.position.x+0.01,mainball.position.y);
		}
		else if(mainball.position.x>=2.1 && mainball.position.x<=3.2 && mainball.position.y>=-1 && mainball.position.y<=-0.5)
		{

		}
		else
		{
			if(mainball.position.x<=3.9)
				mainball.set_position(mainball.position.x+0.1,mainball.position.y);
		}
	}
	
	else if(up)
	{
		if(mainball.position.x>=-1 && mainball.position.x<=1 && mainball.position.y<=-0.8 && mainball.position.y>=-2)
		{
			mainball.speedx = 0.0;
			mainball.speedy = 0.1;
			mainball.accy = -0.05;
		}
		else
		{
			mainball.accy = -0.005;

			if(mainball.position.x>=2.2 && mainball.position.x<=3.1 && mainball.position.y<=-0.4)
				mainball.speedy=0.2;
			if(detect_on_ground((mainball.bounding_box())))
				mainball.speedy = 0.13;
		}
	}
	else
	{
		if(glfwGetKey(window, GLFW_KEY_R))
		{
			screen_center_x=0;
			screen_center_y=0;
			screen_zoom=1;
			reset_screen();
		}
		if (l1 && screen_center_x - 4/screen_zoom > -4) 
		{ 	
			screen_center_x -= 0.05; 
			reset_screen(); 
		}
		if (r1 && screen_center_x + 4/screen_zoom < 4) 
		{
			screen_center_x += 0.05;
			reset_screen();
		}
		if (glfwGetKey(window, GLFW_KEY_W) && screen_center_y + 4/screen_zoom < 4)
		{ 
			screen_center_y += 0.05;
			reset_screen();
		}
		if (glfwGetKey(window, GLFW_KEY_S) && screen_center_y - 4/screen_zoom > -4)
		{
			screen_center_y -= 0.05; 
			reset_screen();
		}
	}
}

void tick_elements() 
{
	int i;
	if(lives<=0)
	{
		glfwSetWindowTitle(window,"GAME OVER");
		mainball.position.x=-100;
	}
	else
	{
		char buffer1[1234];
		char buffer2[1234];
		char buffer3[1234];

		string strb;
		strb="Lives : ";

		sprintf(buffer1,"%d",lives);
		strb+=buffer1;

		strb+=" Score : ";

		sprintf(buffer2,"%d",score);
		strb+=buffer2;

		strb+=" Level : ";

		sprintf(buffer3,"%d",level);
		strb+=buffer3;

		char outstring[1234];

		for(i=0;i<strb.length();i++)
			outstring[i]=strb[i];
		outstring[i]='\0';

		glfwSetWindowTitle(window,outstring);
		if(score>level*100)
		{
			level++;
			for(int j=0;j<numballs;j++)
			{
				if(ball[j].speedx<0)
					ball[j].speedx-=0.01;
				else
					ball[j].speedx+=0.01;
			}
		}
	}

	for(i=0;i<numballs;i++)
		ball[i].tick();
	mainball.tick();

	for(i=0;i<2;i++)
	{
		float theta=M_PI/2;
		if(i==1)
			theta=M_PI/3.5;
		float xpos=ball[i].position.x;
		float ypos=ball[i].position.y;
		float newxpos=xpos+sz[i]*cos(theta);
		float newypos=ypos+sz[i]*sin(theta);
		float m1;
		if(newypos==ypos && newxpos>xpos)
			m1=M_PI/2;
		else if(newypos==ypos && newxpos<xpos)
			m1=-M_PI/2;
		else
			m1=-atan((newxpos-xpos)/(newypos-ypos));
		plank[i]=Plank(newxpos,newypos,COLOR_LBROWN,m1);
	}

	if(mainball.position.x>=2.2 && mainball.position.x<=3.1 && mainball.position.y<=-0.4)
	{
		mainball.accy = -0.005;
		mainball.speedy=0.2;
	}
	for(i=0;i<4;i++)
		kaatein[i].tick();
	if(tt.elapsedTime()%20==0 && tt.elapsedTime()>1)
	{
		if(fl==0)
		{
			fl=1;
			int nn=rand()%2;
			float x4;
			if(nn==1)
				x4=-3;
			else
				x4=3;
			magnet=Magnet(x4,3,COLOR_CRIMSON,0.5);
			t22=tt.elapsedTime();
		}
	}
	else
	{
		if(fl==1 && tt.elapsedTime()>t22+5)
		{
			fl=0;
			magnet=Magnet(-20,3,COLOR_CRIMSON,0.5);
		}
	}


	if(mainball.position.y>2 && magnet.position.x>-4 && magnet.position.x<4)
	{
		if(magnet.position.x<0 && mainball.position.x>=magnet.position.x)
			mainball.accx=-0.0005;
		else if(magnet.position.x>0 && mainball.position.x<=magnet.position.x)
			mainball.accx=0.0005;
	}
	for(i=0;i<numballs;i++)
	{
		if(ball[i].speedx>0 && ball[i].position.x<0)
		{
		}
		else if(ball[i].speedx<0 && ball[i].position.x>0)
		{			
		}
		else if(detect_col_ball(ball[i].bounding_box()))
		{   
			ball[i].speedx = -ball[i].speedx;
			ball[i].speedy = -ball[i].speedy;
		}
		if(i==0)
		{
			if(detect_collision(mainball.bounding_box(),ball[i].bounding_box()) && mainball.speedy<0)
			{
				mainball.speedy*=-1;
				mainball.speedx*=-1;
			}
		}
		if(i==1)
		{
			if(detect_collision(mainball.bounding_box(),ball[i].bounding_box()) && mainball.speedy<0)
			{
				mainball.speedx=0.04;
			}
		}
		if(i>=2 && detect_collision(mainball.bounding_box(),ball[i].bounding_box()) && mainball.speedy<0)
		{
			score+=10;
			if(ball[i].speedx<0)
				ball[i].set_position(5,ball[i].position.y);
			else
				ball[i].set_position(-5,ball[i].position.y);
			mainball.speedy=0.1;
		}
	}
	for(i=0;i<4;i++)
	{
		if(tt.elapsedTime()%10==0 && tt.elapsedTime()>1)
		{
			if(flag[i]==0)
			{
				flag[i]=1;
				int nn=rand()%2;
				float x1=-4+(rand()/(float) RAND_MAX)*2.3;
				float x2=1.1+(rand()/(float) RAND_MAX)*0.5;
				float x4=x1;
				if(nn==1)
					x4=x2;
				kaatein[i].set_position(x4,kaatein[i].position.y);
				t11=tt.elapsedTime();
			}
		}
		else
		{
			if((flag[i]==1 && tt.elapsedTime()>(t11+5+level)))
			{
				flag[i]=0;
				kaatein[i].set_position(kaatein[i].position.x-100,kaatein[i].position.y);
			}
		}
		if(detect_collision(mainball.bounding_box(),kaatein[i].bounding_box()))
		{
			lives--;
			kaatein[i].set_position(kaatein[i].position.x-20,kaatein[i].position.y);
		}
		if(abs(kaatein[i].position.x-2)<=0.1)
		{
			kaatein[i].speedx*=-1;
		}
		if(abs(kaatein[i].position.x+1)<=0.1)
		{
			kaatein[i].speedx*=-1;
		}
		if(abs(kaatein[i].position.x-1)<=0.1)
		{
			kaatein[i].speedx*=-1;
		}
	}
	if(detect_col_ball(mainball.bounding_box()))
	{   
		mainball.speedx = -mainball.speedx;
		mainball.speedy = -mainball.speedy;

	}
	if(mainball.position.x>2.2 && mainball.position.x<3.1 && mainball.position.y<=-0.4)
	{
		mainball.position.y=-0.4;
	}
	if(!(mainball.position.x>=-1 && mainball.position.x<=1 && mainball.position.y<=-0.8 && mainball.position.y>=-2))
	{
		if(detect_on_ground(mainball.bounding_box()))
		{
			mainball.set_position(mainball.position.x,-0.8);
			mainball.speedy = 0.0;
			mainball.accy = 0.0;
			mainball.speedx=0.0;
			mainball.accx=0.0;
		}
	}


}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
	srand(time(0));
	int i,y1=0;
	for(i=0;i<numballs;i+=2)
	{
		float r = 0.1+(rand() / (float) RAND_MAX)*0.2;
		int x1=-3+rand()%4;
		ball[i]=Ball(x1,y1,COLOR_YELLOW,r);
		y1=rand()%3;
		sz[i]=r;
	}
	for(i=1;i<numballs;i+=2)
	{
		float r = 0.1+(rand() / (float) RAND_MAX)*0.2;
		int x1=-3+rand()%4;
		ball[i]=Ball(x1,y1,COLOR_GREEN,r);
		y1=3+rand()%2;
		sz[i]=r;
	}
	int cnt=0;
	mainball = Ball(-2,-1, COLOR_BLACK,0.2);
	mainball.set_position(mainball.position.x,mainball.position.y+0.2);
	ground=Ground(1,3,COLOR_GREEN,0.3);
	ground2=Ground2(1,3,COLOR_LBROWN,0.3);
	trampoline=Trampoline(1,3,COLOR_RED,0.3);
	magnet=Magnet(23,3,COLOR_CRIMSON,0.4);
	mainball.speedx=0.0;
	
	for(i=0;i<4;i++)
		kaatein[i]=Kaatein(-20,-0.8,COLOR_BLACK,0.3); 
	for(i=0;i<2;i++)
	{
		int xpos=ball[i].position.x;
		int ypos=ball[i].position.y;
		plank[i]=Plank(xpos,ypos,COLOR_LBROWN,0.2);
	}
	pond=Pond(0,-1,COLOR_LBLUE,1); 

    // Create and compile our GLSL program from the shaders
	
	programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");

    // Get a handle for our "MVP" uniform
	
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 1.0, COLOR_BACKGROUND.b / 1.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}
int main(int argc, char **argv) 
{
	numballs=12;
	level=1;
	tt.start();
	srand(time(0));
	int width  = 1200;
	int height = 1200;
	lives=7;
	score=0;

	window = initGLFW(width, height);

	initGL (window, width, height);

    /* Draw in loop */
	while(!glfwWindowShouldClose(window)) 
	{
        // Process timers

		if(t60.processTick()) 
		{
            // 60 fps
            // OpenGL Draw commands
			draw();
            // Swap Frame Buffer in double buffering
			glfwSwapBuffers(window);

			tick_elements();
			tick_input(window);
		}
        // Poll for Keyboard and mouse events
		glfwPollEvents();
	}

	quit(window);
}
bool detect_col_ball( const bounding_box_t &a)
{
	if(abs(a.y)>4 || abs(a.x)>4)
		return 1;
	return 0;
}
bool detect_on_ground(const bounding_box_t &a)
{
	return a.y<=-0.8;
}
bool detect_collision(bounding_box_t a, bounding_box_t b) 
{
	return ((abs(a.x - b.x) * 2 < (a.width + b.width)) &&
		(abs(a.y - b.y) * 2 < (a.height + b.height)));
}
void reset_screen() 
{
	float top    = screen_center_y + 4 / screen_zoom;
	float bottom = screen_center_y - 4 / screen_zoom;
	float left   = screen_center_x - 4 / screen_zoom;
	float right  = screen_center_x + 4 / screen_zoom;
	Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
