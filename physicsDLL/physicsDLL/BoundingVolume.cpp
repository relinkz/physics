#include "BoundingVolume.h"
BoundingVolume::BoundingVolume()
{
	this->m_Pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}


BoundingVolume::~BoundingVolume()
{
}

DirectX::XMFLOAT3 BoundingVolume::GetPos() const
{
	return this->m_Pos;
}
