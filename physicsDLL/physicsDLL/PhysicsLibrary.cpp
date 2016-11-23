#include "PhysicsLibrary.h"

namespace PhysicsLibrary
{
#pragma region Functions
	void PhysicsLibrary::Fuctions::Gravity(DirectX::XMVECTOR &force, const float &dt)
	{
		DirectX::XMFLOAT3 alteredForce;
		//store XMVector to XMFloat
		DirectX::XMStoreFloat3(&alteredForce, force);

		alteredForce.x *= dt;
		alteredForce.y *= dt;
		alteredForce.z *= dt;

		//load back to XMVector 
		force = DirectX::XMLoadFloat3(&alteredForce);
	}

	void PhysicsLibrary::Fuctions::_DCollition()
	{
		bool collided = false;

		DirectX::XMFLOAT3 pos1(0, 0, 0);
		DirectX::XMFLOAT3 ext1(2, 2, 2);

		DirectX::XMFLOAT3 pos2(3, 0, 0);
		DirectX::XMFLOAT3 ext2(2, 2, 2);

		AABB box1(pos1, ext1);
		AABB box2(pos2, ext2);

		collided = box1.IntersectAABB(box2);
		//OBB vs OBB
	}
#pragma endregion

#pragma region PhysicsComponent

	PhysicsComponent::PhysicsComponent()
	{
		this->m_pos = DirectX::XMVectorSet(0, 0, 0, 0);
		this->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	}
	PhysicsComponent::~PhysicsComponent()
	{

	}

	void PhysicsComponent::Update(const float &dt)
	{

		float x = DirectX::XMVectorGetX(this->m_pos);
		float y = DirectX::XMVectorGetY(this->m_pos);
		float z = DirectX::XMVectorGetZ(this->m_pos);

		float velX = DirectX::XMVectorGetX(this->m_velocity);
		float velY = DirectX::XMVectorGetY(this->m_velocity);
		float velZ = DirectX::XMVectorGetZ(this->m_velocity);


		x += velX * dt;
		y += velY * dt;
		z += velZ * dt;

		this->m_pos = DirectX::XMVectorSet(x, y, z, 0.0f);



		//this->m_pos = DirectX::XMVectorAdd(this->m_pos, DirectX::XMVectorScale(this->m_velocity, dt));
	}

	DirectX::XMVECTOR PhysicsComponent::GetPos()const
	{
		return this->m_pos;
	}
	DirectX::XMVECTOR PhysicsComponent::GetVelocity()const
	{
		return this->m_velocity;
	}
	float PhysicsComponent::GetMass()const
	{
		return this->m_mass;
	}

	void PhysicsComponent::SetPos(DirectX::XMVECTOR pos)
	{
		float x = DirectX::XMVectorGetX(pos);
		float y = DirectX::XMVectorGetY(pos);
		float z = DirectX::XMVectorGetZ(pos);

		this->m_pos =  DirectX::XMVectorSet(x, y, z, 0.0f);

		//this->m_pos = DirectX::XMVectorEqual(this->m_pos, pos);
	}
	void PhysicsComponent::SetVelocity(DirectX::XMVECTOR vel)
	{
		float x = DirectX::XMVectorGetX(vel);
		float y = DirectX::XMVectorGetY(vel);
		float z = DirectX::XMVectorGetZ(vel);

		this->m_velocity = DirectX::XMVectorSet(x, y, z, 0.0f);

		//this->m_velocity = DirectX::XMVectorEqual(this->m_velocity, vel);
	}
	void PhysicsComponent::SetMass(float mass)
	{
		this->m_mass = mass;
	}

	void PhysicsComponent::ApplyForce(DirectX::XMVECTOR force, const float &dt)
	{
		this->m_velocity = DirectX::XMVectorAdd(this->m_velocity, DirectX::XMVectorScale(force,( 1 / this->m_mass) * dt ));
	}

#pragma endregion


#pragma region PhysicsHandler
	
	PhysicsHandler::PhysicsHandler()
	{
		this->m_nrOfComponents = 0;
		this->m_capacity = 0;
	}
	PhysicsHandler::~PhysicsHandler()
	{
		delete[] this->m_components;
	}

	bool PhysicsHandler::Initialize()
	{
		this->m_gravity = DirectX::XMVectorSet(0, -0.5, 0, 0);

		this->m_capacity = 10;
		this->m_nrOfComponents = 2;
		this->m_components = new PhysicsComponent[this->m_capacity];

		this->m_components[0].SetPos(DirectX::XMVectorSet(0, 2.5, 0, 0));
		this->m_components[1].SetPos(DirectX::XMVectorSet(1, 2.5, 0, 0));

		return true;
	}
	void PhysicsHandler::Update()
	{
		float dt = 0.01f;

		SimpleCollition(dt);
	}

	void PhysicsHandler::SimpleCollition(float dt)
	{
		float m_frictionConstant = 0.999f;
		PhysicsComponent* ptr;

		for (int i = 0; i < this->m_nrOfComponents; i++)
		{
			ptr = &this->m_components[i];


			DirectX::XMVECTOR pos = ptr->GetPos();
			
			float y = DirectX::XMVectorGetY(pos);
			
			if (y > (0 + this->m_offSet))
			{
				SimpleGravity(ptr ,dt);
			}
			else if (y < (0 + this->m_offSet))
			{
				ptr->SetPos(DirectX::XMVectorSet(DirectX::XMVectorGetX(pos), (0 + this->m_offSet), DirectX::XMVectorGetZ(pos), 0.0f));
				DirectX::XMVECTOR vel = ptr->GetVelocity();
				ptr->SetVelocity(DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
			}
			else if (y == (0 + this->m_offSet))
			{
				DirectX::XMVECTOR vel = ptr->GetVelocity();
				ptr->SetVelocity(DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0.0f, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
			}
			ptr->Update(dt);
		}
	}
	void PhysicsHandler::SimpleGravity(PhysicsComponent* componentPtr, const float &dt)
	{
		DirectX::XMVECTOR test = DirectX::XMVECTOR();
		DirectX::XMFLOAT3 testRes(0, 5, 0);
		test = DirectX::XMLoadFloat3(&testRes);

		componentPtr->ApplyForce(DirectX::XMVectorScale(this->m_gravity, componentPtr->GetMass()), dt);
	}

	int PhysicsHandler::getNrOfComponents()const
	{
		return this->m_nrOfComponents;
	}

	PhysicsComponent* PhysicsHandler::getPComponents()const
	{
		return this->m_components;
	}

#pragma endregion
}


