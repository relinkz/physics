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

	this->Gravity = Vector3(0.0f, -1.0f, 0.0f);

	this->model.initialize(this->engine->getDevice(), this->engine->getDeviceContext(), DirectX::XMFLOAT3(0, 0, 0));

	this->box.initializeSkyBox(this->engine->getDevice(), this->engine->getDeviceContext(), DirectX::XMFLOAT3(0, 5, 0));
	this->box.setUniformScale(10);

	Parser parser = Parser();

	this->SRV = (parser.LoadTarga(this->engine->getDevice(), this->engine->getDeviceContext(), "cube_box.tga"));
	this->SRV2 = (parser.LoadTarga(this->engine->getDevice(), this->engine->getDeviceContext(), "Gray.tga"));


	this->components.push_back(PhysicsComponent());
	//this->SRV

	return false;
}

void PhysicsHandler::Update()
{

	SimpleCollition();
}

void PhysicsHandler::SimpleCollition()
{
	int size = this->components.size();
	PhysicsComponent* ptr;
	for (int i = 0; i < size; i++)
	{
		ptr = &this->components.at(i);
		Vector3 pos = ptr->getPos();
		if (pos.y > 0)
		{
			SimpleGravity(ptr);
		}
		else if (pos.y < 0)
		{
			ptr->setPos(Vector3(0, 0, 0));
			Vector3 vel = ptr->getVelocity();
			ptr->setVelocity(Vector3(vel.x, 0.0f, vel.z));
		}
	}
}

void PhysicsHandler::SimpleGravity(PhysicsComponent* pComponent)
{
	Vector3 newPos = pComponent->getPos();
	newPos.y -= 0.5;

	pComponent->setPos(newPos);
}

void PhysicsHandler::Render()
{
	this->box.setTranslationMatrix(Vector3(0, 4, 0));
	this->box.setUniformScale(10);
	this->box.update();
	this->engine->fillCBuffers(this->box.getWorldModel(), *this->gameCamera, 0);
	this->engine->drawObject(this->box, this->SRV);


	for (int i = 0; i < this->components.size(); i++)
	{
		Vector3 pos = components.at(i).getPos();
		this->model.setTranslationMatrix(pos);
		this->model.setUniformScale(1.0f);
		this->model.update();


		this->engine->fillCBuffers(this->model.getWorldModel(), *this->gameCamera, 0);
		this->engine->drawObject(this->model, this->SRV2);
	}

}
