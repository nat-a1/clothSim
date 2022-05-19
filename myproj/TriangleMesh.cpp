#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "TriangleMesh.h"
#include "helperFunctions.h"
#include <glm/gtx/normal.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>





TriangleMesh::TriangleMesh(){}


TriangleMesh::TriangleMesh(std::vector< std::vector<myParticle> >* p_particles, unsigned int width, unsigned int height)
{
	unsigned int i, j;
	n_triangles = (width - 1) * 2 * (height - 1); // nombre de triangles du maillage
	int compt = 0;
	triangle_list.assign(n_triangles, Triangle());

	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height - 1; j++)
		{
			triangle_list[compt] = Triangle(&((*p_particles)[i][j]), &((*p_particles)[i + 1][j]), &((*p_particles)[i][j + 1])); // Créer le triangle supérieur
			triangle_list[compt + 1] = Triangle(&((*p_particles)[i][j + 1]), &((*p_particles)[i + 1][j]), &((*p_particles)[i + 1][j + 1])); // Créer le triangle inférieur

			compt += 2;
		}
	}
	calculateNormals();
}
void TriangleMesh::calculateNormals()
{
	// Calcul des normales de chaque triangle de triangle_list
	for (auto& triangle : triangle_list) {
		triangle.calculateNormals();
	}

}

TriangleMesh::~TriangleMesh(){}

Triangle::Triangle(){}
Triangle::Triangle(Triangle* p_t)
{
	a_p1 = p_t->a_p1;
	a_p2 = p_t->a_p2;
	a_p3 = p_t->a_p3;
	h = p_t->h;
	normal = p_t->normal;
	
}
Triangle::Triangle(myParticle* p1, myParticle* p2, myParticle* p3, float p_h)
{
	a_p1 = p1;
	a_p2 = p2;
	a_p3 = p3;
	h = p_h;
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	area = 0;

	edges = {
		{p1,p2},
		{p2,p3},
		{p3,p1}
	};
}
bool Triangle::checkThicknessCollision(myParticle p) {
	// Vérifie si le triange et la particule sont coplanaires
	return glm::dot(glm::abs(p.position - a_p3->position), normal) < h;

}

bool Triangle::checkCollision(myParticle p) {

	if (!this->checkThicknessCollision(p)) return false;

	const float EPSILON = 0.0000001;

	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = a_p2->position - a_p1->position;
	edge2 = a_p3->position - a_p1->position;
	h = glm::cross(p.normal, edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;    // Le rayon est parallèle au triangle.

	f = 1.0 / a;
	s = p.position - a_p1->position;
	u = f * (glm::dot(s, h));
	if (u < 0.0 || u > 1.0)
		return false;
	q = glm::cross(s, edge1);
	v = f * glm::dot(p.normal, q);
	if (v < 0.0 || u + v > 1.0)
		return false;

	// On calcule t pour savoir ou le point d'intersection se situe sur la ligne.
	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // Intersection avec le rayon
	{
		//outIntersectionPoint = rayOrigin + rayVector * t;
		return true;
	}
	else // On a bien une intersection de droite, mais pas de rayon.
		return false;
}
void Triangle::calculateNormals()
{
	// Calcul de la normale du triangle
	area = tri_area(a_p1, a_p2, a_p3);
	normal = glm::triangleNormal((*a_p1).position, (*a_p2).position, (*a_p3).position);
	return;

}

Triangle::~Triangle(){}



void Triangle::draw() {

	glNormal3fv(&a_p1->normal[0]);
	glVertex3fv(&a_p1->position[0]);

	glNormal3fv(&a_p2->normal[0]);
	glVertex3fv(&a_p2->position[0]);

	glNormal3fv(&a_p3->normal[0]);
	glVertex3fv(&a_p3->position[0]);

	
}
glm::vec3 Triangle::computeBCC(myParticle* p) {
	/*
		float a = tri_area(a_p1, a_p2, a_p3);
		float a1 = tri_area(a_p2,a_p3,p) / a;
		float a2 = tri_area(a_p3, a_p1, p) / a;
		float a3 = tri_area(a_p1, a_p2, p) / a;

		return a1*a_p1->position + a2*a_p2->position + a3*a_p3->position;
	*/
	/**/
	float a = dot(a_p1->position-a_p3->position,a_p1->position - a_p3->position);
	double b = dot(a_p1->position - a_p3->position, -(a_p3->position - a_p2->position));
	double c = dot(a_p1->position - a_p3->position, a_p3->position - a_p2->position);//;v13 * -v32;
	double d = dot(-(a_p3->position - a_p2->position), -(a_p3->position - a_p2->position));// -v32 * -v32;

	double e = dot(a_p1->position - a_p3->position, (p->position - a_p3->position));
	double f = dot(a_p3->position - a_p2->position, (p->position - a_p3->position));
	
	double w2 = (c * e / a - f) / (c * b / a - d);
	double w1 = (e - b * w2) / a;
	double w3 = 1 - w1 - w2;
	return glm::vec3(w1, w2, w3);
}

glm::vec3 Triangle::proximity(myParticle*p) {

	glm::vec3 v43 = p->position - a_p3->position;

	double result = glm::dot(v43,normal);
	if (result < 0) normal = -normal;
	result = fabs(result);

	if (result > DEFAULT_THICKNESS)
		return glm::vec3(0, 0, 0);
	
	glm::vec3 res = computeBCC(p);

	double delta = DEFAULT_THICKNESS / area;
	if ((res.x > -delta && res.x < 1 + delta) &&
		(res.y > -delta && res.y < 1 + delta) &&
		(res.z > -delta && res.z < 1 + delta)) {
		
		return res;

	}
	return glm::vec3(0,0,0);
	
}

float Triangle::tri_area(myParticle* p1, myParticle* p2, myParticle* p3) {
	return glm::dot((p3->position - p1->position), (p2->position - p1->position)) / 2.0f;
}

void TriangleMesh::draw() {

	for (Triangle &t : triangle_list) {
		t.draw();
	}
}