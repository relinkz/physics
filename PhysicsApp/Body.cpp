#include "Body.h"



Body::Body()
{
	this->name = "unknown";
	this->velocity = Vector3(0.0f, 0.0f, 0.0f);
	this->position = Vector3(0.0f, 0.0f, 0.0f);
	this->mass = 0.0f;
	this->size = 0.0f;
	this->bodyModel = nullptr;
	this->SRV = nullptr;
	
}

Body::Body(Model* src, const Vector3& pos)
{
	this->name = "unknown";
	this->velocity = Vector3(0.0f, 0.0f, 0.0f);
	this->position = pos;
	this->mass = 0.0f;
	this->size = 0.0f;
	this->bodyModel = src;
	this->SRV = nullptr;
}

Body::~Body()
{
	if (this->SRV != nullptr)
	{
		//this->SRV->Release();
	}
}

void Body::update(int time) //time
{
	//s = v *     s = v  * t    
	this->position = this->position + (this->velocity* time); // *t

}
void Body::applyForce(Vector3 & force, int time)
{
	//v = v+(f/(m*s))
	this->velocity = this->velocity + (force / this->mass) * time;
}
#pragma region get functions
std::string Body::getName() const
{
	return this->name;
}

Vector3 Body::getVelocity() const
{
	return this->velocity;
}

Vector3 Body::getPosition() const
{
	return this->position;
}

long float Body::getMass() const
{
	return this->mass;
}

float Body::getSize() const
{
	return this->size;
}

Model * Body::getModelAdress()
{
	return this->bodyModel;
}
ID3D11ShaderResourceView* Body::getSRV()
{
	return this->SRV;
}
#pragma endregion

#pragma region set functions
void Body::setName(const std::string & newName)
{
	this->name = newName;
}

void Body::setVelocity(const Vector3 & newVelocity)
{
	this->velocity = newVelocity;
}

void Body::setPosition(const Vector3 & newPosition)
{
	this->position = newPosition;
}

void Body::setMass(const float & newMass)
{
	this->mass = newMass;
}

void Body::setSize(const float & newSize)
{
	this->size = newSize;
}

void Body::setModelAdress(Model * newAdress)
{
	this->bodyModel = newAdress;
}
void Body::setSRV(ID3D11ShaderResourceView *newSRV)
{
	this->SRV = newSRV;
}
#pragma endregion



