#include "PhysicsHandler.h"
//#include "modules\PhysicsLibrary.h"
//#include "C:\Users\apaka\Documents\GitHub\physics\physicsDLL\physicsDLL\PhysicsLibrary.h"
#include "../physicsDLL/physicsDLL/PhysicsLibrary.h"


#pragma comment (lib, "../PhysicsApp/Debug/physicsDLL")

PhysicsHandler::PhysicsHandler()
{
}

PhysicsHandler::~PhysicsHandler()
{
}

bool PhysicsHandler::Initialize(Engine* engine, Camera* gameCamera)
{
	this->m_engine = engine;
	this->m_gameCamera = gameCamera;

	this->m_gravity = Vector3(0.0f, -0.1f, 0.0f);

	this->m_model.initialize(this->m_engine->getDevice(), this->m_engine->getDeviceContext(), DirectX::XMFLOAT3(0, 0, 0));

	this->m_box.initializeSkyBox(this->m_engine->getDevice(), this->m_engine->getDeviceContext(), DirectX::XMFLOAT3(0, 5, 0));
	this->m_box.setUniformScale(10);

	Parser parser = Parser();

	this->m_SRV = (parser.LoadTarga(this->m_engine->getDevice(), this->m_engine->getDeviceContext(), "cube_box.tga"));
	this->m_SRV2 = (parser.LoadTarga(this->m_engine->getDevice(), this->m_engine->getDeviceContext(), "Gray.tga"));


	this->m_components.push_back(PhysicsComponent());
	this->m_components.at(0).SetPos(Vector3(0, 0.5, 0));
	//this->m_components.at(0).SetVelocity(Vector3(-1, 2, 0));

	this->m_components.push_back(PhysicsComponent());
	this->m_components.at(1).SetPos(Vector3(1, 0.5, 0));
	this->m_components.at(1).SetVelocity(Vector3(-1, 1, 0));
	//this->SRV

	this->m_chain.Initialize(3, &this->m_components.at(0), &this->m_components.at(1));

	this->m_walls.push_back(Plane());
	this->m_walls.at(0).m_pos = Vector3(-5, 0, -5);
	this->m_walls.at(0).m_normal = Vector3(0, 1, 0);

	this->m_walls.at(0).topLeft.m_pos1 = Vector3(-5, 0, -5);
	this->m_walls.at(0).topLeft.m_pos2 = Vector3(-5, 0, 5);
	this->m_walls.at(0).topLeft.m_pos3 = Vector3(5, 0, -5);

	this->m_walls.at(0).bottomRight.m_pos1 = Vector3(5, 0, 5);
	this->m_walls.at(0).bottomRight.m_pos2 = Vector3(-5, 0, 5);
	this->m_walls.at(0).bottomRight.m_pos3 = Vector3(5, 0, -5);

	this->m_walls.push_back(Plane());
	this->m_walls.at(1).m_pos = Vector3(5, 5, 0);
	this->m_walls.at(1).m_normal = Vector3(-1, 0, 0);

	this->m_walls.at(1).topLeft.m_pos1 = Vector3(5, 10, 5);
	this->m_walls.at(1).topLeft.m_pos2 = Vector3(5, 10, -5);
	this->m_walls.at(1).topLeft.m_pos3 = Vector3(5, 0, 5);

	this->m_walls.at(1).bottomRight.m_pos1 = Vector3(5, 0, -5);
	this->m_walls.at(1).bottomRight.m_pos2 = Vector3(5, 10, -5);
	this->m_walls.at(1).bottomRight.m_pos3 = Vector3(5, 0, 5);

	

	return false;
}

void PhysicsHandler::Update()
{
	float dt = 0.01f;

	SimpleCollition(dt);
	this->m_chain.DoChainPhysics(dt);

	DirectX::XMVECTOR test;

	PhysicsLibrary::Fuctions::Gravity(test, dt);
}

void PhysicsHandler::SimpleCollition(float dt)
{
	float m_frictionConstant = 0.999f; //this one is temporary

	int size = this->m_components.size();
	PhysicsComponent* ptr;
	for (int i = 0; i < size; i++)
	{
		ptr = &this->m_components.at(i);

		//SpherePlaneIntersevtion(ptr, 0.5f, this->m_floor, dt);

		int nrOfWalls = this->m_walls.size();

		for (int i = 0; i < nrOfWalls; i++)
		{
			SpherePlaneIntersevtion(ptr, 0.5f, this->m_walls.at(i), dt);
		}

		//Vector3 pos = ptr->GetPos();
		//
		//if (!SpherePlaneIntersevtion(ptr, 0.5f, this->m_floor))
		//{
		//	SimpleGravity(ptr, dt);
		//}
		//
		//if (pos.y > (0 + this->m_offSet))
		//{
		//	SimpleGravity(ptr ,dt);
		//}
		//else if (pos.y < (0 + this->m_offSet))
		//{
		//	ptr->SetPos(Vector3(ptr->GetPos().x, (0 + this->m_offSet), ptr->GetPos().z));
		//	Vector3 vel = ptr->GetVelocity();
		//	ptr->SetVelocity(Vector3(vel.x * m_frictionConstant, 0, vel.z * m_frictionConstant));
		//}
		//else if (pos.y == (0 + this->m_offSet))
		//{
		//	Vector3 vel = ptr->GetVelocity();
		//	ptr->SetVelocity(Vector3(vel.x * m_frictionConstant, vel.y, vel.z * m_frictionConstant));
		//}
		ptr->Update(dt);
	}
	if (GetAsyncKeyState(0x50))
	{
		//this->m_components.at(0).SetVelocity(Vector3(-1, 2, 0));

		this->m_components.at(1).SetVelocity(Vector3(1, 1, 0));
	}
	if (GetAsyncKeyState(0x4B))
	{
		//this->m_components.at(0).SetVelocity(Vector3(-1, 2, 0));

		this->m_components.at(1).SetVelocity(Vector3(1, 0, 0));
	}
	if (GetAsyncKeyState(0x4C))
	{
		this->m_components.at(0).SetPos(Vector3(0, 2.5, 0));
		this->m_components.at(0).SetVelocity(Vector3(0, 0, 0));
		//this->m_components.at(0).SetVelocity(Vector3(-1, 2, 0));

		this->m_components.at(1).SetPos(Vector3(1, 2.5, 0));
		this->m_components.at(1).SetVelocity(Vector3(0, 0, 0));
		this->m_components.at(1).SetVelocity(Vector3(0.5f, 0, 0));
	}
	if (GetAsyncKeyState(0x49))
	{
		this->m_components.at(0).SetPos(Vector3(0, 5.5, 0));
		this->m_components.at(0).SetVelocity(Vector3(0, 0, 0));
		//this->m_components.at(0).SetVelocity(Vector3(-1, 2, 0));

		this->m_components.at(1).SetPos(Vector3(1, 5.5, 0));
		this->m_components.at(1).SetVelocity(Vector3(0, 0, 0));
		//this->m_components.at(1).SetVelocity(Vector3(1, 1, 0));
	}
}

void PhysicsHandler::SimpleGravity(PhysicsComponent* pComponent, const float &dt)
{
	DirectX::XMVECTOR test = DirectX::XMVECTOR();
	DirectX::XMFLOAT3 testRes(0, 5, 0);
	test = DirectX::XMLoadFloat3(&testRes);

	pComponent->ApplyForce((this->m_gravity * pComponent->GetMass()),dt);

	PhysicsLibrary::Fuctions::Gravity(test, dt);
	DirectX::XMStoreFloat3(&testRes, test);
}

void PhysicsHandler::Render()
{
	this->m_box.setTranslationMatrix(Vector3(0, 5, 0));
	this->m_box.setUniformScale(10);
	this->m_box.update();
	this->m_engine->fillCBuffers(this->m_box.getWorldModel(), *this->m_gameCamera, 0);
	this->m_engine->drawObject(this->m_box, this->m_SRV);


	for (int i = 0; i < this->m_components.size(); i++)
	{
		Vector3 pos = m_components.at(i).GetPos();
		this->m_model.setTranslationMatrix(pos);
		this->m_model.setUniformScale(0.5f);
		this->m_model.update();


		this->m_engine->fillCBuffers(this->m_model.getWorldModel(), *this->m_gameCamera, 0);
		this->m_engine->drawObject(this->m_model, this->m_SRV2);
	}

}

bool PhysicsHandler::SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt)
{
	float m_frictionConstant = 0.99f; //this one is temporary
	float m_bounsiness = 0.91f; // also temporary

	bool result = false;

	Vector3 pos = pComponent->GetPos();

	Vector3 toObjectVec = (pos - plane.m_pos);

	float angle = Physics::dot(Physics::normalizeVector(toObjectVec), plane.m_normal);

	if (angle > 0.0f)
	{

		Vector3 projectionVec = Physics::vectorOnVectorProjection(toObjectVec, plane.m_normal);
		Vector3 vecOnPlane = toObjectVec - projectionVec;

		float lenghtToPlane = Physics::calcRadius(projectionVec);
		if (lenghtToPlane <= radius)
		{

			Vector3 pointOnPlane = plane.m_pos + vecOnPlane;

			result = PointInTriangleTest(pointOnPlane, plane.topLeft);
			if (!result)
			{
				result = PointInTriangleTest(pointOnPlane, plane.bottomRight);
			}

			if (result)
			{
				if (lenghtToPlane < radius)
				{
					pComponent->SetPos(plane.m_pos + (vecOnPlane)+(plane.m_normal * radius));
				}

				Vector3 vel = (pComponent->GetVelocity());

				

				Vector3 reflectedVel = vel - ((plane.m_normal * Physics::dot(vel, plane.m_normal)) * 2) * m_bounsiness;
				
				pComponent->SetVelocity(Vector3(reflectedVel.x * m_frictionConstant, reflectedVel.y * m_frictionConstant, reflectedVel.z * m_frictionConstant));
			}


			//pComponent->SetPos(plane.m_pos + (vecOnPlane) + (plane.m_normal * radius));
			//
			//Vector3 vel = pComponent->GetVelocity();
			//
			//pComponent->SetVelocity(Vector3(vel.x * m_frictionConstant, 0, vel.z * m_frictionConstant));
			//
			//result = true;
		
		}
	}

	if (!result)
	{
		SimpleGravity(pComponent, dt);
	}



	return result;
}

bool PhysicsHandler::PointInTriangleTest(Vector3 point, Triangle triangle)
{
	bool result = false;
	//one example 
	//http://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle


	//example used
	//http://math.stackexchange.com/questions/544946/determine-if-projection-of-3d-point-onto-plane-is-within-a-triangle

	Vector3 u = triangle.m_pos2 - triangle.m_pos1;

	Vector3 v = triangle.m_pos3 - triangle.m_pos1;

	Vector3 n = Physics::cross(u, v);

	Vector3 w = point - triangle.m_pos1;

	float y = Physics::dot(Physics::cross(u, w), n) / pow(Physics::calcRadius(n),2);

	float b = Physics::dot(Physics::cross(w, v), n) / pow(Physics::calcRadius(n), 2);

	float c = 1 - y - b;

	if (c >= 0 && c <= 1.0f && b >= 0 && b <= 1.0f && y >= 0 && y <= 1.0f)
	{
		result = true;
	}


	return result;
}
