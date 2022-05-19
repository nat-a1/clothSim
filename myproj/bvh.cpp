#include "bvh.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;
/* c'est peut-être mieux d'utiliser des structs pour les aabb,
*  mais je ne sais pas comment gérer les structs entre .h / .c
*  donc, a voir pour plus tard.
*/

/*
*	##############################################################################################
*	##############################################################################################
	class Aabb
*/

Aabb::Aabb() {

	p1 = vec3(0, 0, 0);
	p2 = vec3(0, 0, 0);

}
Aabb::Aabb(Triangle* tt) {

	t = tt;

	 p1 = vec3(0, 0, 0);
	 p2 = vec3(0, 0, 0);
	 isLeaf = true;
};

// ça sert à rien, à enlever, c'est rien que pour l'exemple de la sphere qui entre en contact avec le tissu
Aabb::Aabb(vec3 center) {
	c = center;
}

// update box
 void Aabb::updateBox() { 
		
	vec3* t1 = &t->a_p1->position, * t2 = &t->a_p2->position, *t3 = &t->a_p3->position;

	float x1 = fminf(fminf(t1->x, t2->x), t3->x);
	float y1 = fminf(fminf(t1->y, t2->y), t3->y);
	float z1 = fminf(fminf(t1->z, t2->z), t3->z);

	float x2 = fmaxf(fmaxf(t1->x, t2->x), t3->x);
	float y2 = fmaxf(fmaxf(t1->y, t2->y), t3->y);
	float z2 = fmaxf(fmaxf(t1->z, t2->z), t3->z);

	p1 = vec3(x1, y1, z1);
	p2 = vec3(x2, y2, z2);

	l = abs(x2 - x1);
	L = abs(y2 - y1);
	h = abs(z2 - z1);
 }
 void Aabb::unir(Aabb* a, Aabb* b) {
	 float x1 = fminf(a->p1.x, b->p1.x);
	 float y1 = fminf(a->p1.y, b->p1.y);
	 float z1 = fminf(a->p1.z, b->p1.z);

	 float x2 = fmaxf(a->p2.x, b->p2.x);
	 float y2 = fmaxf(a->p2.y, b->p2.y);
	 float z2 = fmaxf(a->p2.z, b->p2.z);

	 p1.x = x1; p1.y = y1; p1.z = z1;
	 p2.x = x2; p2.y = y2; p2.z = z2;

	 l = abs(x2 - x1);
	 L = abs(y2 - y1);
	 h = abs(z2 - z1);
 }
 void Aabb::updatePos() {
	 l = abs(p2.x - p1.x);
	 L = abs(p2.y - p1.y);
	 h = abs(p2.z - p1.z);
 }
 
// check overlappings
 bool Aabb::overlapPoint(vec3 p) {
	 if (abs(p1.x - p.x) < l && abs(p2.x - p.x)<l) {
		 if (abs(p1.y - p.y)<L && abs(p2.y - p.y)<L) {
			 if (abs(p1.z - p.z)<h && abs(p2.z - p.z)<h) {
				 return true;
			 }
		 }
	 }
	 return false;
 }
 bool Aabb::overlapBox(Aabb* a) {
	 if (overlapPoint(a->p1))  return true;
	 if (overlapPoint(a->p1 + vec3(a->l,0,0)))  return true;
	 if (overlapPoint(a->p1 + vec3(a->l, a->L, 0)))  return true;
	 if (overlapPoint(a->p1 + vec3(0, a->L, 0)))  return true;

	 if (overlapPoint(a->p2))  return true;
	 if (overlapPoint(a->p2 - vec3(a->l, 0.0,0)))  return true;
	 if (overlapPoint(a->p2 - vec3(a->l, a->L,0)))  return true;
	 if (overlapPoint(a->p2 - vec3(0, a->L, 0)))  return true;

	 return false;
 }
 
 // draw box
void Aabb::draw() {
	// attention cette fonction <doit> etre entourée de glbegin et glend,
	// je ne le mets pas à l'interieur de cette fonction car trop couteux --> plutot le mettre au début et fin d'une fonction qui
	// affiche toutes les boites
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p1.x + l, p1.y, p1.z);
	glVertex3f(p1.x + l, p1.y + L, p1.z);
	glVertex3f(p1.x, p1.y + L, p1.z);

	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p1.x, p1.y + L, p1.z);
	glVertex3f(p1.x, p1.y + L, p1.z + h);
	glVertex3f(p1.x, p1.y, p1.z + h);

	glVertex3f(p1.x, p1.y, p1.z + h);
	glVertex3f(p1.x + l, p1.y, p1.z + h);
	glVertex3f(p1.x + l, p1.y + L, p1.z + h);
	glVertex3f(p1.x, p1.y + L, p1.z + h);

	glVertex3f(p1.x + l, p1.y, p1.z);
	glVertex3f(p1.x + l, p1.y + L, p1.z);
	glVertex3f(p1.x + l, p1.y + L, p1.z + h);
	glVertex3f(p1.x + l, p1.y, p1.z + h);
}



/*
*	##############################################################################################
*	##############################################################################################
	class node
*/

Node::Node() {};

Node::Node(Triangle *t) {

	child1 = child2 = this;
	parent = NULL;
	box = Aabb(t);
	isLeaf = true;
	update();
}

Node::Node(Node* c1, Node* c2) {

	parent = NULL;
	child1 = c1;
	child2 = c2;

	isLeaf = false;
	update();
}

void Node::update() {
	// si c'est une feuille,reformer la aabb a partir des point du triangle, (box.updatebox)
	// sinon, aabb est formé par les plus petites et plus grandes coordonnées des enfants. (box.unir) 
	if (isLeaf) {
		box.updateBox(); 
		return;
	}
	box.unir(&child1->box, &child2->box);
	
}


/*
*	##############################################################################################
*	##############################################################################################
	class Tree
*/

Tree::Tree(TriangleMesh *myMesh) {

	for (int i = 0; i < myMesh->triangle_list.size(); i++) {
		Triangle* t = &myMesh->triangle_list[i];
		Node* n = new Node(t);
		nodes.push_back(n);
	}

	creerParents(0, nodes.size(), (DEFAULT_WIDTH - 1) * 2, (DEFAULT_HEIGHT - 1), 0);
}

void Tree::creerParents(int i1, int j1, int w, int h, int d) {

	int taille = abs(j1 - i1);
	if (taille < 1) return;

	int first = nodes.size();
	for (int i = 0; i < w; i += 2) {
		for (int j = 0; j < h; j += 1) {

			Node* n = new Node(nodes[i1 + i + j * w], nodes[i1 + i + j * w + 1]);
			nodes[i1 + i + j * w]->parent = n;
			nodes[i1 + i + j * w + 1]->parent = n;

			nodes.push_back(n);
		}
	}
	int last = nodes.size() - 1;
	creerParents(first, last, h, w / 2, d + 1);

}

void Tree::drawNodes(Node* n, int d) {
	// fonction à mettre dans drawBVH
	if (d == 0) { n->box.draw(); return; }

	drawNodes(n->child1, d - 1);
	drawNodes(n->child2, d - 1);
}

void Tree::drawBVH(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	drawNodes(nodes[nodes.size()-1], 6);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void Tree::update() {

	Node* root = nodes[nodes.size() - 1];
	if (SELF_COLLISIONS_ENABLED == 1) {
		calculeAngles();
		checkSelfCollision(root);
	}

	for (Node* n : nodes) n->update();
}

void Tree::draw() {
	//drawBVH2(nodes[nodes.size()=1,0,],)
}

void Tree::calculeAngles() {
	for (Node *n: nodes)
	{
		if (n->isLeaf) {
			n->angle = 0;
			n->axis = n->box.t->normal;
			

		}
		else {
			float beta = acos(dot(n->child1->axis, n->child2->axis));
			n->axis = (n->child1->axis + n->child2->axis) / 2.0f;
			n->angle = beta / 2.0f + std::max(n->child1->angle, n->child2->angle);
		}
		
	}
}


void Tree::checkCollision(Node *a,Node *b){
	if(!a->box.overlapBox(&b->box) && !b->box.overlapBox(&a->box) ){
		// si pas de collision, s'arrêter
		return	;
	}

	if(a->isLeaf && b->isLeaf){

		if (&a->parent != &b->parent) {
			interpolation(a->box.t, b->box.t);
			interpolation(b->box.t, a->box.t);
		}
		return ;
	}
	checkCollision(b->child1,a); 
	checkCollision(b->child2,a);
}
void Tree::checkSelfCollision(Node* n) {
	if (abs(n->angle) > (1)) {
		checkCollision(n->child1, n->child2);
		checkSelfCollision(n->child1);
		checkSelfCollision(n->child2);
	}
	return;
}


/*
*	##############################################################################################
*	##############################################################################################
	 resolution des resolutions
*/


// (bon, la j'ai plus ou moins a peu pres repris le travail de Mr Nabil...)
// triangle point
void Tree::applyImpulse(Triangle *t, myParticle *p, double I, vec3 n,vec3*bary)
{
	double Ih = (2.0 * I) / (1.0 + pow(bary->x,2) + pow(bary->y,2) + pow(bary->z,2));
	t->a_p1->velocity += n * (float) (bary->x * Ih / DEFAULT_MASS);
	t->a_p2->velocity += n * (float)(bary->y* Ih / DEFAULT_MASS);
	t->a_p3->velocity += n * (float)(bary->z * Ih / DEFAULT_MASS);
	p->velocity += -n * (float)(Ih / DEFAULT_MASS);

}

//edge-edge

void Tree::applyImpulse(std::vector<myParticle*> e1, std::vector<myParticle*> e2, double a, double b, double I, vec3 n)
{
	double Ih = (2 * I) / (a * a + (1 - a) * (1 - a) + b * b + (1 - b) * (1 - b));
	e1[0]->velocity += n * (float)((1 - a) * Ih / e1[0]->mass);
	e1[1]->velocity += n * (float)(a * Ih / e1[1]->mass);
	e2[0]->velocity -= n * (float)((1 - b) * Ih / e2[0]->mass);
	e2[1]->velocity -= n * (float)(b * Ih / e2[1]->mass);
} 


//OK
void Tree::interpolation(Triangle *t1, Triangle *t2) {
	// 1: triangle - point.
	myParticle* pt1[] = { t1->a_p1,t1->a_p2,t1->a_p3 };
	myParticle* pt2[] = { t2->a_p1,t2->a_p2,t2->a_p3 };

	/*
	double a, b;
	for (int i = 0; i < 3;i++) {
		for (int j = 0; j < 3; j++) {
			std::vector<myParticle*>  e1 = t1->edges[i], e2 = t2->edges[j];

			if (e1[0] != e2[0] && e1[0] != e2[1] && e1[1] != e2[0] && e1[1] != e2[1])
			{
				float cps = closest_point_segment(e1[0]->position, e1[1]->position,
					e2[0]->position, e2[1]->position, a, b);
				
				if (cps < DEFAULT_H)
				{
						//std::cout << cps << " close ee.\n";
						vec3 cv1 = (e1[0]->velocity) * (float)(1 - a) + (e1[1]->velocity) * (float)a;
						vec3 cv2 = (e2[0]->velocity) * (float)(1 - b) + (e2[1]->velocity) * (float)b;
						vec3 cv12 = cv2 - cv1;
						vec3 n = ((e2[0]->position) * (float)(1 - b) + (e2[1]->position) * (float)b) -
							((e1[0]->position) * (float)(1 - a) + (e1[1]->position) * (float)a);
						double I = dot(n,cv12);
						//std::cout << "Impulse: " << I << std::endl;
						if (I < 0)	applyImpulse(e1, e2, a, b, I * DEFAULT_MASS / 2, n);
					
				}
			}
			
		}
	}
	*/
	
	for (int i = 0; i < 3; i++)
	{
			myParticle* myp = pt2[i];
			// point - triangle 
			if (pt1[0] != myp && pt1[1] != myp && pt1[2] != myp) { // le point pt2[j] ne fait pas partie du triangle 1
				vec3 bary = t1->proximity(myp);

				if ( length(bary) != 0) { // length == 0 <=> null

					//cout << "come on";
					vec3 v5 = (pt1[0]->velocity) * bary.x + (pt1[1]->velocity) * bary.y + (pt1[2]->velocity) * bary.z;
					vec3 v45 =myp->velocity - v5;

					double I = dot(t1->normal,v45);
 
					float d = DEFAULT_H - dot(myp->position - bary.x * t1->a_p1->position - bary.y * t1->a_p2->position - bary.z * t1->a_p3->position, t1->normal);
					
					// j'ajoute la répuslion ( => moins de collisions )
					if (I > 0.1 *d / DT) {

					}
					else {

						float Ir = - std::min(DT * 8.0f * d, DEFAULT_MASS * ((0.1f * d / DT) - I));
						applyImpulse(t1, myp, Ir * (float)DEFAULT_MASS / (float)2, t1->normal, &bary);
							//(1.0f * d / (float)DEFAULT_MASS) * (float)DT;
					}
					if (I < 0) {

						applyImpulse(t1, myp, (float)I * (float)DEFAULT_MASS / (float)2, t1->normal,&bary);


						//cout << "impulse";
						vec3 v5new = (t1->a_p1->velocity) * bary.x + (t1->a_p2->velocity) * bary.y + (t1->a_p3->velocity) * bary.z;
						vec3 v45new = myp->velocity - v5new;

						vec3 vpre = v45 - t1->normal * (dot(t1->normal,v45));
						I = fabs(dot(t1->normal,v45) - dot(t1->normal,v45new));

						if (MU * I > vpre.length()) { // 1=mu, <=> friction
							vpre = normalize(vpre);

							// sinon .. ?
							applyImpulse(t1,myp, I * MU * DEFAULT_MASS, vpre,&bary); // 1=mu, <=> friction
						}
					}
				}
			}

			/*
			myp = pt1[i];
			// point - triangle 
			if (pt2[0] != myp && pt2[1] != myp && pt2[2] != myp) { // le point pt2[j] ne fait pas partie du triangle 1
				vec3 bary = t2->proximity(myp);

				if (length(bary) != 0) { // length == 0 <=> null

					//cout << "come on";
					vec3 v5 = (pt2[0]->velocity) * bary.x + (pt2[1]->velocity) * bary.y + (pt2[2]->velocity) * bary.z;
					vec3 v45 = myp->velocity - v5;

					double I = dot(t2->normal, v45);

					float d = DEFAULT_H - dot(myp->position - bary.x * t2->a_p1->position - bary.y * t2->a_p2->position - bary.z * t2->a_p3->position, t2->normal);

					// j'ajoute la répuslion ( => moins de collisions )
					if (I > 0.1 * d / DT) {

					}
					else {

						float Ir = -std::min(DT * 1.0f * d, DEFAULT_MASS * ((0.1 * d / DT) - I));
						myp->velocity += t2->normal * (float)(Ir) / (float)DEFAULT_MASS * (float)DT;
						//(1.0f * d / (float)DEFAULT_MASS) * (float)DT;
					}
					if (I < 0) {

						applyImpulse(t2, myp, (float)I * (float)DEFAULT_MASS / (float)2, t2->normal, &bary);


						//cout << "impulse";
						vec3 v5new = (t2->a_p1->velocity) * bary.x + (t2->a_p2->velocity) * bary.y + (t2->a_p3->velocity) * bary.z;
						vec3 v45new = myp->velocity - v5new;

						vec3 vpre = v45 - t2->normal * (dot(t2->normal, v45));
						I = fabs(dot(t2->normal, v45) - dot(t2->normal, v45new));

						if (MU * I > vpre.length()) { // 1=mu, <=> friction
							vpre = normalize(vpre);

							// sinon .. ?
							applyImpulse(t2, myp, I * MU * DEFAULT_MASS, vpre, &bary); // 1=mu, <=> friction
						}
					}
				}
			}
			*/
		}
	

}


