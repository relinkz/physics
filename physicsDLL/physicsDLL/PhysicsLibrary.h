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
	class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API Fuctions
	{
	public:
		static void Gravity(DirectX::XMVECTOR &force, const float &dt);
		static void bla();
		static void boo();
		static void buu();
		static void KimIsGod();
		static void EvenMoreTestFunctions();
		static void PlsWork();
		static void DinMamma();
	};
}


//#include "GlobalData.h"
//#include "Model.h"
//#include "Parser.h"
//#include "Engine.h"
//
//#include "PhysicsComponent.h"
//
//class PhysicsHandler
//{
//private:
//	vector<PhysicsComponent> m_components;
//
//	Vector3 m_gravity;
//	Model m_model;
//	Model m_box;
//
//	ID3D11ShaderResourceView* m_SRV;
//	ID3D11ShaderResourceView* m_SRV2;
//
//	Engine* m_engine;
//	Camera* m_gameCamera;
//
//	const float m_offSet = 0.5f;
//public:
//	PhysicsHandler();
//	~PhysicsHandler();
//
//	bool Initialize(Engine* engine, Camera* gameCamera);
//	void Update();
//
//	void SimpleCollition(float dt);
//	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);
//	void Render();
//};
