#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_R 0x52
#define VK_I 0x49
#define VK_J 0x4A
#define VK_U 0x55
#define VK_H 0x48

UINT const CLIENT_WIDTH = 640;
UINT const CLIENT_HEIGHT = 480;

//const long float AU = (149.6e6 * 1000.0f);
const long float AU = 149597870700;
const long float SCALE = 250 / AU;

const long float SCALE2 = 0.000000000000000000000001f;

const long float G = 6.674 * pow(10, -11);

struct FontChar
{
	float left, right;
	int size;
};
struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};
struct Vertex1
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct Vertex2
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UVs;
};
struct Vertex3
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 UVs;
};

struct Vector2
{
	float x;
	float y;

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Vector3
{
	long float x;
	long float y;
	long float z;

	Vector3()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3 operator +(const Vector3 &src)
	{
		Vector3 newVector;

		newVector.x = this->x + src.x;
		newVector.y = this->y + src.y;
		newVector.z = this->z + src.z;

		return newVector;
	}

	Vector3 operator -(const Vector3 &src)
	{
		Vector3 newVector;

		newVector.x = this->x - src.x;
		newVector.y = this->y - src.y;
		newVector.z = this->z - src.z;

		return newVector;
	}

	Vector3 operator /(const Vector3 &src)
	{
		Vector3 newVector;

		newVector.x = this->x / src.x;
		newVector.y = this->y / src.y;
		newVector.z = this->z / src.z;

		return newVector;
	}

	Vector3 operator *(const Vector3 &src)
	{
		Vector3 newVector;

		newVector.x = this->x * src.x;
		newVector.y = this->y * src.y;
		newVector.z = this->z * src.z;

		return newVector;
	}

	Vector3 operator /(const float &flt)
	{
		Vector3 newVector;

		newVector.x = this->x / flt;
		newVector.y = this->y / flt;
		newVector.z = this->z / flt;

		return newVector;
	}

	Vector3 operator *(const float &flt)
	{
		Vector3 newVector;

		newVector.x = this->x * flt;
		newVector.y = this->y * flt;
		newVector.z = this->z * flt;

		return newVector;
	}
};

struct int3
{
	int i1;
	int i2;
	int i3;
};
struct Triangle
{
	Vector3 m_pos1;
	Vector3 m_pos2;
	Vector3 m_pos3;
};
struct Plane
{
	Triangle topLeft;
	Triangle bottomRight;
	Vector3 m_pos;
	Vector3 m_normal;
	Vector3 m_left;
	Vector3 m_forward;
};
