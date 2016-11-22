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
}


