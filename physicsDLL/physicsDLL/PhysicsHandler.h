#ifndef PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H
#define PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H

//#pragma once
//#define PHYSICSLIBRARY_EXPORTS
#ifdef PHYSICSDLL_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport) //__declspec tells the compiler and linker to export the function or variable from the DLL so that it can be used by other applications.
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

//#include "PhysicsComponent.h"
#include <DirectXMath.h>
#include <vector>

struct PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API AABB
{
	float pos[3];
	float ext[3];
};

struct PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsComponent
{
	int m_active;
	bool m_is_Static;
	int m_entityID;
	float m_mass;
	DirectX::XMVECTOR m_pos;
	DirectX::XMVECTOR m_velocity;
	DirectX::XMVECTOR m_rotationVelocity;
	double m_gravityInfluence;
	bool m_coolides;

	
	//AABB info
	AABB m_AABB;
	//AABB m_looseBoundingBox
	//BoundingVolume* m_tightBoundingVolume; 
	//std::vector<int entityID, event EVENT> m_eventlist;
};

class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
{
private:
	//Chain m_chain;

	//PhysicsComponent2* m_components;

	std::vector<PhysicsComponent*> m_dynamicComponents;
	int m_capacity;
	int m_nrOfComponents;
	//std::vector<Plane> m_walls;

	DirectX::XMVECTOR m_gravity;

	const float m_offSet = 0.5f;
	bool IntersectAABB();

public:
	PhysicsHandler();
	~PhysicsHandler();

	bool Initialize();
	void Update();

	void SimpleCollition(float dt);
	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	bool checkCollition();

	int getNrOfComponents()const;
	PhysicsComponent* getDynamicComponents(int index)const;


	//bool SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt);
	//bool PointInTriangleTest(DirectX::XMVECTOR point, Triangle triangle);


};

#endif
