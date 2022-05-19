#pragma once
#include "configuration.h"
#include "myParticle.h"
#include "mySpring.h"
#include "mySphere.h"
#include "PerlinNoise.hpp"


#include "Wind.h"
#include "bvh.h"
#include "TriangleMesh.h"
#include <vector>
#include <list>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class myMassSpring
{
public:
	std::vector< std::vector<myParticle> > particles;
	std::vector<mySpring> springs;

	// nat modifs
		std::vector< std::vector<vec3*> > triangles;
		std::vector<vec3> projs;

		//std::vector< Node* > nodes;
		Tree *myTree;

		int depth;
		siv::PerlinNoise mynoise;
		TriangleMesh *myMesh;
		
		// r�so* de SLE
		MatrixXf matrix_B,matrix_A;
		VectorXf dV;
	
	glm::vec4 kd;

	myMassSpring(unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_WIDTH);

	void clearForces();

	void addForces(Wind wind);

	void calculateNextPosition();

	void ballCollision(mySphere *s);

	void groundCollision(float floor_y);

	void computeNormals();

	void drawSpring();


	// fonctions nat:

			/*initialise les ressorts. */
			void initSprings(unsigned int width, unsigned int height, int profondeurLiens);

			/* mauvaise impl�mentation des collisions, (avec des sph�res)*/
			void badcollisionNat(mySphere* s);

			/* respect des contraintes de position par applications d'impulsions
			(changement instantan� dans la vitesse, mais pas dans la position, 
			changement de vitesse � l'instant t pour la correction de la position � l'instant t+h)
			*/
			void epicUnnamedFunction();

			/* fonction temporaire 
				initialise le tableau (temporaire) leaves de bounding boxes pour les triangles*/
			void makeTree();

			/*remplit le tab de triangles (utile pour les bounding boxes j'imagine)*/
			void initTriangles();

			/*dessiner les bounding boxes, r�cursif */
			void drawNodes(Node *n, int depth);
			void updateNodes();
			
			// affiche la bvh en entier ( appuyer sur F1 et F2 pour voir une profondeur au dessus/dessous)
			void drawBVH(); 

			// affiche quels noeuds sont explor�s pour la collision
			void drawBVH2(Node *n, int depth, mySphere *s); 

			// cr�e l'arbre � partir des feuilles cr��es dans "makeTree", �tage par �tage. ne marche que pour un bout de tissu rectangulaire,
			// attention, il faut pour que ca marche que les hauteurs et lageurs soient (puissance de 2) +1 (ouais, OK d�so je changerai)
			void creerParents(int i, int j, int w, int h, int d); 
			
			//////////////////////// pour r�soudre toutes les contraintes en une seule fois:
			void computeMatrixA();



	~myMassSpring();
};

