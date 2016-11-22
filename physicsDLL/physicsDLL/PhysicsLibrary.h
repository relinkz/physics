#pragma once
#define PHYSICSLIBRARY_EXPORTS
#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

#include "BoundingVolume.h"

/*
dllexport and dllimport
https://msdn.microsoft.com/en-us/library/3y1sfaz2.aspx
*/
namespace PhysicsLibrary
{
	class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API Fuctions
	{
	public:
		static void Gravity(DirectX::XMVECTOR &force, const float &dt);
	};
}

