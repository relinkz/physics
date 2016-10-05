#pragma once
#include "GlobalData.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Update(float dt);

	DirectX::XMMATRIX getViewMatrix() const;
	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getOrthogMatrix() const;

	void ResetCamera();
	void SetPosition(Vector3 newPos);
private:
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR target;
	DirectX::XMVECTOR upVector;
	DirectX::XMVECTOR right;

	DirectX::XMMATRIX translation;
	DirectX::XMMATRIX rotation;

	float perspAngle;
	float nearPlane;
	float farPlane;
};

