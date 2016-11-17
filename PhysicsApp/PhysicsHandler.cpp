#include "PhysicsHandler.h"

PhysicsHandler::PhysicsHandler()
{
}

PhysicsHandler::~PhysicsHandler()
{
}

bool PhysicsHandler::Initialize(Engine* engine, Camera* gameCamera)
{
	this->engine = engine;
	this->gameCamera = gameCamera;

	this->Gravity = Vector3(0.0f, -0.1f, 0.0f);

	this->model.initialize(this->engine->getDevice(), this->engine->getDeviceContext(), DirectX::XMFLOAT3(0, 0, 0));

	this->box.initializeSkyBox(this->engine->getDevice(), this->engine->getDeviceContext(), DirectX::XMFLOAT3(0, 5, 0));
	this->box.setUniformScale(10);

	Parser parser = Parser();

	this->SRV = (parser.LoadTarga(this->engine->getDevice(), this->engine->getDeviceContext(), "cube_box.tga"));
	this->SRV2 = (parser.LoadTarga(this->engine->getDevice(), this->engine->getDeviceContext(), "Gray.tga"));


	this->components.push_back(PhysicsComponent());
	this->components.at(0).setPos(Vector3(0, 5, 0));
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
	int size = this->components.size();
	PhysicsComponent* ptr;
	for (int i = 0; i < size; i++)
	{
		ptr = &this->components.at(i);
		Vector3 pos = ptr->getPos();
		if (pos.y > (0 + offSet))
		{
			SimpleGravity(ptr ,dt);
		}
		else if (pos.y < (0 + offSet))
		{
			ptr->setPos(Vector3(0, (0 + offSet), 0));
			Vector3 vel = ptr->getVelocity();
			ptr->setVelocity(Vector3(vel.x, 0.0f, vel.z));
		}
		ptr->Update(dt);
	}
}

void PhysicsHandler::SimpleGravity(PhysicsComponent* pComponent, float dt)
{
	//Vector3 newPos = pComponent->getPos();
	//newPos.y -= 0.5;
	//
	//pComponent->setPos(newPos);

	pComponent->ApplyForce(this->Gravity,dt);
}

void PhysicsHandler::Render()
{
	this->box.setTranslationMatrix(Vector3(0, 5, 0));
	this->box.setUniformScale(10);
	this->box.update();
	this->engine->fillCBuffers(this->box.getWorldModel(), *this->gameCamera, 0);
	this->engine->drawObject(this->box, this->SRV);


	for (int i = 0; i < this->components.size(); i++)
	{
		Vector3 pos = components.at(i).getPos();
		this->model.setTranslationMatrix(pos);
		this->model.setUniformScale(0.5f);
		this->model.update();


		this->engine->fillCBuffers(this->model.getWorldModel(), *this->gameCamera, 0);
		this->engine->drawObject(this->model, this->SRV2);
	}

}
