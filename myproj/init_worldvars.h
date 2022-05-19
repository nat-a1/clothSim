#pragma once
#include "configuration.h"

// world
float GRAVITY = 1;				// Gravity constant
double DT = 0.01;				// Refreshing step

/*Particles*/
double DEFAULT_MASS = 0.01;		// Mass of the particles
double DEF_RATE = 0.01;		// degré de deformation autorisé


/*Springs*/
float DEFAULT_K = 8;			//8		// Stiffness hooke's constant.
float M_KD_1 = 0;                     // Springs damping (scalaire)
float M_KD_2 = 0.01;			//0.01	// smooth springs damping (utilisée)

/*Mass-spring*/
int DEFAULT_WIDTH = 33;			// Width of the mass-spring system
int DEFAULT_HEIGHT = 33;				// Height of the mass-spring system 
float SPACE_P1_TO_P2 = 0.5;	// Space between masses (particles)

int EULER = 0;
int VERLET = 1;
int INTEGRATOR = EULER;

float DEFAULT_H = 0.2f;

float MU = 1;
float DEFAULT_THICKNESS = 0.2f;
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
int CONFIG = 2;
int DRAW_BVH = 0;
int SELF_COLLISIONS_ENABLED = true;
