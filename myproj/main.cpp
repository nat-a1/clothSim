#include <fstream>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <gl\glaux.h>
#include "GL/glui.h"

#include <time.h> 
#include <vector>
#include <stdlib.h>      

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <SOIL/SOIL.h>

#include "configuration.h"
#include "init_worldvars.h"

#include "myShader.h"
#include "helperFunctions.h"
#include "myParticle.h"
#include "mySpring.h"
#include "Wind.h"
#include "bvh.h"
#include "myMassSpring.h"
#include "mySphere.h"
#include <iostream>
#include "myCamera.h"
#include "default_constants.h"






using namespace std;

void init();

//glui
int   wireframe = 0;
int   segments = 8;
int   main_window,subwin;
int   welcome_win, main_win;

float grav = 1;

myShader *shader;
myCamera *camera;
 
static int GLUTmouse[2] = { 0, 0 };
static int GLUTbutton[3] = { 0, 0, 0 };

mySphere *sphere;

myMassSpring *my_mass_spring;
 
Wind *wind;
int ang;
bool smove = true;
////////// glui __
GLUI* glui_subwin, *subwin2;

/// texture.
GLuint texture;












///////////////////////////////////





int rx = 0, ry = 0;
void idle2() {

	rx = (rx + 1) % 15;
	if (rx == 0) {
		camera->crystalball_rotateView(1, ry);
	}

	glutPostRedisplay();

	if (smove) {
		my_mass_spring->addForces(*wind);
		my_mass_spring->groundCollision(-CUBE_RADIUS);


		my_mass_spring->calculateNextPosition();
		my_mass_spring->clearForces();
	}

	if (sphere != nullptr) my_mass_spring->ballCollision(sphere);

	my_mass_spring->computeNormals();


	if (glutGetWindow() != main_win)
		glutSetWindow(main_win);
	glutPostRedisplay();

	//glutSetWindow(main_win);



	if (sphere == nullptr || sphere->center.x < -4 * CUBE_RADIUS || sphere->center.y < -4 * CUBE_RADIUS)
	{
		if (sphere != nullptr) delete sphere;
		//sphere = new mySphere(glm::vec3(CUBE_RADIUS, 2, 0), 6, glm::vec4(0.4, 0.0, 0.4, 0.0), 0.01f);
		float random_y = 2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 2.0f;
		float random_z = 2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.0f;
		float random_radius = 3 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) + 0.70f);
		float random_speed = 0.03f;//* (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)+0.1f);

		//sphere = new mySphere(glm::vec3(2*CUBE_RADIUS, random_y, random_z), random_radius, glm::vec4((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), 0.0), random_speed);
		sphere = new mySphere(glm::vec3(0.0f, 1.0f, 5.0f * SPACE_P1_TO_P2), random_radius, glm::vec4((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), 0.0), random_speed);

	}
	wind->updateSpeed();
	if (glutGetWindow() != welcome_win)
		glutSetWindow(welcome_win);
	glutPostRedisplay();
}

void disp2() {

}



void mouse(int button, int state, int x, int y)
{


  int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
  GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;

  GLUTmouse[0] = x;
  GLUTmouse[1] = y;

  glutPostRedisplay();
  
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
		camera->moveForward(DEFAULT_MOUSEWHEEL_MOVEMENT_STEPSIZE);
	else
		camera->moveBack(DEFAULT_MOUSEWHEEL_MOVEMENT_STEPSIZE);

	glutPostRedisplay();
}

void mousedrag(int x, int y)
{   
  if (GLUTbutton[0])
  {
	  camera->crystalball_rotateView(x - GLUTmouse[0], y - GLUTmouse[1]);
      glutPostRedisplay(); 
  }

	if (GLUTbutton[2])
  {
	  camera->panView( 16*(x - GLUTmouse[0]), 16 * (y - GLUTmouse[1]) );
	  glutPostRedisplay();
  }
	
  GLUTmouse[0] = x;
  GLUTmouse[1] = y;
}

void keyboard(unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case 'a': 
			glutPostRedisplay();
			break;
		case 'h':
			std::cout << "There is no help. You're on your own :(\n";
			break;
		case 'r':
			init();
			std::cout << "\n";
			break;
		case 27:  
			exit(0) ;
			break ;
	}
	glutPostRedisplay();
}


void keyboard2(int key, int x, int y) 
{
	smove = false;
 	int mode = glutGetModifiers();

	switch(key) 
	{
		case GLUT_KEY_UP:
			camera->moveForward(DEFAULT_KEY_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_DOWN:
			camera->moveBack(DEFAULT_KEY_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_LEFT:
			camera->turnLeft(DEFAULT_LEFTRIGHTTURN_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_RIGHT:
			camera->turnRight(DEFAULT_LEFTRIGHTTURN_MOVEMENT_STEPSIZE);
			break;
		case GLUT_KEY_F1:
			wind->amplitude += 1;
			my_mass_spring->depth += 1;
			smove = true;
			break;
		case GLUT_KEY_F2:
			wind->amplitude -= 1;
			my_mass_spring->depth -= 1;
			break;
	}
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	camera->window_width = width;
	camera->window_height = height;
}
 
void idle()
{
	
	if (sphere != nullptr )
		sphere->translate(glm::vec3(-sphere->velocity, 0.0f, 0.0f));
	 
	if (smove) {
		my_mass_spring->addForces(*wind);
		my_mass_spring->groundCollision(-CUBE_RADIUS);

	
		my_mass_spring->calculateNextPosition();
		my_mass_spring->clearForces();
	}

	if (sphere != nullptr) my_mass_spring->ballCollision(sphere);

	my_mass_spring->computeNormals();
	

	if (glutGetWindow() != main_win)
		glutSetWindow(main_win);
	glutPostRedisplay();

    //glutSetWindow(main_win);



	if (sphere == nullptr || sphere->center.x < -4*CUBE_RADIUS || sphere->center.y < -4*CUBE_RADIUS)
	{
		if (sphere != nullptr) delete sphere;
		//sphere = new mySphere(glm::vec3(CUBE_RADIUS, 2, 0), 6, glm::vec4(0.4, 0.0, 0.4, 0.0), 0.01f);
		float random_y = 2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 2.0f;
		float random_z = 2 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.0f;
		float random_radius = 3 * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) + 0.70f);
		float random_speed = 0.03f;//* (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)+0.1f);

		//sphere = new mySphere(glm::vec3(2*CUBE_RADIUS, random_y, random_z), random_radius, glm::vec4((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), 0.0), random_speed);
		sphere = new mySphere(glm::vec3(0.0f, 1.0f,5.0f*SPACE_P1_TO_P2 ), random_radius, glm::vec4((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)), 0.0), random_speed);

	}
	wind->updateSpeed();
}


void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	glViewport(0, 0, camera->window_width, camera->window_height);

	glm::mat4 projection_matrix = camera->projectionMatrix();
	shader->setUniform("myprojection_matrix", projection_matrix);

	glm::mat4 view_matrix = camera->viewMatrix();
	shader->setUniform("myview_matrix", view_matrix);

	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(view_matrix)));
	shader->setUniform("mynormal_matrix", normal_matrix);

	
	//drawing the sphere.
	if (sphere != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		//sphere->box.draw();
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		shader->setUniform("kd", sphere->kd);
		shader->setUniform("mymodel_matrix", glm::translate(glm::mat4(1.0f), sphere->center));
		glutSolidSphere(sphere->radius - 0.2, 50, 50);
	
	}
	
	shader->setUniform("mymodel_matrix", glm::mat4(1.0f));
	

	
	//drawing the bounding cube.
	glutWireCube(2*CUBE_RADIUS);
	
	//drawing the mass spring.
	
	shader->setUniform("kd", my_mass_spring->kd);
	my_mass_spring->drawSpring();
	//my_mass_spring->drawBVH();

	/*
	glm::vec3 rotAxis(0.0f, -1.0f, 0.0f);
	shader->setUniform("mymodel_matrix", glm::rotate(glm::mat4(1.0f), wind->angle-1.7f,rotAxis));
	glutSolidCone(1, 4, 10, 10);
	*/
	//////////////////////////////////
	//nat : draw all the bounding cubes

	
	shader->setUniform("mymodel_matrix", glm::mat4(1.0f));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	//if (sphere != nullptr) my_mass_spring->myTree->drawBVH2(my_mass_spring->myTree->nodes[my_mass_spring->myTree->nodes.size() - 1], 0, sphere);
	if (DRAW_BVH == 1) {
		my_mass_spring->myTree->drawBVH();
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	glFlush();
}

void init()
{
	srand( (unsigned int)time(NULL));
	sphere = nullptr;
	my_mass_spring = new myMassSpring();
	wind = new Wind();
	ang = 0;
}

void apply_changes() {
}

void goto_mainwindow() {
	glutHideWindow();
	glutSetWindow(main_win);
	glutInitWindowPosition(50, 0);
	glutShowWindow();
	GLUI_Master.set_glutIdleFunc(idle);
	GLUI_Master.set_glutDisplayFunc(display);
	camera->window_width = 750;
	camera->window_height = 600;
	glutReshapeWindow(camera->window_width, camera->window_height);
	
}

void init_scenario_1() {
	CONFIG = 1;
	init();
}
void init_scenario_2() {
	CONFIG = 2;
	init();
}
void init_scenario_3() {
	CONFIG = 3;
	init();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );

	glutInitWindowSize(640, 480);
	//glutInitWindowPosition(50, 200);
	main_win = glutCreateWindow("CLOTHSIM 3D");
	   
	GLenum err = glewInit() ; 
	if (GLEW_OK != err) std::cerr << "Error: " << glewGetString(err) << std::endl; 

	shader = new myShader("shaders/phong-vertex.glsl", "shaders/phong-fragment.glsl");
	camera = new myCamera();

	glClearColor(1.0, 1, 1, 0);
 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS) ;
	glDisable(GL_CULL_FACE);


	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	// glui: main win
	//main_win = glutCreateWindow("Main gfx window");


	glui_subwin = GLUI_Master.create_glui_subwindow(main_win,GLUI_SUBWINDOW_BOTTOM);
	glui_subwin->set_main_gfx_window(main_win);

	// collisions, wind, wireframe, launch ball quit reset


	////////////// options panel
	glui_subwin->add_button("RESET", 0, (GLUI_Update_CB)init);
	glui_subwin->add_button("QUIT", 0, (GLUI_Update_CB)exit);

	//////////////collision panel
	glui_subwin->add_column(true);
	GLUI_Rollout* collisions = glui_subwin->add_rollout("COLLISION",false);
	
	glui_subwin->add_checkbox_to_panel(collisions,"enable self-collisions", &SELF_COLLISIONS_ENABLED, (SELF_COLLISIONS_ENABLED+1)%2);
	glui_subwin->add_checkbox_to_panel(collisions,"draw BVH", &DRAW_BVH, (DRAW_BVH + 1) % 2);
	GLUI_Panel* bvh_view = glui_subwin->add_panel_to_panel(collisions,"bvh view");
	glui_subwin->add_checkbox_to_panel(bvh_view, "dynamic");
	GLUI_Spinner* segment_spinner =
	glui_subwin->add_spinner_to_panel(bvh_view,"reach depth:", GLUI_SPINNER_INT);
	segment_spinner->set_speed(0.4f);
	segment_spinner->set_float_limits(0.0f, 8.0f);

	GLUI_StaticText* text = glui_subwin->add_statictext("");
	text->set_w(200);
	/////////////cloth stiffness panel : stiffness, damping, constraints(none, simple position correction, impulse based correction
	glui_subwin->add_column(true);
	GLUI_Rollout* stiffness = glui_subwin->add_rollout("STIFFNESS",false);
	GLUI_Spinner* springstiff =
		glui_subwin->add_spinner_to_panel(stiffness, "spring stiffness:", GLUI_SPINNER_FLOAT);
	springstiff->set_speed(0.1f);
	springstiff->set_float_limits(1.0f, 8.0f);

	GLUI_Listbox* constraints = glui_subwin->add_listbox_to_panel(stiffness,"correction");
	constraints->add_item(1, "none ");
	constraints->add_item(2, "simple ");
	constraints->add_item(3, "impulse-based");
	GLUI_StaticText* text2 = glui_subwin->add_statictext("");
	text2->set_w(200);
	glui_subwin->add_column(true);
	
	GLUI_Panel* scenarios = glui_subwin->add_panel("scenarios");
	glui_subwin->add_button_to_panel(scenarios,"scene 1",0, (GLUI_Update_CB)init_scenario_1);
	glui_subwin->add_button_to_panel(scenarios, "scene 2", 0,(GLUI_Update_CB)init_scenario_2);
	glui_subwin->add_button_to_panel(scenarios, "scene 3",0, (GLUI_Update_CB)init_scenario_3);
	//GLUI_Rotation* arcball = glui_subwin->add_rotation("ball (doesn't do anything)");
	//glui->set_main_gfx_window(main_window);
	//glui->set_main_gfx_window(main_window);


	GLUI_Master.set_glutIdleFunc(idle);
	GLUI_Master.set_glutDisplayFunc(display);

	GLUI_Master.set_glutSpecialFunc(keyboard2);
	GLUI_Master.set_glutMouseFunc(mouse);
	GLUI_Master.set_glutReshapeFunc(reshape);


	//GLUI_Master.set_glutMotionFunc(mousedrag);
	//GLUI_Master.set_glutReshapeWindow(reshape);

	//glutDisplayFunc(display);
	//glutIdleFunc(idle);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(keyboard2);
	//glutReshapeFunc(reshape);
	glutReshapeWindow(camera->window_width, camera->window_height);
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);

	glutMotionFunc(mousedrag) ;
	glutMouseFunc(mouse) ;
	glutMouseWheelFunc(mouseWheel);
	
	init();
	
	glutHideWindow();

	//glutSetWindow(welcome_win);

	/// welcome*

	//loadTexture(&texture,"./CLOTHSIM3D.jpg");
	
	// create the welcome window
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 200);
	welcome_win = glutCreateWindow("CLOTHSIM 3D");

	shader = new myShader("shaders/phong-vertex.glsl", "shaders/phong-fragment.glsl");


	glClearColor(1.0, 1, 1, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	/////// sub window of welcome win..
	
	subwin2 = GLUI_Master.create_glui_subwindow(welcome_win, GLUI_SUBWINDOW_BOTTOM);
	subwin2->set_main_gfx_window(welcome_win);
	GLUI_StaticText* clothsim = subwin2->add_statictext("CLOTHSIM 3D");

	GLUI_Panel* panel = subwin2->add_panel("");
	subwin2->add_column_to_panel(panel,true);
	auto quitbutton = subwin2->add_button_to_panel(panel,"QUIT", 0, (GLUI_Update_CB)exit);
	quitbutton->set_w(100); quitbutton->set_h(30); 

	subwin2->add_column_to_panel(panel, true);
	auto startbutton = subwin2->add_button_to_panel(panel,"START", 0, (GLUI_Update_CB)goto_mainwindow);
	startbutton->set_w(100); startbutton->set_h(30);

	// titre et auteurs
	subwin2->add_column(true);
	subwin2->add_column(false);
	subwin2->add_statictext("                 par:");
	subwin2->add_column(false);
	subwin2->add_statictext("CHARTIER Aurèle");
	subwin2->add_statictext("ARNOULD Nathan");
	subwin2->add_statictext("DAUNAIS Nicolas");
	subwin2->add_statictext("TOUATI Baptiste");
	GLUI_Master.set_glutIdleFunc(idle2);
	GLUI_Master.set_glutDisplayFunc(display);

	GLUI_Master.set_glutSpecialFunc(keyboard2);
	GLUI_Master.set_glutMouseFunc(mouse);
	GLUI_Master.set_glutReshapeFunc(reshape);
	
	glutReshapeWindow(camera->window_width, camera->window_height);
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);

	glutReshapeWindow(500, 350);

	camera->panView(0, -60);
	camera->crystalball_rotateView(0, 50);


	glutMainLoop();








	return 0;
}




