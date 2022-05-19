#include "myMassSpring.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mySpring.h"
#include "myShader.h"
#include <vector>

#include <iostream>
#include <math.h>   

#include "Wind.h"
#include <list>

// eigen?
#include <Eigen/Dense>



/// ce QUE j'ai modifié:
/// mymassSpring():
///		* initTriangles() 
///		* makeTree() (dépend du tableau que j'ai fait "triangles", donc ptet remplacer par le tien )
/// 
///	calculateNextPosition():
///		*updateNodes()
///		*epicUnnamedFunction() => c'est la correction par impulsions, mais ça marche pas super, +- correction directe de pos



myMassSpring::myMassSpring(unsigned int width, unsigned int height)
{
	unsigned int i, j;

	//Creating width*height particles.
	particles.assign(width, std::vector<myParticle>());

	// créer les particles
	for (i = 0; i < width; i++)
	{
		particles[i].assign(height, myParticle());
		for (j = 0; j < height; j++)
		{
			if (CONFIG == 4) {
				particles[i][j] = myParticle(static_cast<float>(i * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * width / 2.0f),
					static_cast<float>(j * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * height / 2.0f + 2.0f), 10.0f );
			}
			else {
				particles[i][j] = myParticle(static_cast<float>(i * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * width / 2.0f), 10.0f,
					static_cast<float>(j * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * height / 2.0f + 2.0f));
			}

		}
	}

	// créer les ressorts...
	int profondeurLiens = 3;
	initSprings(width, height,profondeurLiens);

	//création des triangles
	int nbTri = (width - 1) * 2 * (height - 1);
	triangles.assign(nbTri, std::vector<vec3*>());
	//initTriangles();

	myMesh = new TriangleMesh(&particles,width,height);

	// création de la bvh
	//makeTree();
	myTree = new Tree(myMesh);
	
	if (CONFIG == 1) {
		for (i = (int)(0); i < (int)(1); i++) {} // 2
			particles[width / 2 + i][height / 2 + i].movable = false;
	}
	if (CONFIG == 2) {
		//particles[i][width-1].movable = false;
		//for (i = (int)(0); i < 3 ; i++)
			particles[0][0].movable = false;
			particles[0][width-1].movable = false;
	}
	if (CONFIG == 3) {
		for (i = (int)(width/2-1); i < width/2 +1; i++)
			particles[0][i].movable = false;
			particles[width-1][i].movable = false;
	}
	if (CONFIG == 4) {
		//for (i = (int)(0); i < 3; i++)
			//particles[0][0].movable = false;
			//particles[0][width - 1].movable = false;
			//particles[width- 1][0].movable = false;
			//particles[width - 1][height - 1].movable = false;
	}
	// remplir matrix b et a
	/*
	projs.assign(springs.size() - 1,vec3(0,0,0));
	matrix_B = MatrixXf(springs.size() - 1, springs.size() - 1);
	matrix_A = MatrixXf(springs.size() - 1, springs.size() - 1);
	dV = VectorXf(springs.size() - 1);
	for (i = 0; i < springs.size()-1; i++)
	{
		for (j = 0; j < springs.size() - 1; j++)
		{
			myParticle* pi1 = springs[i].p1, *pi2 = springs[i].p2;
			myParticle* pj1 = springs[j].p1, * pj2 = springs[j].p2;
			float ka = 1 / DEFAULT_MASS;

			if (pi1 == pj1 && pi2 != pj2) matrix_B(i, j) = ka;
			else if (pi2 == pj2 && pi1 != pj1) matrix_B(i, j) = ka;
			else if (pi1 == pj2 && pi2 != pj1) matrix_B(i, j) = -ka;
			else if (pi2 == pj1 && pi1 != pj2) matrix_B(i, j) = -ka;
			else if (i == j) matrix_B(i, j) = 2.0f * ka;
			else if (pi1 == pj1 && pi2 != pj2) matrix_B(i, j) = ka;
			else matrix_B(i, j) = 0;
		}
	}
	*/

	//Giving color values.
	kd[0] = 0.8f;		kd[1] = 0.8f;		kd[2] = 1.0f;		kd[3] = 1.0f;
	depth = 2;
	computeNormals();
}

void myMassSpring::clearForces()
{
	for (auto& row : particles) {
		for (auto& col : row) {
			col.force = glm::vec3 (0.0f, 0.0f, 0.0f);

		}
	}
}


void myMassSpring::addForces(Wind wind)
{
	

	glm::vec3 gravity(0.0f, -DEFAULT_MASS * GRAVITY, 0.0f);

	for (auto& row : particles) {
		for (auto& col : row) {
			col.addForce(gravity);

			//if (CONFIG == 4 ) {
				//col.addForce(wind.disneyForce(col));
				//col.addForce(wind.basicForce(col));
			//}
		}
	}
	
	for (auto& sp : springs) {
		sp.addForce();
	}

}

void myMassSpring::calculateNextPosition()
{
	
	
	
	//epicUnnamedFunction();
	
	
	
	//computeMatrixA();
	//for (auto& sp : springs) {
	//	sp.corrigepos();
	//}
	myMesh->calculateNormals();
	myTree->update();

	// compute all candidate positions.
	for (auto& row : this->particles) {
		for (auto& col : row) {
			col.calculateNextPosition();
		}
	}

	// collisions, bvh, repulsion, fin voila etc


	// update all position
	for (auto& row : this->particles) {
		for (auto& col : row) {
			col.updatePosition();
		}
	}



	/*TODO*/
}

void myMassSpring::ballCollision(mySphere* s)
{
	/*TODO: move cloth if collides with ball*/
	badcollisionNat(s);
	
}

void myMassSpring::groundCollision(float floor_y)
{
	float l;
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
		{
			l = (particles[i][j].position.y - floor_y);
			if (l < 0)
				//particles[i][j].position.y = floor_y;
				particles[i][j].movable = false;
		}
}

void myMassSpring::computeNormals()
{
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].normal = glm::vec3(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < particles.size() - 1; i++)
		for (size_t j = 0; j < particles[i].size() - 1; j++)
		{
			glm::vec3 face_normal = glm::cross(particles[i + 1][j].position - particles[i][j].position,
				particles[i + 1][j + 1].position - particles[i + 1][j].position);
			particles[i][j].normal += face_normal;
			particles[i + 1][j].normal += face_normal;
			particles[i + 1][j + 1].normal += face_normal;

			face_normal = glm::cross(particles[i][j + 1].position - particles[i + 1][j + 1].position,
				particles[i][j].position - particles[i][j + 1].position);
			particles[i][j].normal += face_normal;
			particles[i][j + 1].normal += face_normal;
			particles[i + 1][j + 1].normal += face_normal;
		}

	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].normal = glm::normalize(particles[i][j].normal);
}

void myMassSpring::drawSpring()
{
	glBegin(GL_TRIANGLES);
		myMesh->draw();
	glEnd();
}


/* nat functions */
	void myMassSpring::initSprings(unsigned int width, unsigned int height, int profondeurLiens)
	{
		int taille=0;
		int compt=0;

		/* initialiser le tableau:
		   1: liens (horizontaux et verticaux) */
		for (int i=1;i<profondeurLiens;i++){
			taille += (width - profondeurLiens) * (height)+ (width)*(height - profondeurLiens)
				;
		}
		taille += 2 * (width - 1) * (height - 1) ;
		/* 2: liens (diagonales) */

		springs.assign(taille,mySpring());


		/* créer les springs (horizontal et vertical)*/
		for(int n=1;n<profondeurLiens;n++)
		{
			// springs horizontaux 
			for (int i = 0; i < width-profondeurLiens; i++)
			{
				for (int j = 0; j < height; j++)
				{
					springs[compt] = mySpring(&particles[i][j], &particles[i+n][j],DEFAULT_K,n*SPACE_P1_TO_P2);
					compt += 1;
				}
			}
			// springs verticaux
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height-profondeurLiens; j++)
				{
					springs[compt] = mySpring(&particles[i][j], &particles[i][j+n],DEFAULT_K,n*SPACE_P1_TO_P2);
					compt += 1;
				}
			}
			/* créer les springs (diagonales) */
		
		}
		for (int n = 1; n < 2; n++) {
			for (int i = 0; i < width - n; i++)
			{
				for (int j = 0; j < height - n; j++)
				{
					springs[compt] = mySpring(&particles[i][j], &particles[i + n][j + n], DEFAULT_K, sqrt(pow(SPACE_P1_TO_P2 * n, 2.0) * 2.0f));
					springs[compt + 1] = mySpring(&particles[i][j + n], &particles[i + n][j], DEFAULT_K, sqrt(pow(SPACE_P1_TO_P2 *n, 2.0) * 2.0f));

					compt += 2;
				}
			}
		}
	
	
	}

	void myMassSpring::badcollisionNat(mySphere* s) {
		for (size_t i = 0; i < particles.size(); i++) {
			for (size_t j = 0; j < particles[i].size(); j++)
			{
				glm::vec3 dist = s->center - particles[i][j].position;
				if (glm::length(dist) <= s->radius) {
					//particles[i][j].velocity -= dist;
					float erreur = glm::length(dist) - s->radius;
					glm::vec3 impulse = (normalize(dist)/(1.0f/(particles[i][j].mass)))*(erreur/(float)DT);
					particles[i][j].velocity += (impulse)*(1.0f/particles[i][j].mass);
					//s->v += (impulse) * (1.0f / s->mass);
				}
			}
		}
	}

	void myMassSpring::epicUnnamedFunction() {
	
		bool allConstraintsSatisfied = true;
		int j = 0;
		do {
			allConstraintsSatisfied = true;
			// trouver l'erreur par intégration? 
			// ==> calculer la position à partir des vitesses?
			for (int i = 0; i < springs.size(); i++) {
				mySpring s = springs[i];
				float e = s.getConstraint(); //get Error
			
				if (abs(e) >= (DEF_RATE * s.restLength)) { // si l'erreur est supérieure à 10% de la longueur du ressort

					glm::vec3 posdist = normalize(s.p2->position - s.p1->position);
					//glm::vec3 spddist = s.p2->velocity - s.p1->velocity;

					float ka = 0, kb =0; 
					if (s.p1->movable) ka = 1.0f / DEFAULT_MASS;
					if (s.p2->movable) kb = 1.0f / DEFAULT_MASS;

					// compute impulse
					glm::vec3 p = 0.5f*(posdist / (ka + kb)) * (e / (float)DT);
				
					// correction impulses (je ne crois pas..)
					//s.p1->addForce(p); s.p2->addForce(-p);

					// new velocity
					s.p1->velocity += ka*p;
					s.p2->velocity -= kb*p;
					allConstraintsSatisfied = false; j++;
				}
			}
		}while (!allConstraintsSatisfied && j<3);
	
	}

	void myMassSpring::computeMatrixA() {
		
		

		for (int i = 0; i < springs.size() - 1; i++)
		{
			
			for (int j = 0; j < springs.size() - 1; j++)
			{
				vec3 pi = normalize(springs[i].p2->position - springs[i].p1->position)*matrix_B(i,j);
				vec3 pj = projs[i] = normalize(springs[j].p2->position - springs[j].p1->position);
				matrix_A(i, j) = pi.x * pj.x + pi.y*pj.y+pi.z*pj.z;
			}
			dV(i) = springs[i].getConstraint()/(float)DT;
			
		}
		VectorXf p = matrix_A.colPivHouseholderQr().solve(dV);
		for (int i = 0; i < springs.size() - 1; i++)
		{
			springs[i].p1->velocity += (p[i]/ 2.0f)*projs[i] / springs[i].p1->mass;
			springs[i].p2->velocity -= (p[i]/ 2.0f) * projs[i] / springs[i].p2->mass;
		}
	}
	
myMassSpring::~myMassSpring()
{
	
}


