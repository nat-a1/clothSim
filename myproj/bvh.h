#pragma once

#include <glm/glm.hpp>

#include "TriangleMesh.h"
#include "mySphere.h"
#include "utils.h"

using namespace glm;


/*
*	###############################################
*	###############################################
	class Aabb
*/

class Aabb{
	public:

		vec3 p1,p2,c;
		Triangle* t;

		float l, L, h;

		bool isLeaf;

		Aabb();
		Aabb(Triangle *t);
		Aabb(vec3 center);

		void updateBox();				 // pour les feuilles les triangles ne changent pas de forme, mais changent d'orientation donc on doit recalculer la boite?
		void unir(Aabb *a, Aabb *b);
		void updatePos();
		
		bool overlapBox(Aabb *a);
		bool overlapPoint(vec3 p);

		void draw();
};


/*
*	###############################################
*	###############################################
	class node
*/

class Node{
	public:
		bool isLeaf;
		float angle;
		vec3 axis; 

		Aabb box;
		Node* child1, *child2, *parent;

		Node();
		Node(Triangle* t); // --> créer une feuille (p1, p2, p3 sont les 3 points du triangle)
		Node(Node* c1, Node* c2);			// --> créer un parent 

		void update();
};


///  il faut appeler "update" à chaque frame pour avoir un AAbb dynamique 
///  (le bout de tissu change de forme) 



class Tree {
	public:

		std::vector< Node* > nodes;

		Tree();
		Tree(TriangleMesh *myMesh);

		void creerParents(int i1, int j1, int w, int h, int d);
		void drawBVH();
		void drawBVH2(Node* n, int depth, Aabb * a);

		void drawNodes(Node* n, int d);
		void draw();
		void update();
		void calculeAngles();

		void checkCollision(Node *a, Node*b);
		void checkSelfCollision(Node* a);
		void interpolation(Triangle *t1, Triangle *t2);
		void applyImpulse(Triangle* t, myParticle* p, double I, vec3 n,vec3*b);
		void Tree::applyImpulse(std::vector<myParticle*> e1, std::vector<myParticle*> e2, double a, double b, double I, vec3 n);

};
