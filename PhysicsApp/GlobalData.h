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

UINT const CLIENT_WIDTH = 640;
UINT const CLIENT_HEIGHT = 480;


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
	float x;
	float y;
	float z;

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
};

struct int3
{
	int i1;
	int i2;
	int i3;
};