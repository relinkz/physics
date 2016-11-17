#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
	this->pos = Vector3(0, 0, 0);
	this->velocity = Vector3(0, 0, 0);
}

PhysicsComponent::~PhysicsComponent()
{
}

Vector3 PhysicsComponent::getPos() const
{
	return this->pos;
}

Vector3 PhysicsComponent::getVelocity() const
{
	return this->velocity;
}

void PhysicsComponent::setPos(Vector3 pos)
{
	this->pos = pos;
}

void PhysicsComponent::setVelocity(Vector3 vel)
{
	this->velocity = vel;
}
