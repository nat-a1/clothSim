#include "mySphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

mySphere::mySphere(glm::vec3 s_center, float s_radius, glm::vec4 rgba, float s_velocity)
	:center(s_center), radius(s_radius), kd(rgba), velocity(s_velocity)
{
	mass = 10.0f;
	p = v = a = vec3(0,0,0);
	p = center;
	//v.x = velocity*200;
	//v.y = -1.0f;
}

mySphere::~mySphere()
{
}

void mySphere::translate(glm::vec3 t)
{
	//v.y += GRAVITY / mass * (float)DT;
	//center -= v * (float)DT;
	
}