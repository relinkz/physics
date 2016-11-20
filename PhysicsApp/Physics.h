#ifndef PHYSICS_H
#define PHYSICS_H
#include "GlobalData.h"
#include "Body.h"


namespace Physics
{
	void attraction(Body &body1, Body &body2, int time);
	long float calcRadius(const Vector3& v1);
	Vector3 normalizeVector(Vector3 vec);
	float dot(Vector3 vec1, Vector3 vec2);
	Vector3 cross(Vector3 vec1, Vector3 vec2);
	Vector3 vectorOnVectorProjection(Vector3 vec1, Vector3 vec2);
}

#endif
