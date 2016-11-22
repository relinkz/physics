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

		DirectX::XMFLOAT3 pos1(0,0,0);
		DirectX::XMFLOAT3 ext1(2,2,2);

		DirectX::XMFLOAT3 pos2(3,0,0);
		DirectX::XMFLOAT3 ext2(2,2,2);
	}
}


