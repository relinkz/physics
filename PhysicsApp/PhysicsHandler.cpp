#include "PhysicsHandler.h"

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
	this->m_components.at(0).SetPos(Vector3(0, 5, 0));
	//this->SRV

	return false;
}

void PhysicsHandler::Update()
{
	float dt = 0.01f;

	SimpleCollition(dt);
}

void PhysicsHandler::SimpleCollition(float dt)
{
	int size = this->m_components.size();
	PhysicsComponent* ptr;
	for (int i = 0; i < size; i++)
	{
		ptr = &this->m_components.at(i);
		Vector3 pos = ptr->GetPos();
		if (pos.y > (0 + this->m_offSet))
		{
			SimpleGravity(ptr ,dt);
		}
		else if (pos.y < (0 + this->m_offSet))
		{
			ptr->SetPos(Vector3(0, (0 + this->m_offSet), 0));
			Vector3 vel = ptr->GetVelocity();
			ptr->SetVelocity(Vector3(vel.x, 0.0f, vel.z));
		}
		ptr->Update(dt);
	}
}

void PhysicsHandler::SimpleGravity(PhysicsComponent* pComponent, const float &dt)
{
	pComponent->ApplyForce(this->m_gravity,dt);
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
