//#include "BoundingVolume.h"
//BoundingVolume::BoundingVolume()
//{
//	this->m_Pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
//}
//
//
//BoundingVolume::~BoundingVolume()
//{
//}
//
//DirectX::XMFLOAT3 BoundingVolume::GetPos() const
//{
//	return this->m_Pos;
//}
//
//bool BoundingVolume::CollitionTest(const BoundingVolume &other) const
//{
//	bool hit = false;
//
//	//first intersection test, very inprecice but cheap. Is there a possible collition?
//	//http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=3
//	hit = this->m_AABB.IntersectAABB(other.m_AABB);
//	if (hit == false)
//	{
//		return false;
//	}
//	
//	//OBB vs OBB
//
//	return true;
//}
