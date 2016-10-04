#ifndef BODY_H
#define BODY_H
#include "GlobalData.h"


class Body
{
private:
	std::string name;
	Vector3 Velocity;
	float Mass;
	float Size;

public:
	Body();
	virtual ~Body();
};

#endif

