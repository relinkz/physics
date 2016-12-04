#ifndef PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H
#define PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H

#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>

struct PhysicsComponent
{
	DirectX::XMVECTOR m_pos;
	DirectX::XMVECTOR m_velocity;
	DirectX::XMVECTOR m_rotationVelocity;
	double m_gravityInfluence;
	int m_active;
	int m_entityID;
	float m_mass;
	bool m_is_Static;
	bool m_coolides;
	//AABB m_looseBoundingBox
	//BoundingVolume* m_tightBoundingVolume; 
	//std::vector<int entityID, event EVENT> m_eventlist;

};
struct Chain
{
	float m_linkLenght;
	float m_totalLenght;
	std::vector<PhysicsComponent*> m_links;
};

class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
{
private:
	PhysicsComponent* m_ballPC;
	PhysicsComponent* m_playerPC;
	std::vector<PhysicsComponent> m_dynamicComponents;
	Chain m_chain;

	DirectX::XMVECTOR m_gravity;

	double m_old_time;
	double m_current_time;


	bool test;

	void SpringJointPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt);
	void ChainMomentumPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt);

public:
	PhysicsHandler();
	~PhysicsHandler();

	bool Initialize();
	void Update();

	void SimpleCollition(PhysicsComponent* componentPtr, float dt);
	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	void UpdatePhysicsComponent(PhysicsComponent* componentPtr, float dt);
	void ApplyForceToComponent(PhysicsComponent* componentPtr, DirectX::XMVECTOR force, float dt);

	void DoChainPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt);
	void AdjustChainLinkPosition();

	int getNrOfComponents()const;
	PhysicsComponent* getDynamicComponent(int index);
	PhysicsComponent* getPlayerComponent()const;
	PhysicsComponent* getBallComponent()const;


	//bool SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt);
	//bool PointInTriangleTest(DirectX::XMVECTOR point, Triangle triangle);


};

#endif
