#include "myParticle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>   
#include <iostream>


myParticle::myParticle()
{
	movable = true;
}

myParticle::myParticle(float x1, float y1, float z1, float inputmass)
{
	force = glm::vec3(0.0f, 0.0f, 0.0f);

	position = glm::vec3(x1, y1, z1);
	old_position = next_position  = position ;

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	old_velocity = next_velocity = velocity ;

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	old_acceleration = acceleration;

	mass = inputmass;
	movable = true;
}

void myParticle::addForce(glm::vec3 f)
{
	this->force += f;
}

void myParticle::calculateNextPosition()
{
	acceleration = force / mass;
	if (!movable) return;
	if (INTEGRATOR == EULER) {
			//velocity = velocity + acceleration * (float)DT;
			//position = position + velocity * (float)DT;
			//old_position = position;
			//old_velocity = velocity;

			next_velocity = velocity + acceleration * (float)DT;
			next_position = position + next_velocity * (float)DT;
			avg_velocity = (next_position - position) / (float)DT;
			return;
		}
	if (INTEGRATOR == VERLET) {
			//position = position + velocity * (float)DT + old_acceleration *(float)(DT * DT * 0.5);
			//velocity = velocity + (old_acceleration + acceleration) * (float)(DT * 0.5);

			//old_position = position;
			//old_velocity = velocity;
			//old_acceleration = acceleration;		

			next_position = position + velocity * (float)DT + old_acceleration * (float)(DT * DT * 0.5);
			next_velocity = velocity + (old_acceleration + acceleration) * (float)(DT * 0.5);
			avg_velocity = (next_position - position) / (float)DT;
			return;	
	}	
}
glm::vec3 myParticle::getNextPosition() {
	glm::vec3 nextpos = position + velocity * (float)DT;
	return nextpos;
}
void myParticle::computeNewVelocity() {
	acceleration = force / mass;
	//velocity = velocity + (old_acceleration + acceleration) * (float)(DT * 0.5);
}

void myParticle::updatePosition() {
	
	if (!movable) return;

	

	position = position+ next_velocity*(float)DT ;
	old_position = position;

	velocity = next_velocity;
	old_velocity = velocity;

	old_acceleration = acceleration;
}
myParticle::~myParticle()
{

}
