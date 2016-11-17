#pragma once
#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H

#include "GlobalData.h"
#include "Model.h"
#include "Parser.h"
#include "Engine.h"

#include "PhysicsComponent.h"

class PhysicsHandler
{
public:
	PhysicsHandler();
	~PhysicsHandler();

	bool Initialize(Engine* engine, Camera* gameCamera);
	void Update();
	 
	void SimpleCollition();
	void SimpleGravity(PhysicsComponent* componentPtr);
	void Render();


private:
	
	vector<PhysicsComponent> components;

	Vector3 Gravity;
	Model model;
	Model box;
	ID3D11ShaderResourceView* SRV;
	ID3D11ShaderResourceView* SRV2;

	Engine* engine;
	Camera* gameCamera;

};

#endif

