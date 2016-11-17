#ifndef BASICWINDOW_PHYSICS_PHYSICSHANDLER_H
#define BASICWINDOW_PHYSICS_PHYSICSHANDLER_H

#include "GlobalData.h"
#include "Model.h"
#include "Parser.h"
#include "Engine.h"

#include "PhysicsComponent.h"

class PhysicsHandler
{
private:
	vector<PhysicsComponent> m_components;

	Vector3 m_gravity;
	Model m_model;
	Model m_box;
	
	ID3D11ShaderResourceView* m_SRV;
	ID3D11ShaderResourceView* m_SRV2;

	Engine* m_engine;
	Camera* m_gameCamera;

	const float m_offSet = 0.5f;
public:
	PhysicsHandler();
	~PhysicsHandler();

	bool Initialize(Engine* engine, Camera* gameCamera);
	void Update();

	void SimpleCollition(float dt);
	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);
	void Render();	
};

#endif

