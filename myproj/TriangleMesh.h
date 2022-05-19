#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "myParticle.h"
#include "configuration.h"
using namespace std;

class Triangle
{
public:
	myParticle* a_p1;
	myParticle* a_p2;
	myParticle* a_p3;
	
	std::vector< std::vector<myParticle*> > edges;

	float h,area;				// thickness

	glm::vec3 normal;

	bool checkThicknessCollision(myParticle p);
	bool checkCollision(myParticle p);
	void calculateNormals();

	Triangle();
	Triangle(Triangle* p_t);
	Triangle(myParticle* p1, myParticle* p2, myParticle* p3, float p_h = DEFAULT_H);
	~Triangle();

	void draw();

	glm::vec3 computeBCC(myParticle *p1);
	glm::vec3 proximity(myParticle *p1);

	float tri_area(myParticle* p1, myParticle* p2, myParticle* p3);
};
class TriangleMesh
{
public:
	int n_triangles, n_sommets;
	std::vector<Triangle> triangle_list;

	//std::vector<glm::vec3 *> normal_list;

	void calculateNormals();

	TriangleMesh();
	TriangleMesh(std::vector< std::vector<myParticle> >* p_particles, unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_WIDTH);
	~TriangleMesh();
	void draw();
};

class Edge {
public:
	Edge(myParticle* p1, myParticle *p2);
};