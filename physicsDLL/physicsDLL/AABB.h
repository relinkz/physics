#ifndef  PHYSICSDLL_PHYSICS_AABB_H
#define  PHYSICSDLL_PHYSICS_AABB_H

//including the vector
#include <DirectXMath.h>

class AABB
{
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_extends;

public:
	AABB();
	AABB(DirectX::XMFLOAT3 &pos, DirectX::XMFLOAT3 &ext);
	virtual ~AABB();


};

#endif

