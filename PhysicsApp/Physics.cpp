#include "Physics.h"



//body1 attracts body2
void Physics::attraction(Body & body1, Body & body2, int time)
{
	
	//double tempScale = 0.00000000000001f;
	//tempScale = 1.0f;

	Vector3 dir = (body1.getPosition() - body2.getPosition());
	long float d = calcRadius(dir);
	if (d == 0)
	{
		//that shit is not acceptable
		d = 0.00001;
	}
	long double m1m2 = (body1.getMass()) * (body2.getMass());
	
	long double d2 = pow(d, 2);

	long double f = (G * (m1m2 / d2));

	Vector3 dir2 = normalizeVector(dir);
	float theta = atan2(dir2.y, dir2.x);

	Vector3 force = Vector3(0, 0, 0);
	force.x = cos(theta) * f;
	force.y = sin(theta) * f;

	body2.applyForce(force, time);

}


long float Physics::calcRadius(const Vector3 & v1)
{
	long float result = 0;
	result = abs(v1.x) + abs(v1.y) + abs(v1.z);

	return result;
}
Vector3 Physics::normalizeVector(Vector3 vec)
{
	long float length = calcRadius(vec);

	Vector3 result = vec / length;

	return result;
}