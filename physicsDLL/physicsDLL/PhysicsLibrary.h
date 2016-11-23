#pragma once
#define PHYSICSLIBRARY_EXPORTS
#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

//#include "PhysicsComponent.h"
/*
dllexport and dllimport
https://msdn.microsoft.com/en-us/library/3y1sfaz2.aspx

*/

//including the vector
#include <DirectXMath.h>
namespace PhysicsLibrary
{



	class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API Fuctions
	{
	private:


	public:
		static void Gravity(DirectX::XMVECTOR &force, const float &dt);
	};
	

	/*class Chain
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

	};*/

	//Chain::Chain()
	//{
	//}

	//Chain::~Chain()
	//{
	//}

	//bool Chain::Initialize(float lenght, PhysicsComponent * playerPtr, PhysicsComponent * ballPtr)
	//{
	//	this->playerPtr = playerPtr;
	//	this->ballPtr = ballPtr;
	//	this->lenght = lenght;

	//	return false;
	//}

	//void Chain::DoChainPhysics(float dt)
	//{
	//	Vector3 playerToBall = this->ballPtr->GetPos() - this->playerPtr->GetPos();

	//	float d = Physics::calcRadius(playerToBall);

	//	playerToBall = Physics::normalizeVector(playerToBall);

	//	if (d > this->lenght)
	//	{
	//		//this->ballPtr->SetPos(this->playerPtr->GetPos() + (playerToBall * d));
	//		//
	//		//Vector3 velocityVector = this->ballPtr->GetVelocity();
	//		//float velocityLenght = Physics::calcRadius(velocityVector);
	//		//
	//		//velocityVector = Physics::normalizeVector(velocityVector);
	//		//
	//		//Vector3 resultVel = ((velocityVector * playerToBall) * playerToBall) * velocityLenght;
	//		//
	//		//this->playerPtr->SetVelocity(resultVel);


	//		Vector3 force = playerToBall * (d - this->lenght) * 0.2f;

	//		force = force + (this->ballPtr->GetVelocity() - this->playerPtr->GetVelocity()) * 0.5f;

	//		this->playerPtr->ApplyForce(force, dt);
	//	}



	//}

	//void Chain::AdjustVelocities(PhysicsComponent * pComponent, float dt)
	//{
	//	PhysicsComponent* toBeAdjusted = nullptr;
	//	PhysicsComponent* other = nullptr;


	//	if (pComponent == this->ballPtr)
	//	{
	//		//adjust player data
	//		toBeAdjusted = this->playerPtr;
	//		other = this->ballPtr;

	//	}
	//	if (pComponent == this->playerPtr)
	//	{
	//		//adjust ball data
	//		toBeAdjusted = this->ballPtr;
	//		other = this->playerPtr;

	//	}

	//	if (toBeAdjusted != nullptr)
	//	{
	//		Vector3 toVec = toBeAdjusted->GetPos() - other->GetPos();

	//		float d = Physics::calcRadius(toVec);

	//		toVec = Physics::normalizeVector(toVec);

	//		if (d > this->lenght)
	//		{

	//			toBeAdjusted->SetPos(other->GetPos() + (toVec * this->lenght));



	//		}


	//	}

	//}

	//void Chain::SetPlayerPtr(PhysicsComponent * playerPtr)
	//{
	//	this->playerPtr = playerPtr;
	//}

	//void Chain::SetBallPtr(PhysicsComponent * ballPtr)
	//{
	//	this->ballPtr = ballPtr;
	//}

	//void Chain::SetLenght(float lenght)
	//{
	//	this->lenght = lenght;
	//}

}

