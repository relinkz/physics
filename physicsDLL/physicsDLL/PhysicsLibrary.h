#pragma once
#define PHYSICSLIBRARY_EXPORTS
#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

/*
dllexport and dllimport
https://msdn.microsoft.com/en-us/library/3y1sfaz2.aspx

*/

//including the vector
#include <DirectXMath.h>
namespace PhysicsLibrary
{
	//struct Triangle
	//{
	//	DirectX::XMVECTOR m_pos1;
	//	DirectX::XMVECTOR m_pos2;
	//	DirectX::XMVECTOR m_pos3;
	//};
	//struct Plane
	//{
	//	Triangle topLeft;
	//	Triangle bottomRight;
	//	DirectX::XMVECTOR m_pos;
	//	DirectX::XMVECTOR m_normal;
	//};


	class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API Fuctions
	{
	private:


	public:
		static void Gravity(DirectX::XMVECTOR &force, const float &dt);
	};
	class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsComponent
	{
	private:
		DirectX::XMVECTOR m_pos;
		DirectX::XMVECTOR m_velocity;
		float m_mass;

		DirectX::XMVECTOR m_vel;
	public:
		PhysicsComponent();
		~PhysicsComponent();

		void Update(const float &dt);

		DirectX::XMVECTOR GetPos()const;
		DirectX::XMVECTOR GetVelocity()const;
		float GetMass()const;

		void SetPos(DirectX::XMVECTOR pos);
		void SetVelocity(DirectX::XMVECTOR vel);
		void SetMass(float mass);

		void ApplyForce(DirectX::XMVECTOR force, const float &dt);

	};

	class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
	{
	private:
		//Chain m_chain;

		PhysicsComponent* m_components;
		int m_capacity;
		int m_nrOfComponents;
		//std::vector<Plane> m_walls;

		DirectX::XMVECTOR m_gravity;

		const float m_offSet = 0.5f;
	public:
		PhysicsHandler();
		~PhysicsHandler();

		bool Initialize();
		void Update();

		void SimpleCollition(float dt);
		void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

		int getNrOfComponents()const;
		PhysicsComponent* getPComponents()const;


		//bool SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt);
		//bool PointInTriangleTest(DirectX::XMVECTOR point, Triangle triangle);


	};
}

