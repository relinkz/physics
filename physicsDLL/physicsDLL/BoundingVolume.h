#ifndef  PHYSICSDLL_PHYSICS_BOUNDINGVOLUME_H
#define  PHYSICSDLL_PHYSICS_BOUNDINGVOLUME_H
#include "AABB.h"

class BoundingVolume
{
private:
	DirectX::XMFLOAT3 m_Pos;
public:
	BoundingVolume();
	virtual ~BoundingVolume();

	DirectX::XMFLOAT3 GetPos() const;
};

#endif

