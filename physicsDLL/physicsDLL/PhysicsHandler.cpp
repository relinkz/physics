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

	this->m_dynamicComponents.push_back(PhysicsComponent());
	this->m_dynamicComponents.push_back(PhysicsComponent());
	this->m_dynamicComponents.push_back(PhysicsComponent());
	//this->m_dynamicComponents.push_back(PhysicsComponent());

	this->m_dynamicComponents.push_back(PhysicsComponent());


	this->m_ballPC = &this->m_dynamicComponents.at(0);

	int size = this->m_dynamicComponents.size();

	this->m_links.push_back(ChainLink());
	this->m_links.at(0).CL_lenght = 3;
	this->m_links.at(0).CL_previous = &this->m_dynamicComponents.at(0);
	this->m_links.at(0).CL_next = &this->m_dynamicComponents.at(1);

	this->m_links.push_back(ChainLink());
	this->m_links.at(1).CL_lenght = 3;
	this->m_links.at(1).CL_previous = &this->m_dynamicComponents.at(1);
	this->m_links.at(1).CL_next = &this->m_dynamicComponents.at(2);

	this->m_links.push_back(ChainLink());
	this->m_links.at(2).CL_lenght = 3;
	this->m_links.at(2).CL_previous = &this->m_dynamicComponents.at(2);
	this->m_links.at(2).CL_next = &this->m_dynamicComponents.at(3);

	this->m_links.push_back(ChainLink());
	this->m_links.at(3).CL_lenght = 3;
	this->m_links.at(3).CL_previous = &this->m_dynamicComponents.at(3);
	this->m_links.at(3).CL_next = &this->m_dynamicComponents.at(4);

	//for (int i = 0; i < size; i++)
	//{
	//	this->m_chain.m_links.push_back(&this->m_dynamicComponents.at(i));
	//}

	this->m_playerPC = &this->m_dynamicComponents.at(size - 1);

	
	this->m_coneMagnet.m_pos = DirectX::XMVectorSet(0, 5, 0, 0);
	this->m_coneMagnet.m_lookDir = DirectX::XMVectorSet(0, -1, 0, 0);
	this->m_coneMagnet.m_pullStrenght = 1;
	this->m_coneMagnet.m_type = MagnetType::mag_Cone;
	this->m_coneMagnet.m_coneAreaAngle = (3.14f / 180.0f) * 45.0f;


	this->m_sphereMagnet.m_pos = DirectX::XMVectorSet(100, 9, 0, 0);
	this->m_sphereMagnet.m_lookDir = DirectX::XMVectorSet(0, -1, 0, 0);
	this->m_sphereMagnet.m_pullStrenght = 1;
	this->m_sphereMagnet.m_type = MagnetType::mag_Sphere;
	this->m_sphereMagnet.m_sphereRadius = 8.0f;

	this->m_AABBMagnet.m_pos = DirectX::XMVectorSet(100, 9, 0, 0);
	this->m_AABBMagnet.m_lookDir = DirectX::XMVectorSet(0, -1, 0, 0);
	this->m_AABBMagnet.m_pullStrenght = 1;
	this->m_AABBMagnet.m_type = MagnetType::mag_AABB;
	this->m_AABBMagnet.m_xWidth = 1;
	this->m_AABBMagnet.m_yWidth = 10;
	this->m_AABBMagnet.m_zWidth = 1;

	for (int i = 0; i < size; i++)
	{
		this->m_dynamicComponents.at(i).m_mass = 1.0f;
	}

	//ball physicsComponent
	this->m_ballPC->m_pos = DirectX::XMVectorSet(0, 0, 0, 0);
	this->m_ballPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	this->m_ballPC->m_mass = 5.0;
	this->m_ballPC->m_is_Static = false;
	//player physicsComponent
	this->m_playerPC->m_pos = DirectX::XMVectorSet(2, 0, 0, 0);
	this->m_playerPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
	this->m_playerPC->m_mass = 10000.0;
	this->m_playerPC->m_is_Static = false;

	int nrOfChainLinkObjects = this->m_chain.m_links.size();
	for (int i = 1; i < nrOfChainLinkObjects - 1; i++)
	{
		if (i != 0 && i != nrOfChainLinkObjects - 1)
		{
			this->m_chain.m_links.at(i)->m_pos = DirectX::XMVectorSet(3, 0, 0, 0);
			this->m_chain.m_links.at(i)->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
			this->m_chain.m_links.at(i)->m_mass = 1.0;
			this->m_chain.m_links.at(i)->m_is_Static = false;
		}
	}

	this->m_chain.m_linkLenght = 3.0f;

	this->m_chain.m_totalLenght = this->m_chain.m_linkLenght * this->m_chain.m_links.size();

	this->m_old_time = (GetCurrentTime());

	this->IntersectionTesting();

	return true;
}

void PhysicsHandler::Update()
{
	this->m_current_time = (GetCurrentTime());
	float dt = 0.01f;
	dt = (this->m_current_time - this->m_old_time) / 100;

	int size = this->m_chain.m_links.size();
	//for (int i = 0; i < size - 1; i++)
	//{
	//	this->DoChainPhysics(this->m_chain.m_links.at(i), this->m_chain.m_links.at(i + 1), dt);
	//}

	int nrOfLinks = this->m_links.size();
	for (int i = 0; i < nrOfLinks; i++)
	{
		this->DoChainPhysics(&this->m_links.at(i), dt);
	}

	size = this->m_dynamicComponents.size();
	for (int i = 0; i < size; i++)
	{
		if (!this->m_dynamicComponents.at(i).m_is_Static)
		{
			this->SimpleCollition(&this->m_dynamicComponents.at(i), dt);
		}
		if (i == 0)
		{

		}

		this->UpdatePhysicsComponent(&this->m_dynamicComponents.at(i), dt);
	}
	for (int i = 0; i < nrOfLinks; i++)
	{
		this->AdjustChainLinkPosition(&this->m_links.at(i));
	}
	//this->AdjustChainLinkPosition();

	this->m_old_time = this->m_current_time;
#pragma region input_for_testing

	if (GetAsyncKeyState(0x4f))
	{
		//this->m_ballPC->m_velocity = DirectX::XMVectorSet(3, 3, 0, 0);
		this->m_ballPC->m_velocity = DirectX::XMVectorSet(3, 3, 0, 0);
	}
	if (GetAsyncKeyState(0x55))
	{
		//this->m_ballPC->m_velocity = DirectX::XMVectorSet(-3, 3, 0, 0);
		this->m_ballPC->m_velocity = DirectX::XMVectorSet(-3, 3, 0, 0);
	}

	float str = 1.0f;
	if (GetAsyncKeyState(0x49))
	{
		this->ApplyForceToComponent(this->m_ballPC, DirectX::XMVectorSet(0, str, 0, 0), dt);
	}
	if (GetAsyncKeyState(0x4A))
	{
		this->ApplyForceToComponent(this->m_ballPC, DirectX::XMVectorSet(-str, 0, 0, 0), dt);
	}
	if (GetAsyncKeyState(0x4C))
	{
		this->ApplyForceToComponent(this->m_ballPC, DirectX::XMVectorSet(str, 0, 0, 0), dt);
	}
	if (GetAsyncKeyState(0x4B))
	{
		this->ApplyForceToComponent(this->m_ballPC, DirectX::XMVectorSet(0, -str, 0, 0), dt);
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
		this->m_ballPC->m_pos = DirectX::XMVectorSet(0, 9, 0, 0);
		this->m_ballPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		//this->m_ballPC->m_mass = 10;
		//this->m_ballPC->m_is_Static = true;
		//player physicsComponent
		this->m_playerPC->m_pos = DirectX::XMVectorSet(0, 9, 0, 0);
		this->m_playerPC->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		//this->m_playerPC->m_mass = 10;
		//this->m_playerPC->m_is_Static = false;

		int nrOfChainLinkObjects = this->m_chain.m_links.size();
		for (int i = 1; i < nrOfChainLinkObjects - 1; i++)
		{
			if (i != 0 && i != nrOfChainLinkObjects - 1)
			{
				this->m_chain.m_links.at(i)->m_pos = DirectX::XMVectorSet(0, 9, 0, 0);
				this->m_chain.m_links.at(i)->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
				//this->m_chain.m_links.at(i)->m_mass = 1.0;
				//this->m_chain.m_links.at(i)->m_is_Static = false;
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
	//SimpleGravity(ptr, dt);
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
	//componentPtr->m_velocity = DirectX::XMVectorScale(componentPtr->m_velocity, 0.9999);
	componentPtr->m_pos = DirectX::XMVectorAdd(componentPtr->m_pos, DirectX::XMVectorScale(componentPtr->m_velocity, dt));
}

void PhysicsHandler::ApplyForceToComponent(PhysicsComponent * componentPtr, DirectX::XMVECTOR force, float dt)
{
	if (!componentPtr->m_is_Static)
	{
		//float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(force));
		//if (lenght > 25)
		//{
		//	int error = 0;
		//}
		//float scale = (1.0 / componentPtr->m_mass);
		componentPtr->m_velocity = DirectX::XMVectorAdd(componentPtr->m_velocity, DirectX::XMVectorScale(force, (1.0 / componentPtr->m_mass)));
	}

}

void PhysicsHandler::DoChainPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt)
{ 
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(current->m_pos, next->m_pos);
	float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

	if (lenght >= this->m_chain.m_linkLenght)
	{
		diffVec = DirectX::XMVector3Normalize(diffVec);

		DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, lenght - this->m_chain.m_linkLenght), 0.2);
		//force = DirectX::XMVectorAdd(force, DirectX::XMVectorScale(DirectX::XMVectorSubtract(current->m_velocity, next->m_velocity), 0.5));

		this->ApplyForceToComponent(next, force, dt);
		force = DirectX::XMVectorScale(force, -1.0);
		this->ApplyForceToComponent(current, force, dt);
	}

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

		if (lenght > this->m_chain.m_linkLenght + 0.50f)
		{
			next->m_pos = DirectX::XMVectorAdd(current->m_pos, DirectX::XMVectorScale(diffVec, (this->m_chain.m_linkLenght + 0.5f)));
		}
	}
}

void PhysicsHandler::DoChainPhysics(ChainLink * link, float dt)
{
	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(link->CL_previous->m_pos, link->CL_next->m_pos);
	float lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));
	diffVec = DirectX::XMVector3Normalize(diffVec);
	if (lenght > link->CL_lenght)
	{
		DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, lenght - link->CL_lenght), 0.2);
		//force = DirectX::XMVectorAdd(force, DirectX::XMVectorScale(DirectX::XMVectorSubtract(link->CL_previous->PC_velocity, link->CL_next->PC_velocity), 0.5));

		this->ApplyForceToComponent(link->CL_next, force, dt);
		force = DirectX::XMVectorScale(force, -1.0);
		this->ApplyForceToComponent(link->CL_previous, force, dt);
	}
}

void PhysicsHandler::AdjustChainLinkPosition(ChainLink * link)
{

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(link->CL_next->m_pos, link->CL_previous->m_pos);

	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

	if (distance > link->CL_lenght + 0.5)
	{
		diffVec = DirectX::XMVector3Normalize(diffVec);
		link->CL_next->m_pos = DirectX::XMVectorAdd(link->CL_previous->m_pos, DirectX::XMVectorScale(diffVec, (link->CL_lenght + 0.5)));
		//link->CL_next->m_OBB.pos = link->CL_next->m_pos;
	}
}

void PhysicsHandler::MagnetComponentTest(PhysicsComponent * componentPtr, Magnet* magnet, float dt)
{
	if (magnet->m_type == MagnetType::mag_Cone)
	{
		DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(magnet->m_pos, componentPtr->m_pos);
		double lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));
		diffVec = DirectX::XMVector3Normalize(diffVec);

		double angle = DirectX::XMVectorGetX(DirectX::XMVector3Dot(diffVec, magnet->m_lookDir));
		angle = DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenVectors(DirectX::XMVectorScale(diffVec, -1.0), magnet->m_lookDir));

		if (angle <= magnet->m_coneAreaAngle)
		{
			DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, magnet->m_pullStrenght), (1.0f / lenght));
			this->ApplyForceToComponent(componentPtr, force, dt);
			if (lenght < 0.5)
			{
				componentPtr->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
			}
		}
	}
	if (magnet->m_type == MagnetType::mag_Sphere)
	{
		DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(magnet->m_pos, componentPtr->m_pos);
		double lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

		if (lenght <= magnet->m_sphereRadius)
		{
			DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, magnet->m_pullStrenght), (1.0 / lenght));
			this->ApplyForceToComponent(componentPtr, force, dt);
			if (lenght < 0.5)
			{
				componentPtr->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
			}
		}

	}
	if (magnet->m_type == MagnetType::mag_AABB)
	{
		DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(magnet->m_pos, componentPtr->m_pos);
		double lenght = DirectX::XMVectorGetX(DirectX::XMVector3Length(diffVec));

		double x = DirectX::XMVectorGetX(diffVec);
		double y = DirectX::XMVectorGetY(diffVec);
		double z = DirectX::XMVectorGetZ(diffVec);
		if (abs(x) < magnet->m_xWidth)
		{
			if (abs(y) < magnet->m_yWidth)
			{
				if (abs(z) < magnet->m_zWidth)
				{
					DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(DirectX::XMVectorScale(magnet->m_lookDir,-1.0f), magnet->m_pullStrenght), (1.0f / lenght));
					this->ApplyForceToComponent(componentPtr, force, dt);
					if (lenght < 0.5)
					{
						componentPtr->m_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
					}
				}
			}
		}

	}
}


void PhysicsHandler::IntersectionTesting()
{
	Sphere test_sphere;
	test_sphere.radius = 1.0f;

	AABB test_box;
	test_box.x_Width = 1;
	test_box.y_Width = 1;
	test_box.z_Width = 1;

	OBB test_obb;
	test_obb.m_XAxis = DirectX::XMVectorSet(1, 0, 0, 0);
	test_obb.m_YAxis = DirectX::XMVectorSet(0, 1, 0, 0);
	test_obb.m_ZAxis = DirectX::XMVectorSet(0, 0, 1, 0);

	DirectX::XMMATRIX rot;

	rot = DirectX::XMMatrixRotationX((3.14 / 180) * 90);// *DirectX::XMMatrixRotationY((3.14 / 180) * 66) *  DirectX::XMMatrixRotationZ((3.14 / 180) * 179);
	rot = DirectX::XMMatrixIdentity();

	test_obb.m_XAxis = DirectX::XMVector3Transform(test_obb.m_XAxis, rot);
	test_obb.m_YAxis = DirectX::XMVector3Transform(test_obb.m_YAxis, rot);
	test_obb.m_ZAxis = DirectX::XMVector3Transform(test_obb.m_ZAxis, rot);

	Plane test_plane;
	test_plane.m_normal = DirectX::XMVectorSet(1, 0, 0, 0);



	bool result = false;
	//result = this->OBBPlaneIntersectionTest(DirectX::XMVectorSet(0, 1, 0, 0), &test_obb, &test_plane);
	//result = this->AABBPlaneIntersectionTest(DirectX::XMVectorSet(0, 1, 0, 0), test_box, &test_plane);

	//result = this->SpherePlaneIntersectionTest(DirectX::XMVectorSet(1, 0, 0, 0), test_sphere, &test_plane);

	DirectX::XMVECTOR spherePos = DirectX::XMVectorSet(0, 10, 0, 0);
	DirectX::XMVECTOR boxPos = DirectX::XMVectorSet(0, 7, 0, 0);

	DirectX::XMVECTOR local_spherePos = DirectX::XMVectorSubtract(spherePos, boxPos);
	DirectX::XMMATRIX invRot = DirectX::XMMatrixInverse(nullptr, rot);
	local_spherePos = DirectX::XMVector3Transform(local_spherePos, invRot);

	AABB box;
	box.x_Width = DirectX::XMVectorGetX(DirectX::XMVector3Length(test_obb.m_XAxis));
	box.y_Width = DirectX::XMVectorGetX(DirectX::XMVector3Length(test_obb.m_YAxis));
	box.z_Width = DirectX::XMVectorGetX(DirectX::XMVector3Length(test_obb.m_ZAxis));

	result = this->SphereAABBIntersectionTest(local_spherePos, test_sphere, box);

	

}

bool PhysicsHandler::SphereAABBIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere sphere, AABB box)
{
	float sphere_center[3];
	sphere_center[0] = DirectX::XMVectorGetX(sphere_pos);
	sphere_center[1] = DirectX::XMVectorGetY(sphere_pos);
	sphere_center[2] = DirectX::XMVectorGetZ(sphere_pos);

	float box_center[3];
	box_center[0] = 0;
	box_center[1] = 0;
	box_center[2] = 0;
	float box_widths[3];
	box_widths[0] = box.x_Width;
	box_widths[1] = box.y_Width;
	box_widths[2] = box.z_Width;

	float s = 0;
	float d = 0;

	for (int i = 0; i < 3; i++)
	{
		if (sphere_center[i] < (-box_widths[i]))
		{
			s = (-box_widths[i]);
			d += s*s;
		}
		else if (sphere_center[i] > (box_widths[i]))
		{
			s = (box_widths[i]);
			d += s*s;
		}
	}

	return d <= sphere.radius * sphere.radius;

}

bool PhysicsHandler::SphereOBBIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere, OBB * box)
{


	return false;
}

bool PhysicsHandler::OBBAABBIntersectionTest(DirectX::XMVECTOR OBB_pos, OBB* obb, AABB aabb)
{
	return false;
}

bool PhysicsHandler::SphereSphereIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere sphere1, Sphere sphere2)
{
	float d = DirectX::XMVectorGetX(DirectX::XMVector3Length(sphere_pos));

	return d <= sphere1.radius + sphere2.radius;
}

bool PhysicsHandler::SpherePlaneIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere sphere1, Plane* plane)
{
	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR pPerpendicular;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, sphere_pos, plane->m_normal);

	float d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	return d < sphere1.radius;
}

bool PhysicsHandler::AABBPlaneIntersectionTest(DirectX::XMVECTOR box_pos, AABB box, Plane* plane)
{
	float d = 0;

	float a1 = box.x_Width;
	float a2 = box.y_Width;
	float a3 = box.z_Width;

	float sum = 0;
	sum += a1 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(plane->m_normal, DirectX::XMVectorSet(box.x_Width, 0, 0, 0)));
	sum += a2 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(plane->m_normal, DirectX::XMVectorSet(0, box.y_Width, 0, 0)));
	sum += a3 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(plane->m_normal, DirectX::XMVectorSet(0, 0, box.z_Width, 0)));

	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR dump;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &dump, box_pos, plane->m_normal);

	d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	return d < sum;
}

bool PhysicsHandler::OBBPlaneIntersectionTest(DirectX::XMVECTOR box_pos, OBB* box, Plane* plane)
{
	float d = 0;

	float a1 = DirectX::XMVectorGetX(DirectX::XMVector3Length(box->m_XAxis));
	float a2 = DirectX::XMVectorGetX(DirectX::XMVector3Length(box->m_YAxis));
	float a3 = DirectX::XMVectorGetX(DirectX::XMVector3Length(box->m_ZAxis));

	float sum = 0;
	sum += a1 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(plane->m_normal, box->m_XAxis));
	sum += a2 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(plane->m_normal, box->m_YAxis));
	sum += a3 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(plane->m_normal, box->m_ZAxis));

	DirectX::XMVECTOR pParallel;
	DirectX::XMVECTOR pPerpendicular;

	DirectX::XMVector3ComponentsFromNormal(&pParallel, &pPerpendicular, box_pos, plane->m_normal);

	d = DirectX::XMVectorGetX(DirectX::XMVector3Length(pParallel));

	return d < sum;
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

		DirectX::XMVECTOR force = DirectX::XMVectorScale(DirectX::XMVectorScale(diffVec, (lenght - this->m_chain.m_linkLenght)), 0.02);
		//force = DirectX::XMVectorAdd(force, DirectX::XMVectorScale(DirectX::XMVectorSubtract(next->m_velocity, current->m_velocity), 0.5));

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

		next->m_pos = DirectX::XMVectorAdd(current->m_pos, DirectX::XMVectorScale(diffVec, (this->m_chain.m_linkLenght)));

		float m1 = current->m_mass;
		float m2 = next->m_mass;

		DirectX::XMVECTOR velocityVec1 = current->m_velocity;
		DirectX::XMVECTOR velocityVec2 = next->m_velocity;

		DirectX::XMVECTOR parallelVelocityVec1;
		DirectX::XMVECTOR parallelVelocityVec2;
		DirectX::XMVECTOR perpendicularVelocityVec1;
		DirectX::XMVECTOR perpendicularVelocityVec2;

		DirectX::XMVector3ComponentsFromNormal(&parallelVelocityVec1, &perpendicularVelocityVec1, velocityVec1, diffVec);
		DirectX::XMVector3ComponentsFromNormal(&parallelVelocityVec2, &perpendicularVelocityVec2, velocityVec2, diffVec);

		float v1_old = DirectX::XMVectorGetX(DirectX::XMVector3Length(parallelVelocityVec1));
		float v2_old = DirectX::XMVectorGetX(DirectX::XMVector3Length(parallelVelocityVec2));


		float v1_new = (((m1 - m2) / (m1 + m2)) * v1_old) + (((2 * m1) / (m1 + m2))* v2_old);
																		//50 is important
		float forceMagnitude1 = ((current->m_mass * (v1_new - v1_old)) / 15.0);
		DirectX::XMVECTOR forceVec1 = DirectX::XMVectorScale(diffVec, (forceMagnitude1));
		this->ApplyForceToComponent(current, forceVec1, dt);


		float v2_new = (((2 * m1) / (m1 + m2)) * v1_old) + (((m2 - m1) / (m1 + m2))* v2_old);

		float forceMagnitude2 = ((next->m_mass * (v2_new - v2_old)) / 15.0);
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