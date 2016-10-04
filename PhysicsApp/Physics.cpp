#include "Physics.h"


void Physics::doPhysics(Body &body1, Body &body2)
{
	const float G = 6.674 * pow(10,-11);
	long float distance = 0;

	//G

	//const Vector3 diffVector = body1.getPosition() - body2.getPosition();
	//distance = calcRadius(diffVector);
	distance = 384400000; //m



	//do c00l stuff
	//F = G* ((m.b1 * mb2) / distance^2)

	double force;
	long float result;

	float m1m2 = body1.getMass() * body2.getMass();
	
	result = m1m2 / pow(distance, 2);
	result *= G;

	force = result;
}

float Physics::calcSpeed(/*Body body*/)
{
	float result = 0;
	
	//result += abs(body.getVelocity().x);
	//result += abs(body.getVelocity().y);
	//result += abs(body.getVelocity().z);

	return result;
}

float Physics::calcRadius(const Vector3 & v1)
{
	float result = 0;
	result = abs(v1.x) + abs(v1.y) + abs(v1.z);

	return result;
}