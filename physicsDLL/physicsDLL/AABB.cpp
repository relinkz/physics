#include "AABB.h"



AABB::AABB()
{
	this->m_pos		= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->m_extends = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

AABB::AABB(DirectX::XMFLOAT3 & pos, DirectX::XMFLOAT3 & ext)
{
	this->m_pos		= pos;
	this->m_extends	= ext;
}

AABB::~AABB()
{
}

bool AABB::IntersectAABB(const AABB & toCheck) const
{
	bool possibleCollitionX = false;
	bool possibleCollitionY = false;
	bool possibleCollitionZ = false;
	DirectX::XMFLOAT3 vecToB;	//vector from A to B

	vecToB.x = toCheck.m_pos.x - this->m_pos.x;
	vecToB.y = toCheck.m_pos.y - this->m_pos.y;
	vecToB.z = toCheck.m_pos.z - this->m_pos.z;

	//Fraps return the absolute value
	//http://www.cplusplus.com/reference/cmath/fabs/

	//if the extensions from objA and objB together is smaller than the vector to b, then no collition
	possibleCollitionX = (fabs(vecToB.x) <= (this->m_extends.x + toCheck.m_extends.x));
	if (possibleCollitionX == false)
	{
		return false;
	}

	possibleCollitionY = (fabs(vecToB.y) <= (this->m_extends.y + toCheck.m_extends.y));
	if (possibleCollitionY == false)
	{
		return false;
	}

	possibleCollitionZ = (fabs(vecToB.z) <= (this->m_extends.z + toCheck.m_extends.z));
	if (possibleCollitionZ == false)
	{
		return false;
	}

	//collition possible
	return true;
}
