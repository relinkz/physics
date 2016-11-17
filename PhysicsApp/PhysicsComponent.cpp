#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
	this->pos = Vector3(0, 0, 0);
	this->velocity = Vector3(0, 0, 0);
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update(float dt)
{
	this->pos = this->pos + (this->velocity * dt);
}

void PhysicsComponent::ApplyForce(Vector3 force, float dt)
{
	this->velocity = this->velocity + (force * dt);
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
