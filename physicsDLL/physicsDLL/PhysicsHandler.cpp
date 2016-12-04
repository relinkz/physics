#include "PhysicsHandler.h"



PhysicsHandler::PhysicsHandler()
{
}
PhysicsHandler::~PhysicsHandler()
{
	//int size = this->m_dynamicComponents.size();
	//for (int i = 0; i < size; i++)
	//{
	//	delete this->m_dynamicComponents.at(i);
	//}
	//delete this->m_ballPC;
	//delete this->m_playerPC;
	//size = this->m_chain.m_links.size();
	//for (int i = 0; i < size; i++)
	//{
	//	delete this->m_chain.m_links.at(i);
	//}
}

bool PhysicsHandler::Initialize()
{
	this->m_gravity = DirectX::XMVectorSet(0, -0.005, 0, 0);
	//this->m_gravity = DirectX::XMVectorSet(0, 0.0, 0, 0);
	
	this->m_dynamicComponents.push_back(PhysicsComponent());

	//this->m_dynamicComponents.push_back(PhysicsComponent());
	//this->m_dynamicComponents.push_back(PhysicsComponent());
	//this->m_dynamicComponents.push_back(PhysicsComponent());
	//this->m_dynamicComponents.push_back(PhysicsComponent());

	this->m_dynamicComponents.push_back(PhysicsComponent());


	this->m_ballPC = &this->m_dynamicComponents.at(0);

	int size = this->m_dynamicComponents.size();

	for (int i = 0; i < size; i++)
	{
		this->m_chain.m_links.push_back(&this->m_dynamicComponents.at(i));
	}

	this->m_playerPC = &this->m_dynamicComponents.at(size - 1);


	//ball physicsComponent
	this->m_ballPC->m_pos = DirectX::XMVectorSet(3, 9, 0, 0);
	this->m_ballPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	this->m_ballPC->m_mass = 10.0;
	this->m_ballPC->m_is_Static = false;
	//player physicsComponent
	this->m_playerPC->m_pos = DirectX::XMVectorSet(0, 9, 0, 0);
	this->m_playerPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	this->m_playerPC->m_mass = 10.0;
	this->m_playerPC->m_is_Static = false;

	int nrOfChainLinkObjects = this->m_chain.m_links.size();
	for (int i = 1; i < nrOfChainLinkObjects - 1; i++)
	{
		if (i != 0 && i != nrOfChainLinkObjects - 1)
		{
			this->m_chain.m_links.at(i)->m_pos = DirectX::XMVectorSet(1.5, 9, 0, 0);
			this->m_chain.m_links.at(i)->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
			this->m_chain.m_links.at(i)->m_mass = 1.0;
			this->m_chain.m_links.at(i)->m_is_Static = false;
		}
	}

	this->m_chain.m_linkLenght = 3.0f;

	this->m_chain.m_totalLenght = this->m_chain.m_linkLenght * this->m_chain.m_links.size();

	this->m_old_time = (GetCurrentTime());

	return true;
}

void PhysicsHandler::Update()
{
	this->m_current_time = (GetCurrentTime());
	float dt = 0.01f;
	dt = (this->m_current_time - this->m_old_time) / 1000;

	int size = this->m_chain.m_links.size();
	for (int i = 0; i < size - 1; i++)
	{
		this->DoChainPhysics(this->m_chain.m_links.at(i), this->m_chain.m_links.at(i + 1), dt);
	}

	size = this->m_dynamicComponents.size();
	for (int i = 0; i < size; i++)
	{
		if (!this->m_dynamicComponents.at(i).m_is_Static)
		{
			this->SimpleCollition(&this->m_dynamicComponents.at(i), dt);
		}
		this->UpdatePhysicsComponent(&this->m_dynamicComponents.at(i), dt);
	}

	this->AdjustChainLinkPosition();

	this->m_old_time = this->m_current_time;
#pragma region input_for_testing

	float str = 1.0f;
	if (GetAsyncKeyState(0x49))
	{
		this->ApplyForceToComponent(this->m_playerPC, DirectX::XMVectorSet(0, str, 0, 0), dt);
	}
	if (GetAsyncKeyState(0x4A))
	{
		this->ApplyForceToComponent(this->m_playerPC, DirectX::XMVectorSet(-str, 0, 0, 0), dt);
	}
	if (GetAsyncKeyState(0x4C))
	{
		this->ApplyForceToComponent(this->m_playerPC, DirectX::XMVectorSet(str, 0, 0, 0), dt);
	}
	if (GetAsyncKeyState(0x4B))
	{
		this->ApplyForceToComponent(this->m_playerPC, DirectX::XMVectorSet(0, -str, 0, 0), dt);
	}
	if (GetAsyncKeyState(0x48))
	{
		int nrOfObjects = this->m_dynamicComponents.size();
		for (int i = 0; i < nrOfObjects; i++)
		{
			this->m_dynamicComponents.at(i).m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		}
	}

	if (GetAsyncKeyState(0x52))
	{
		//ball physicsComponent
		this->m_ballPC->m_pos = DirectX::XMVectorSet(3, 9, 0, 0);
		this->m_ballPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		this->m_ballPC->m_mass = 10;
		this->m_ballPC->m_is_Static = false;
		//player physicsComponent
		this->m_playerPC->m_pos = DirectX::XMVectorSet(0, 9, 0, 0);
		this->m_playerPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		this->m_playerPC->m_mass = 10;
		this->m_playerPC->m_is_Static = false;

		int nrOfChainLinkObjects = this->m_chain.m_links.size();
		for (int i = 1; i < nrOfChainLinkObjects - 1; i++)
		{
			if (i != 0 && i != nrOfChainLinkObjects - 1)
			{
				this->m_chain.m_links.at(i)->m_pos = DirectX::XMVectorSet(0, 9, 0, 0);
				this->m_chain.m_links.at(i)->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
				this->m_chain.m_links.at(i)->m_mass = 10.0;
				this->m_chain.m_links.at(i)->m_is_Static = false;
			}
		}
	}
#pragma endregion
}

void PhysicsHandler::SimpleCollition(PhysicsComponent* componentPtr, float dt)
{
	float m_offSet = 0.5f;
	float m_frictionConstant = 0.99f;
	PhysicsComponent* ptr = componentPtr;
	int size = this->m_dynamicComponents.size();

	DirectX::XMVECTOR pos = ptr->m_pos;

	float y = DirectX::XMVectorGetY(pos);
	float x = DirectX::XMVectorGetX(pos);

	float groundlevel = 0.0;
	if (y > (groundlevel + m_offSet))
	{
		SimpleGravity(ptr, dt);
	}
	else if (y < (groundlevel + m_offSet))
	{
		ptr->m_pos =  (DirectX::XMVectorSet(DirectX::XMVectorGetX(pos), (groundlevel + m_offSet), DirectX::XMVectorGetZ(pos), 0.0f));
		DirectX::XMVECTOR vel = ptr->m_velocity;
		ptr->m_velocity = (DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, 0, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
	}
	else if (y == (groundlevel + m_offSet))
	{
		DirectX::XMVECTOR vel = ptr->m_velocity;
		ptr->m_velocity = (DirectX::XMVectorSet(DirectX::XMVectorGetX(vel) * m_frictionConstant, DirectX::XMVectorGetY(vel) * m_frictionConstant, DirectX::XMVectorGetZ(vel) * m_frictionConstant, 0.0f));
	}

}

void PhysicsHandler::SimpleGravity(PhysicsComponent* componentPtr, const float &dt)
{
	DirectX::XMVECTOR test = DirectX::XMVECTOR();
	DirectX::XMFLOAT3 testRes(0, 5, 0);
	test = DirectX::XMLoadFloat3(&testRes);


	//componentPtr->m_velocity = DirectX::XMVectorAdd(componentPtr->m_velocity, this->m_gravity);
	this->ApplyForceToComponent(componentPtr, DirectX::XMVectorScale(this->m_gravity, componentPtr->m_mass), dt);

}

void PhysicsHandler::UpdatePhysicsComponent(PhysicsComponent * componentPtr, float dt)
{
	if (componentPtr->m_is_Static)
	{
		componentPtr->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	}
	componentPtr->m_velocity = DirectX::XMVectorScale(componentPtr->m_velocity, 0.9999);
	componentPtr->m_pos = DirectX::XMVectorAdd(componentPtr->m_pos, DirectX::XMVectorScale(componentPtr->m_velocity, dt));
}

void PhysicsHandler::ApplyForceToComponent(PhysicsComponent * componentPtr, DirectX::XMVECTOR force, float dt)
{
	if (!componentPtr->m_is_Static)
	{
		float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(force));
		if (lenght > 25)
		{
			int error = 0;
		}
		float scale = (1.0 / componentPtr->m_mass);
		componentPtr->m_velocity = DirectX::XMVectorAdd(componentPtr->m_velocity, DirectX::XMVectorScale(force, scale));
	}

}

void PhysicsHandler::DoChainPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt)
{ 

	this->ChainMomentumPhysics(current, next, dt);


}

void PhysicsHandler::AdjustChainLinkPosition()
{
	int size = this->m_chain.m_links.size();;
	PhysicsComponent* current;
	PhysicsComponent* next;
	for (int i = 0; i < size - 1; i++)
	{
		current = this->m_chain.m_links.at(i);
		next = this->m_chain.m_links.at(i + 1);
		DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(current->m_pos, next->m_pos);
		diffVec = DirectX::XMVectorSubtract(next->m_pos, current->m_pos);
		float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));
		diffVec = DirectX::XMVector3Normalize(diffVec);

		if (lenght > this->m_chain.m_linkLenght)
		{
			next->m_pos = DirectX::XMVectorAdd(current->m_pos, DirectX::XMVectorScale(diffVec, (this->m_chain.m_linkLenght)));
		}
	}
}


int PhysicsHandler::getNrOfComponents()const
{
	return this->m_dynamicComponents.size();
}
PhysicsComponent* PhysicsHandler::getDynamicComponent(int index)
{
	if (index >= 0 && index < this->m_dynamicComponents.size())
	{
		PhysicsComponent* ptr = &this->m_dynamicComponents.at(index);
		return ptr;
	}
	return nullptr;

}

PhysicsComponent * PhysicsHandler::getPlayerComponent() const
{
	return this->m_playerPC;
}

PhysicsComponent * PhysicsHandler::getBallComponent() const
{
	return this->m_ballPC;
}

void PhysicsHandler::SpringJointPhysics(PhysicsComponent * current, PhysicsComponent * next, float dt)
{
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(current->m_pos, next->m_pos);
	diffVec = DirectX::XMVectorSubtract(next->m_pos, current->m_pos);
	float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));
	if (lenght >= this->m_chain.m_linkLenght)
	{

		diffVec = DirectX::XMVector3Normalize(diffVec);

		DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, lenght - this->m_chain.m_linkLenght), 1.0);
		//force = DirectX::XMVectorAdd(force, DirectX::XMVectorScale(DirectX::XMVectorSubtract(current->m_velocity, next->m_velocity), 1.0));

		this->ApplyForceToComponent(current, force, dt);
		force = DirectX::XMVectorScale(force, -1.0);
		this->ApplyForceToComponent(next, force, dt);
	}
}

void PhysicsHandler::ChainMomentumPhysics(PhysicsComponent * current, PhysicsComponent * next, float dt)
{
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(current->m_pos, next->m_pos);
	diffVec = DirectX::XMVectorSubtract(next->m_pos, current->m_pos);
	float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));
	if (lenght >= this->m_chain.m_linkLenght)
	{

		diffVec = DirectX::XMVector3Normalize(diffVec);

		next->m_pos = DirectX::XMVectorAdd(current->m_pos, DirectX::XMVectorScale(diffVec, (this->m_chain.m_linkLenght * 1.0)));

		float m1 = current->m_mass;
		float m2 = next->m_mass;

		DirectX::XMVECTOR velocityVec1 = current->m_velocity;
		DirectX::XMVECTOR velocityVec2 = next->m_velocity;

		//DirectX::XMVECTOR parallelVelocityVec1;
		//DirectX::XMVECTOR parallelVelocityVec2;
		//DirectX::XMVECTOR perpendicularVelocityVec1;
		//DirectX::XMVECTOR perpendicularVelocityVec2;

		//DirectX::XMVector3ComponentsFromNormal(&parallelVelocityVec1, &perpendicularVelocityVec1, velocityVec1, diffVec);
		//DirectX::XMVector3ComponentsFromNormal(&parallelVelocityVec2, &perpendicularVelocityVec2, velocityVec2, diffVec);

		float v1_old = DirectX::XMVectorGetX(DirectX::XMVector3Length(velocityVec1));
		float v2_old = DirectX::XMVectorGetX(DirectX::XMVector3Length(velocityVec2));


		float v1_new = (((m1 - m2) / (m1 + m2)) * v1_old) + (((2 * m1) / (m1 + m2))* v2_old);

		float forceMagnitude1 = ((current->m_mass * (v1_new - v1_old)) / 5.0);
		DirectX::XMVECTOR forceVec1 = DirectX::XMVectorScale(diffVec, (forceMagnitude1));
		this->ApplyForceToComponent(current, forceVec1, dt);


		float v2_new = (((2 * m1) / (m1 + m2)) * v1_old) + (((m2 - m1) / (m1 + m2))* v2_old);

		float forceMagnitude2 = ((next->m_mass * (v2_new - v2_old)) / 5.0);
		DirectX::XMVECTOR forceVec2 = DirectX::XMVectorScale(diffVec, (forceMagnitude2));

		this->ApplyForceToComponent(next, forceVec2, dt);
	}
}


//bool PhysicsHandler::SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt)
//{
//	float m_frictionConstant = 0.99f; //this one is temporary
//	float m_bounsiness = 0.91f; // also temporary
//
//	bool result = false;
//
//	DirectX::XMVECTOR pos = pComponent->GetPos();
//	
//	DirectX::XMVECTOR toObjectVec = DirectX::XMVectorSubtract(pos, plane.m_pos);
//	
//	
//	float angle = DirectX::XMVectorGetIntX(DirectX::XMVector3Dot(toObjectVec, plane.m_normal));
//
//	
//	if (angle > 0.0f)
//	{
//
//		DirectX::XMVECTOR pParallel;
//		DirectX::XMVECTOR pPurpendicular;
//		DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPurpendicular, toObjectVec, plane.m_normal);
//
//		float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(pPurpendicular));
//
//		if (lenght <= radius)
//		{
//			DirectX::XMVECTOR pointOnPlane = DirectX::XMVectorAdd(plane.m_pos, pParallel);
//
//			result = PointInTriangleTest(pointOnPlane, plane.topLeft);
//			if (!result)
//			{
//				result = PointInTriangleTest(pointOnPlane, plane.bottomRight);
//			}
//			
//			if (result)
//			{
//				//if (lenghtToPlane < radius)
//				//{
//				//	pComponent->SetPos(plane.m_pos + (vecOnPlane)+(plane.m_normal * radius));
//				//}
//				//
//				//Vector3 vel = (pComponent->GetVelocity());
//				//
//				//
//				//
//				//Vector3 reflectedVel = vel - ((plane.m_normal * Physics::dot(vel, plane.m_normal)) * 2) * m_bounsiness;
//				//
//				//pComponent->SetVelocity(Vector3(reflectedVel.x * m_frictionConstant, reflectedVel.y * m_frictionConstant, reflectedVel.z * m_frictionConstant));
//			}
//	
//	
//			//pComponent->SetPos(plane.m_pos + (vecOnPlane) + (plane.m_normal * radius));
//			//
//			//Vector3 vel = pComponent->GetVelocity();
//			//
//			//pComponent->SetVelocity(Vector3(vel.x * m_frictionConstant, 0, vel.z * m_frictionConstant));
//			//
//			//result = true;
//	
//		}
//	}
//	
//	if (!result)
//	{
//		SimpleGravity(pComponent, dt);
//	}
//
//
//
//	return result;
//}
//bool PhysicsHandler::PointInTriangleTest(DirectX::XMVECTOR point, Triangle triangle)
//{
//	bool result = false;
//	//one example 
//	//http://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle
//
//
//	//example used
//	//http://math.stackexchange.com/questions/544946/determine-if-projection-of-3d-point-onto-plane-is-within-a-triangle
//
//
//
//	DirectX::XMVECTOR u = DirectX::XMVectorSubtract(triangle.m_pos2, triangle.m_pos1);
//
//	DirectX::XMVECTOR v = DirectX::XMVectorSubtract(triangle.m_pos3, triangle.m_pos1);
//
//	DirectX::XMVECTOR n = DirectX::XMVector3Cross(u, v);
//
//	DirectX::XMVECTOR w = DirectX::XMVectorSubtract(point, triangle.m_pos1);
//
//	float y = DirectX::XMVectorGetX(DirectX::XMVectorScale(DirectX::XMVector3Dot(DirectX::XMVector3Cross(u, w), n), pow(DirectX::XMVectorGetX(DirectX::XMVector3Length(n)), 2)));
//
//	float b = DirectX::XMVectorGetX(DirectX::XMVectorScale(DirectX::XMVector3Dot(DirectX::XMVector3Cross(w, v), n), pow(DirectX::XMVectorGetX(DirectX::XMVector3Length(n)), 2)));
//
//	float c = 1 - y - b;
//
//	if (c >= 0 && c <= 1.0f && b >= 0 && b <= 1.0f && y >= 0 && y <= 1.0f)
//	{
//		result = true;
//	}
//
//
//	return result;
//}