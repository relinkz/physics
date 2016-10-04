#ifndef PHYSICS_H
#define PHYSICS_H
#include "GlobalData.h"
#include "Body.h"


namespace Physics
{
	void doPhysics(Body &body1, Body &body2);
	float calcSpeed(/*Body body*/);
	float calcRadius(const Vector3& v1);
}

#endif
