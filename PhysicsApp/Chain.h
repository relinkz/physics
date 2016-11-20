#ifndef CHAIN_H
#define CHAIN_H
#include "GlobalData.h"
#include "Physics.h"
#include "PhysicsComponent.h"

class Chain
{
private:
	PhysicsComponent* playerPtr;
	PhysicsComponent* ballPtr;
	float lenght;


public:
	Chain();
	~Chain();

	bool Initialize(float lenght, PhysicsComponent* playerPtr, PhysicsComponent* ballPtr);

	void DoChainPhysics(float dt);
	void AdjustVelocities(PhysicsComponent* pComponent, float dt);

	void SetPlayerPtr(PhysicsComponent* playerPtr);
	void SetBallPtr(PhysicsComponent* ballPtr);
	void SetLenght(float lenght);

};

#endif
