#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H
#include "GlobalData.h"

class PhysicsComponent
{
private:
	Vector3 pos;
	Vector3 velocity;
public:
	PhysicsComponent();
	~PhysicsComponent();

	Vector3 getPos()const;
	Vector3 getVelocity()const;

	void setPos(Vector3 pos);
	void setVelocity(Vector3 vel);

};

#endif

