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

enum MagnetType
{
	mag_Sphere,
	mag_Cone,
	mag_AABB
};
struct Magnet
{
	DirectX::XMVECTOR m_pos;
	DirectX::XMVECTOR m_lookDir;
	double m_pullStrenght;
	MagnetType m_type;

	double m_coneAreaAngle;
	double m_sphereRadius;

	double m_xWidth;
	double m_yWidth;
	double m_zWidth;
};
struct Sphere
{
	float radius;
};
struct AABB
{
	float x_Width,
		y_Width,
		z_Width;
};
struct Plane
{
	DirectX::XMVECTOR m_normal;
};
struct OBB
{
	DirectX::XMVECTOR m_XAxis;
	DirectX::XMVECTOR m_YAxis;
	DirectX::XMVECTOR m_ZAxis;
};
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

struct ChainLink
{
	float CL_lenght;
	PhysicsComponent* CL_next;
	PhysicsComponent* CL_previous;
};
class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
{
private:
	PhysicsComponent* m_ballPC;
	PhysicsComponent* m_playerPC;
	std::vector<PhysicsComponent> m_dynamicComponents;
	std::vector<ChainLink> m_links;
	Chain m_chain;
	Magnet m_coneMagnet;
	Magnet m_sphereMagnet;
	Magnet m_AABBMagnet;

	DirectX::XMVECTOR m_gravity;

	double m_old_time;
	double m_current_time;


	bool test;

	void SpringJointPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt);
	void ChainMomentumPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt);

	void IntersectionTesting();
	bool SphereAABBIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere sphere, AABB box);
	bool SphereOBBIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere, OBB* box);
	bool OBBAABBIntersectionTest(DirectX::XMVECTOR OBB_pos, OBB* obb, AABB aabb);
	bool SphereSphereIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere sphere1, Sphere sphere2);
	bool SpherePlaneIntersectionTest(DirectX::XMVECTOR sphere_pos, Sphere sphere1, Plane* plane);
	bool AABBPlaneIntersectionTest(DirectX::XMVECTOR box_pos, AABB box, Plane* plane);
	bool OBBPlaneIntersectionTest(DirectX::XMVECTOR box_pos, OBB* box, Plane* plane);



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

	void DoChainPhysics(ChainLink* link, float dt);
	void AdjustChainLinkPosition(ChainLink* link);

	void MagnetComponentTest(PhysicsComponent* componentPtr, Magnet* magnet, float dt);

	int getNrOfComponents()const;
	PhysicsComponent* getDynamicComponent(int index);
	PhysicsComponent* getPlayerComponent()const;
	PhysicsComponent* getBallComponent()const;


	//bool SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt);
	//bool PointInTriangleTest(DirectX::XMVECTOR point, Triangle triangle);


};

#endif
