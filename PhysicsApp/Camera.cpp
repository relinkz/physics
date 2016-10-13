#include "Camera.h"

Camera::Camera()
{
	this->position	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, -8.0f)));
	this->target	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)));
	this->upVector	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));
	this->right		= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)));
	this->translation = DirectX::XMMatrixTranslationFromVector(this->position);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	this->rotation = DirectX::XMMatrixIdentity();
	float rotationPerFrame = 0.1f;
	float speed = 0.5f * dt;


	if (GetAsyncKeyState(VK_SPACE))
	{
		this->position = DirectX::XMVectorAdd(this->position, DirectX::XMVectorScale(this->upVector, speed));
	}
	else if (GetAsyncKeyState(VK_CONTROL))
	{
		this->position = DirectX::XMVectorAdd(this->position, DirectX::XMVectorNegate(DirectX::XMVectorScale(this->upVector, speed)));
	}
	if (GetAsyncKeyState(VK_W))
	{
		this->position = DirectX::XMVectorAdd(this->position, DirectX::XMVectorScale(this->target, speed));
	}
	else if (GetAsyncKeyState(VK_S))
	{
		this->position = DirectX::XMVectorAdd(this->position, DirectX::XMVectorNegate(DirectX::XMVectorScale(this->target, speed)));
	}
	if (GetAsyncKeyState(VK_D))
	{
		this->position = DirectX::XMVectorAdd(this->position, DirectX::XMVectorScale(this->right, speed));
	}
	else if (GetAsyncKeyState(VK_A))
	{
		this->position = DirectX::XMVectorAdd(this->position, DirectX::XMVectorNegate(DirectX::XMVectorScale(this->right, speed)));
	}


	//this->rotation *= DirectX::XMMatrixRotationAxis(this->right, rotationPerFrame * dt);
	float angle = DirectX::XMVectorGetX(DirectX::XMVector3Dot(this->target, this->upVector));
	//DirectX::XMVECTOR a = DirectX::XMVector3AngleBetweenNormals(this->target, this->upVector);
	if (GetAsyncKeyState(VK_UP) && angle < 0.9f)
	{
		this->rotation *= DirectX::XMMatrixRotationAxis(this->right, -rotationPerFrame * dt);
	}
	else if (GetAsyncKeyState(VK_DOWN) && angle > -0.9f)
	{
		this->rotation *= DirectX::XMMatrixRotationAxis(this->right, rotationPerFrame * dt);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		this->rotation *= DirectX::XMMatrixRotationY(-rotationPerFrame * dt);
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		this->rotation *= DirectX::XMMatrixRotationY(rotationPerFrame * dt);
	}
	
	if (GetAsyncKeyState(VK_R))
	{
		ResetCamera();
	}

	this->target = DirectX::XMVector3Transform(this->target, this->rotation);
	this->right = DirectX::XMVector3Transform(this->right, this->rotation);

	this->translation = DirectX::XMMatrixTranslationFromVector(this->position);
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	DirectX::XMMATRIX toReturn;


	//DirectX::XMVector3Transform(this->target, this->translation);

	//toReturn = DirectX::XMMatrixLookAtLH(this->position, DirectX::XMVector3Transform(this->target, this->translation), this->upVector);
	toReturn = DirectX::XMMatrixLookAtLH(this->position, DirectX::XMVector3Transform(this->target, this->translation), DirectX::XMVector3Cross(this->target, this->right));
	//DirectX::XMVECTOR test1 = DirectX::XMVector3Transform(this->target, this->translation);
	//DirectX::XMVECTOR test2 = DirectX::XMVector3Cross(this->target, this->right);
	//toReturn = DirectX::XMMatrixLookAtLH(this->position, test1, test2);
	return DirectX::XMMatrixTranspose(toReturn);
}

DirectX::XMMATRIX Camera::getProjectionMatrix() const
{
	DirectX::XMMATRIX toReturn;
	toReturn = DirectX::XMMatrixPerspectiveFovLH(3.14f*0.45f, float(CLIENT_WIDTH) / float(CLIENT_HEIGHT), 0.5f, 50.0f);
	return DirectX::XMMatrixTranspose(toReturn);
}

DirectX::XMMATRIX Camera::getOrthogMatrix() const
{
	DirectX::XMMATRIX toReturn;
	toReturn = DirectX::XMMatrixOrthographicLH(CLIENT_WIDTH, CLIENT_HEIGHT, 0.5f, 20.0f);
	return DirectX::XMMatrixTranspose(toReturn);
}

void Camera::ResetCamera()
{
	this->position = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f)));
	this->target = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)));
	this->upVector = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));
	this->right = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)));
}

void Camera::SetPosition(Vector3 newPos)
{
	this->position = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(newPos.x, newPos.y, -4));
}

DirectX::XMMATRIX Camera::GetWorldMatrix() const
{
	float x = DirectX::XMVectorGetX(this->position);
	float y = DirectX::XMVectorGetY(this->position);
	float z = DirectX::XMVectorGetZ(this->position);

	return DirectX::XMMatrixTranslation(x, y, z);
}

Vector3 Camera::GetCameraPos() const
{
	float x = DirectX::XMVectorGetX(this->position);
	float y = DirectX::XMVectorGetY(this->position);
	float z = DirectX::XMVectorGetZ(this->position);


	Vector3 pos = Vector3(x, y, z);
	return pos;
}
