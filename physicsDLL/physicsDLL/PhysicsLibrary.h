#pragma once
#define PHYSICSLIBRARY_EXPORTS
#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

/*
dllexport and dllimport
https://msdn.microsoft.com/en-us/library/3y1sfaz2.aspx

*/

//including the vector
#include <DirectXMath.h>
namespace PhysicsLibrary
{
	class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API Fuctions
	{
	private:


	public:
		static void Gravity(DirectX::XMVECTOR &force, const float &dt);
	};
}

