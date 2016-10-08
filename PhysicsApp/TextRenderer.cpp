#include "TextRenderer.h"



TextRenderer::TextRenderer()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->inputLayout = nullptr;
	this->textSRV = nullptr;
	this->samplerState = nullptr;
	this->vertexBuffer = nullptr; 
	this->textSize = 0.1f;
	this->UPerNumber = 0.1;
}

TextRenderer::~TextRenderer()
{
}

bool TextRenderer::Initialize(ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext)
{
	HRESULT result;

	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;

	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"TextVS.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	result = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->vertexShader);

	//creating the input layout
	D3D11_INPUT_ELEMENT_DESC desc1[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = this->gDevice->CreateInputLayout(desc1, 2, pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->inputLayout);
	pVS->Release();

	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"TextPS.hlsl", // filename		//L"PixelShader.hlsl"
		nullptr,		// optional macros
		nullptr,		// optional include files
		"main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	result = this->gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->pixelShader);

	Vertex3 vertex;
	vertex.Pos = DirectX::XMFLOAT3(0, 0, 0);
	vertex.UVs = DirectX::XMFLOAT2(0, 1);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0, 0.5, 0);
	vertex.UVs = DirectX::XMFLOAT2(0, 0);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0.5, 0.5, 0);
	vertex.UVs = DirectX::XMFLOAT2(1, 0);
	this->vertexData.push_back(vertex);

	vertex.Pos = DirectX::XMFLOAT3(0, 0, 0);
	vertex.UVs = DirectX::XMFLOAT2(0, 1);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0.5, 0.5, 0);
	vertex.UVs = DirectX::XMFLOAT2(1, 0);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0.5, 0, 0);
	vertex.UVs = DirectX::XMFLOAT2(1, 1);
	this->vertexData.push_back(vertex);

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Vertex2) * this->vertexData.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = false;
	desc.StructureByteStride = 0;

	result = this->gDevice->CreateBuffer(&desc, NULL, &this->vertexBuffer);

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = this->gDevice->CreateSamplerState(&sampDesc, &this->samplerState);

	Parser parser = Parser();

	this->textSRV = parser.LoadTarga(this->gDevice, this->gDeviceContext, "Numbers.tga");

	return true;
}

void TextRenderer::RenderNumber(Vector3 pos, float number)
{
	//this->vertexData.at(0).Pos = DirectX::XMFLOAT3(pos.x / CLIENT_WIDTH, pos.y / CLIENT_HEIGHT, 0);
	//this->vertexData.at(1).Pos = DirectX::XMFLOAT3((pos.x / CLIENT_WIDTH) + this->textSize, (pos.y / CLIENT_HEIGHT) + this->textSize, 0);
	//this->vertexData.at(2).Pos = DirectX::XMFLOAT3(pos.x / CLIENT_WIDTH, (pos.y / CLIENT_HEIGHT) + this->textSize, 0);
	//
	//this->vertexData.at(3).Pos = DirectX::XMFLOAT3(pos.x / CLIENT_WIDTH, pos.y / CLIENT_HEIGHT, 0);
	//this->vertexData.at(4).Pos = DirectX::XMFLOAT3((pos.x / CLIENT_WIDTH) + this->textSize, pos.y / CLIENT_HEIGHT, 0);
	//this->vertexData.at(5).Pos = DirectX::XMFLOAT3((pos.x / CLIENT_WIDTH) + this->textSize, (pos.y / CLIENT_HEIGHT) + this->textSize, 0);

	this->UpdateVertexBuffer();

	this->gDeviceContext->VSSetShader(this->vertexShader, nullptr, 0);

	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->pixelShader, nullptr, 0);

	this->gDeviceContext->PSSetSamplers(0, 1, &this->samplerState);
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->textSRV);

	this->gDeviceContext->IASetInputLayout(this->inputLayout);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT vertexSize = sizeof(Vertex3);
	UINT offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);

	this->gDeviceContext->Draw(6, 0);

}

void TextRenderer::RenderText(Vector3 pos, std::string str)
{
}

float TextRenderer::getTextSize() const
{
	return this->textSize;;
}

float TextRenderer::getUPerNumber() const
{
	return this->UPerNumber;
}

void TextRenderer::setTextSize(float newTextSize)
{
	this->textSize = newTextSize;
}

void TextRenderer::setUPerNumber(float newUPerNumber)
{
	this->UPerNumber = newUPerNumber;
}


void TextRenderer::UpdateVertexBuffer()
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedData;

	//map the memory, so that it cant be used in gpu while we change it
	result = this->gDeviceContext->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	//read the data and save them in a variable
	Vertex3* v = reinterpret_cast<Vertex3*>(mappedData.pData);
	UINT size = this->vertexData.size();

	//finally write the data into the vertex buffer
	for (UINT i = 0; i < size; i++)
	{
		v[i] = this->vertexData.at(i);
	}

	gDeviceContext->Unmap(this->vertexBuffer, 0);
}
