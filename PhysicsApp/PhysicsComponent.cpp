#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
	this->m_pos = Vector3(0, 0, 0);
	this->m_vel = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);
	this->m_velocity = Vector3(0, 0, 0);
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update(const float &dt)
{
	this->m_pos = this->m_pos + (this->m_velocity * dt);
}

void PhysicsComponent::ApplyForce(Vector3 force, const float &dt)
{
	this->m_velocity = this->m_velocity + (force * dt);
}

Vector3 PhysicsComponent::GetPos() const
{
	return this->m_pos;
}

Vector3 PhysicsComponent::GetVelocity() const
{
	return this->m_velocity;
}

void PhysicsComponent::SetPos(Vector3 pos)
{
	this->m_pos = pos;
}

void PhysicsComponent::SetVelocity(Vector3 vel)
{
	this->m_velocity = vel;
}
