#ifndef PHYSICS_H
#define PHYSICS_H
#include "GlobalData.h"
#include "Body.h"


namespace Physics
{
	void atracttion(Body &body1, Body &body2);
	void doPhysics(Body &body1, Body &body2);
	float calcSpeed(/*Body body*/);
	long float calcRadius(const Vector3& v1);
	Vector3 normalizeVector(Vector3 vec);
}

#endif
