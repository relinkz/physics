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
