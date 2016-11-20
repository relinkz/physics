#ifndef BASICWINDOW_PHYSICS_PHYSICSCOMPONENT_H
#define BASICWINDOW_PHYSICS_PHYSICSCOMPONENT_H
#include "GlobalData.h"

class PhysicsComponent
{
private:
	Vector3 m_pos;
	Vector3 m_velocity;
	float m_mass;
	
	DirectX::XMFLOAT3 m_vel;
public:
	PhysicsComponent();
	~PhysicsComponent();

	void Update(const float &dt);

	Vector3 GetPos()const;
	Vector3 GetVelocity()const;
	float GetMass()const;

	void SetPos(Vector3 pos);
	void SetVelocity(Vector3 vel);
	void SetMass(float mass);

	void ApplyForce(Vector3 force, const float &dt);
	void ApplyVelocity(Vector3 velocity, float dt);
};

#endif

