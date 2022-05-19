
#include <fstream>
#include <iostream>

#include "objet.h"
#include <glm/glm.hpp>
using namespace glm;

/* Object constructor */
Object::Object(char* filename, int texture,
	GLfloat ar, GLfloat ag, GLfloat ab,
	GLfloat dr, GLfloat dg, GLfloat db,
	GLfloat sr, GLfloat sg, GLfloat sb,
	GLfloat s)
{
	//readMesh(filename);
	readMesh2(filename);
	//computeNormals();
	initBuffers();

	textureId = texture;

	ka[0] = ar;
	ka[1] = ag;
	ka[2] = ab;
	ka[3] = 1;

	kd[0] = dr;
	kd[1] = dg;
	kd[2] = db;
	kd[3] = 1;

	ks[0] = sr;
	ks[1] = sg;
	ks[2] = sb;
	ks[3] = 1;

	shininess = s;
}


/* Object constructor */
Object::Object(char* filename,
	GLfloat ar, GLfloat ag, GLfloat ab,
	GLfloat dr, GLfloat dg, GLfloat db,
	GLfloat sr, GLfloat sg, GLfloat sb,
	GLfloat s)
{
	readMesh(filename);
	//readMesh2(filename);
	computeNormals();
	initBuffers();

	//textureId = texture;

	ka[0] = ar;
	ka[1] = ag;
	ka[2] = ab;
	ka[3] = 1;

	kd[0] = dr;
	kd[1] = dg;
	kd[2] = db;
	kd[3] = 1;

	ks[0] = sr;
	ks[1] = sg;
	ks[2] = sb;
	ks[3] = 1;

	shininess = s;
}


/* Object constructor */
Object::Object(
	GLfloat ar, GLfloat ag, GLfloat ab,
	GLfloat dr, GLfloat dg, GLfloat db,
	GLfloat sr, GLfloat sg, GLfloat sb,
	GLfloat s)
{
	//readMesh(filename);
	//readMesh2(filename);
	//computeNormals();
	//initBuffers();

	//textureId = texture;

	ka[0] = ar;
	ka[1] = ag;
	ka[2] = ab;
	ka[3] = 1;

	kd[0] = dr;
	kd[1] = dg;
	kd[2] = db;
	kd[3] = 1;

	ks[0] = sr;
	ks[1] = sg;
	ks[2] = sb;
	ks[3] = 1;

	shininess = s;
}


/* Object destructor */
Object::~Object(void)
{
}


/* Loads an obj */
void Object::readMesh(char* filename)
{
	char c[100];

	std::ifstream fin(filename);

	fin >> c;
	n = atoi(c);
	vertices = new GLfloat[3 * n];
	colors = new GLfloat[3 * n];

	fin >> c;
	m = atoi(c);
	indices = new GLuint[3 * m];

	for (int i = 0; i < n; i++)
	{
		fin >> c;

		fin >> c;
		vertices[3 * i] = atof(c);
		fin >> c;
		vertices[3 * i + 1] = atof(c);
		fin >> c;
		vertices[3 * i + 2] = atof(c);

		colors[3 * i] = 1.0 - (float)i / (float)n;
		colors[3 * i + 1] = 0.0;
		colors[3 * i + 2] = (float)i / (float)n;
	}

	for (int i = 0; i < m; i++)
	{
		fin >> c;

		fin >> c;
		indices[3 * i] = atoi(c) - 1;
		fin >> c;
		indices[3 * i + 1] = atoi(c) - 1;
		fin >> c;
		indices[3 * i + 2] = atoi(c) - 1;
	}
}


/* Loads an obj */
void Object::readMesh2(char* filename)
{
	/*int i;
	char c[200];
	char *h, *t;

	std::ifstream fin(filename);

	fin >> c;
	m = atoi(c);
	fin.getline(c, 200);

	n = 3*m;

	vertices = new GLfloat[3*n];
	normals = new GLfloat[3*n];
	textures = new GLfloat[2*n];
	indices = new GLuint[3*m];

	GLfloat *tmpvertices = new GLfloat[3*3*m]; int tv;
	GLfloat *tmptextures = new GLfloat[3*3*m]; int tt;
	GLfloat *tmpnormals = new GLfloat[3*3*m]; int tn;

	i = 0, tv=0, tt=0, tn=0;
	while(i < 3*m) {
		fin.getline(c, 200);
		h = strtok(c, " ");

		if (!strcmp(h, "v"))
		{
			t = strtok(NULL, " ");
			tmpvertices[3*tv] = atof(t);
			t = strtok(NULL, " ");
			tmpvertices[3*tv+1] = atof(t);
			t = strtok(NULL, " ");
			tmpvertices[3*tv+2] = atof(t);
			tv++;
		}
		else if (!strcmp(h, "vn"))
		{
			t = strtok(NULL, " ");
			tmpnormals[3*tn] = atof(t);
			t = strtok(NULL, " ");
			tmpnormals[3*tn+1] = atof(t);
			t = strtok(NULL, " ");
			tmpnormals[3*tn+2] = atof(t);
			tn++;
		}
		else if (!strcmp(h, "vt"))
		{
			t = strtok(NULL, " ");
			tmptextures[2*tt] = atof(t);
			t = strtok(NULL, " ");
			tmptextures[2*tt+1] = atof(t);
			tt++;
		}
		else if (!strcmp(h, "f"))
		{
			t = strtok(NULL, "/"); vertices[3*i] = tmpvertices[3*(atoi(t)-1)]; vertices[3*i+1] = tmpvertices[3*(atoi(t)-1)+1]; vertices[3*i+2] = tmpvertices[3*(atoi(t)-1)+2];
			t = strtok(NULL, "/"); textures[2*i] = tmptextures[2*(atoi(t)-1)]; textures[2*i+1] = tmptextures[2*(atoi(t)-1)+1];
			t = strtok(NULL, " "); normals[3*i] = tmpnormals[3*(atoi(t)-1)]; normals[3*i+1] = tmpnormals[3*(atoi(t)-1)+1]; normals[3*i+2] = tmpnormals[3*(atoi(t)-1)+2];
			indices[i] = i;
			i++;
			t = strtok(NULL, "/"); vertices[3*i] = tmpvertices[3*(atoi(t)-1)]; vertices[3*i+1] = tmpvertices[3*(atoi(t)-1)+1]; vertices[3*i+2] = tmpvertices[3*(atoi(t)-1)+2];
			t = strtok(NULL, "/"); textures[2*i] = tmptextures[2*(atoi(t)-1)]; textures[2*i+1] = tmptextures[2*(atoi(t)-1)+1];
			t = strtok(NULL, " "); normals[3*i] = tmpnormals[3*(atoi(t)-1)]; normals[3*i+1] = tmpnormals[3*(atoi(t)-1)+1]; normals[3*i+2] = tmpnormals[3*(atoi(t)-1)+2];
			indices[i] = i;
			i++;
			t = strtok(NULL, "/"); vertices[3*i] = tmpvertices[3*(atoi(t)-1)]; vertices[3*i+1] = tmpvertices[3*(atoi(t)-1)+1]; vertices[3*i+2] = tmpvertices[3*(atoi(t)-1)+2];
			t = strtok(NULL, "/"); textures[2*i] = tmptextures[2*(atoi(t)-1)]; textures[2*i+1] = tmptextures[2*(atoi(t)-1)+1];
			t = strtok(NULL, " "); normals[3*i] = tmpnormals[3*(atoi(t)-1)]; normals[3*i+1] = tmpnormals[3*(atoi(t)-1)+1]; normals[3*i+2] = tmpnormals[3*(atoi(t)-1)+2];
			indices[i] = i;
			i++;
		}
	}*/
}


/* Computes normal */
void Object::computeNormal(int v1, int v2, int v3, double& x, double& y, double& z)
{
	double dx1 = vertices[v2 * 3] - vertices[v1 * 3];
	double dx2 = vertices[v3 * 3] - vertices[v2 * 3];
	double dy1 = vertices[v2 * 3 + 1] - vertices[v1 * 3 + 1];
	double dy2 = vertices[v3 * 3 + 1] - vertices[v2 * 3 + 1];
	double dz1 = vertices[v2 * 3 + 2] - vertices[v1 * 3 + 2];
	double dz2 = vertices[v3 * 3 + 2] - vertices[v2 * 3 + 2];

	double dx = dy1 * dz2 - dz1 * dy2;
	double dy = dz1 * dx2 - dx1 * dz2;
	double dz = dx1 * dy2 - dy1 * dx2;

	double length = sqrt(dx * dx + dy * dy + dz * dz);

	x = dx / length;
	y = dy / length;
	z = dz / length;
}

/* Computes normals */
void Object::computeNormals()
{
	int k;
	double x1, y1, z1;

	normals = new GLfloat[3 * n];
	for (int i = 0; i < n; i++)
	{
		k = 0;
		normals[3 * i] = 0;
		normals[3 * i + 1] = 0;
		normals[3 * i + 2] = 0;

		for (int j = 0; j < m; j++)
		{
			if (indices[3 * j] == i || indices[3 * j + 1] == i || indices[3 * j + 2] == i)
			{
				computeNormal(indices[3 * j], indices[3 * j + 1], indices[3 * j + 2], x1, y1, z1);
				normals[3 * i] += x1; normals[3 * i + 1] += y1; normals[3 * i + 2] += z1;
				k++;
			}
		}
		normals[3 * i] /= k; normals[3 * i + 1] /= k; normals[3 * i + 2] /= k;
	}
}


/* Buffers initialization */
void Object::initBuffers()
{
	glGenBuffers(5, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * n * 3, vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, buffers[1]) ; 
	//glBufferData(GL_ARRAY_BUFFER, n*3*4, colors, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * m * 3, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals) * n * 3, normals, GL_STATIC_DRAW);

	/* glBindBuffer(GL_ARRAY_BUFFER, buffers[4]) ;
	 glBufferData(GL_ARRAY_BUFFER, n*2*4, textures, GL_STATIC_DRAW);*/
}


/* Displays an object */
void Object::display()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glColorPointer(3, GL_FLOAT, 0, 0) ;
	glEnableClientState(GL_COLOR_ARRAY) ;
	*/
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glNormalPointer(GL_FLOAT, 0, 0);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);

	glDrawElements(GL_TRIANGLES, m * 3, GL_UNSIGNED_INT, 0);
}


/* Draws a cube on the scene */
void Object::drawCube(vec3 p, double size)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	glutWireCube(size);
	glPopMatrix();
}

