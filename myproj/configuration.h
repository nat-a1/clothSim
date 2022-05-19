#pragma once
 
// world
extern float GRAVITY;				// Gravity constant
extern double DT;				// Refreshing step

/*Particles*/
extern double DEFAULT_MASS;		// Mass of the particles
extern double DEF_RATE;		// degré de deformation autorisé


/*Springs*/
extern float DEFAULT_K;			//8		// Stiffness hooke's constant.
extern float M_KD_1;                    // Springs damping (scalaire)
extern float M_KD_2;		//0.01	// smooth springs damping (utilisée)

/*Mass-spring*/
extern int DEFAULT_WIDTH;			// Width of the mass-spring system
extern int DEFAULT_HEIGHT;			// Height of the mass-spring system 
extern float SPACE_P1_TO_P2;		// Space between masses(particles)

extern int EULER;
extern int VERLET;
extern int INTEGRATOR;

extern float DEFAULT_H;

extern float MU;
extern float DEFAULT_THICKNESS;
/*
	n:
	ajout: 
		* verlet integrator
		* contrainte facile de longueur des ressorts
		* correction d'une erreur débile dans le massSpring.cpp (ds initSpring)

*/


///// configurations:
//1: tenu par quelques points au centre
//2: tenu par les extrémités supérieures
extern int CONFIG;
extern int DRAW_BVH;
extern int SELF_COLLISIONS_ENABLED;
