#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "configuration.h"
#include <glm/gtx/normal.hpp>

using namespace glm;
/* Object class */

class Object
{
public:

	/* Attributes */
	GLfloat* vertices;
	GLuint* indices;
	GLfloat* colors;
	GLfloat* normals;
	GLfloat* textures;
	int n, m;
	GLuint buffers[5];
	GLfloat kd[4], ks[4], ka[4];
	GLfloat shininess;
	int textureId;

	/* Methods */
	Object(char* filename, int texture, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	Object(char* filename, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	Object(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

	~Object(void);

	void initBuffers();
	void display();
	void drawCube(vec3 p, double size);

private:

	void readMesh(char* filename);
	void readMesh2(char* filename);
	void computeNormal(int v1, int v2, int v3, double& x, double& y, double& z);
	void computeNormals();
};

