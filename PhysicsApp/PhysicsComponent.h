#ifndef BASICWINDOW_PHYSICS_PHYSICSCOMPONENT_H
#define BASICWINDOW_PHYSICS_PHYSICSCOMPONENT_H
#include "GlobalData.h"

class PhysicsComponent
{
private:
	Vector3 pos;
	Vector3 velocity;
public:
	PhysicsComponent();
	~PhysicsComponent();

	void Update(float dt);

	void ApplyForce(Vector3 force, float dt);

	Vector3 getPos()const;
	Vector3 getVelocity()const;

	void setPos(Vector3 pos);
	void setVelocity(Vector3 vel);

};

#endif

