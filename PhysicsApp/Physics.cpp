#include "Physics.h"



//body1 attracts body2
void Physics::atracttion(Body & body1, Body & body2)
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

	body2.applyForce(force);

}

//body1 acts on body2
void Physics::doPhysics(Body &body1, Body &body2)
{
	////float SCALE = 0.0000001f;

	////const float G = 6.674 * pow(10,-11) * SCALE;
	//long float distance = 0;

	////G

	//Vector3 diffVector = body1.getPosition() - body2.getPosition();
	//diffVector = normalizeVector(diffVector);
	////distance = calcRadius(diffVector);
	//distance = 384400000.0f * SCALE; //m



	////do c00l stuff
	////F = G* ((m.b1 * mb2) / distance^2)

	//double force;
	//long float result;

	//float m1m2 = (body1.getMass() * SCALE) * (body2.getMass() * SCALE);
	//
	//result = m1m2 / pow(distance, 2);
	//result *= G;

	//force = result;

	//diffVector = diffVector * force;

	//body2.applyForce(diffVector);
}

float Physics::calcSpeed(/*Body body*/)
{
	float result = 0;
	
	//result += abs(body.getVelocity().x);
	//result += abs(body.getVelocity().y);
	//result += abs(body.getVelocity().z);

	return result;
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