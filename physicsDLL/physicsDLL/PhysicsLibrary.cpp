#include "PhysicsLibrary.h"

namespace PhysicsLibrary
{
	void PhysicsLibrary::Fuctions::Gravity(DirectX::XMVECTOR &force, const float &dt)
	{
		DirectX::XMFLOAT3 alteredForce;
		//store XMVector to XMFloat
		DirectX::XMStoreFloat3(&alteredForce, force);

		alteredForce.x *= dt;
		alteredForce.y *= dt;
		alteredForce.z *= dt;

		//load back to XMVector 
		force = DirectX::XMLoadFloat3(&alteredForce);
	}
	void PhysicsLibrary::Fuctions::bla()
	{
		int ahahahhaha = 0;
	}
	void PhysicsLibrary::Fuctions::boo()
	{
		int muwhahahahhahahahha = 12;
	}
	void PhysicsLibrary::Fuctions::buu()
	{
		int blublublub = 13;
	}
	void PhysicsLibrary::Fuctions::KimIsGod()
	{
		int skill_level = 90001;
	}
	void PhysicsLibrary::Fuctions::EvenMoreTestFunctions()
	{
		int woh = 0;
	}
	void PhysicsLibrary::Fuctions::PlsWork()
	{
		int plsplsplsplsplsplspsls = 1;
	}
	void PhysicsLibrary::Fuctions::DinMamma()
	{
	}
}


