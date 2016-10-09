#include "TextRenderer.h"



TextRenderer::TextRenderer()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->inputLayout = nullptr;
	this->textSRV = nullptr;
	this->samplerState = nullptr;
	this->vertexBuffer = nullptr; 
	this->textWidth =  (160.0f / CLIENT_WIDTH);
	this->textHeight = (120.0f / CLIENT_HEIGHT);
	this->UPerNumber = 0.1;
}

TextRenderer::~TextRenderer()
{
	delete[] this->fontChars;
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
	vertex.UVs = DirectX::XMFLOAT2(0, 0);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0.5, -0.5, 0);
	vertex.UVs = DirectX::XMFLOAT2(1, 1);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0, -0.5, 0);
	vertex.UVs = DirectX::XMFLOAT2(0, 1);
	this->vertexData.push_back(vertex);

	vertex.Pos = DirectX::XMFLOAT3(0, 0, 0);
	vertex.UVs = DirectX::XMFLOAT2(0, 0);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0.5, 0, 0);
	vertex.UVs = DirectX::XMFLOAT2(1, 0);
	this->vertexData.push_back(vertex);
	vertex.Pos = DirectX::XMFLOAT3(0.5, -0.5, 0);
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

	//this->textSRV = parser.LoadTarga(this->gDevice, this->gDeviceContext, "Numbers.tga");
	this->textSRV = parser.LoadTarga(this->gDevice, this->gDeviceContext, "font.tga");
	//this->textSRV = parser.LoadTarga(this->gDevice, this->gDeviceContext, "swapTest.tga");

	std::ifstream file;

	char junkChar;
	int junkInt;
	std::stringstream ss;
	std::string line;
	std::string path = "fontData.txt";
	file.open(path, std::ios::in);
	if (!file.is_open()) {
		return false;
	}

	this->fontChars = new FontChar[95];

	for (int i = 0; i < 95; i++) {
		std::getline(file, line);
		ss.str(line);
		ss >> junkInt >> junkChar >> this->fontChars[i].left >> this->fontChars[i].right >> this->fontChars[i].size;
		ss.clear();
	}


	file.close();

	return true;
}

void TextRenderer::RenderBodyInfo(Body * body, Vector3 pos, float size)
{
	Vector3 position = pos;
	float yPosIncrement = 16 * size;
	//string str = "this text is rendered";

	//render body Name
	string str = body->getName();
	this->RenderTextRow(position, str, size);
	position.x = 0;
	position.y -= yPosIncrement;

	Vector3 v = body->getVelocity();
	//render x Velocity
	str = "Velocity X: " + std::to_string((int)v.x);
	this->RenderTextRow(position, str, size);
	position.x = 0;
	position.y -= yPosIncrement;

	//render y Velocity
	str = "Velocity Y: " + std::to_string((int)v.y);
	this->RenderTextRow(position, str, size);
	position.x = 0;
	position.y -= yPosIncrement;


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

	this->UpdateQuadPos(pos);
	//this->UpdateUVCoords(number);


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

void TextRenderer::RenderText(Vector3 pos, char* text, float size)
{

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
	
	Vector3 NDCpos = Vector3(0, 0, 0);
	NDCpos.x -= 1;
	NDCpos.y += 1;

	int nrLetters = (int)strlen(text);
	int letter = 0;
	int index = 0;
	float dx, dy = 0.0f;
	
	
	NDCpos = Vector3(pos.x / CLIENT_WIDTH, pos.y / CLIENT_HEIGHT, 0);
	NDCpos.x -= 1;
	NDCpos.y += 1;

	letter = text[0] - 32;
	if (letter == 0)
	{
		NDCpos.x += (3.0f * size) / CLIENT_WIDTH;
	}
	else
	{
		dx = float(this->fontChars[letter].size * size)  / CLIENT_WIDTH;
		dy = (16.0f * size) / CLIENT_HEIGHT;
		this->vertexData.at(0).Pos = DirectX::XMFLOAT3(NDCpos.x, NDCpos.y, 0);				//top left
		this->vertexData.at(0).UVs = DirectX::XMFLOAT2(this->fontChars[letter].left, 0);

		this->vertexData.at(1).Pos = DirectX::XMFLOAT3(NDCpos.x + dx, NDCpos.y - dy, 0);	//bottom right
		this->vertexData.at(1).UVs = DirectX::XMFLOAT2(this->fontChars[letter].right, 1);

		this->vertexData.at(2).Pos = DirectX::XMFLOAT3(NDCpos.x, NDCpos.y - dy, 0);			//bottom left
		this->vertexData.at(2).UVs = DirectX::XMFLOAT2(this->fontChars[letter].left, 1);

		this->vertexData.at(3).Pos = DirectX::XMFLOAT3(NDCpos.x, NDCpos.y, 0);				//top left
		this->vertexData.at(3).UVs = DirectX::XMFLOAT2(this->fontChars[letter].left, 0);

		this->vertexData.at(4).Pos = DirectX::XMFLOAT3(NDCpos.x + dx, NDCpos.y, 0);			//top right
		this->vertexData.at(4).UVs = DirectX::XMFLOAT2(this->fontChars[letter].right, 0);

		this->vertexData.at(5).Pos = DirectX::XMFLOAT3(NDCpos.x + dx, NDCpos.y - dy, 0);	//bottom right
		this->vertexData.at(5).UVs = DirectX::XMFLOAT2(this->fontChars[letter].right, 1);

		float t = (40.0f / CLIENT_WIDTH);
		NDCpos.x += t;

		this->UpdateVertexBuffer();
		this->gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);

		this->gDeviceContext->Draw(6, 0);

	}
	



}

float TextRenderer::getTextWidth() const
{
	return this->textWidth;;
}
float TextRenderer::getTextHeight() const
{
	return this->textHeight;;
}

float TextRenderer::getUPerNumber() const
{
	return this->UPerNumber;
}

void TextRenderer::setTextWidth(float newTextWidth)
{
	this->textWidth = (newTextWidth / CLIENT_WIDTH);
}
void TextRenderer::setTextHeight(float newTextHeight)
{
	this->textHeight = (newTextHeight / CLIENT_HEIGHT);
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

void TextRenderer::UpdateQuadPos(Vector3 pos)
{
	Vector3 NDCpos = Vector3(pos.x / CLIENT_WIDTH, pos.y / CLIENT_HEIGHT, 0);
	NDCpos.x -= 1;
	NDCpos.y += 1;

	this->vertexData.at(0).Pos = DirectX::XMFLOAT3(NDCpos.x, NDCpos.y, 0);
	this->vertexData.at(1).Pos = DirectX::XMFLOAT3(NDCpos.x + this->textWidth, NDCpos.y - this->textHeight, 0);
	this->vertexData.at(2).Pos = DirectX::XMFLOAT3(NDCpos.x, NDCpos.y - this->textHeight, 0);

	this->vertexData.at(3).Pos = DirectX::XMFLOAT3(NDCpos.x, NDCpos.y, 0);
	this->vertexData.at(4).Pos = DirectX::XMFLOAT3(NDCpos.x + this->textWidth, NDCpos.y, 0);
	this->vertexData.at(5).Pos = DirectX::XMFLOAT3(NDCpos.x + this->textWidth, NDCpos.y - this->textHeight, 0);


}

void TextRenderer::UpdateUVCoords(int number)
{
	Vector2 UV = Vector2(0, 0);
	UV.x = number * this->UPerNumber;

	this->vertexData.at(0).UVs = DirectX::XMFLOAT2(UV.x, UV.y);
	this->vertexData.at(1).UVs = DirectX::XMFLOAT2(UV.x + this->UPerNumber, 1);
	this->vertexData.at(2).UVs = DirectX::XMFLOAT2(UV.x, 1);
						   
	this->vertexData.at(3).UVs = DirectX::XMFLOAT2(UV.x, UV.y);
	this->vertexData.at(4).UVs = DirectX::XMFLOAT2(UV.x + this->UPerNumber, UV.y);
	this->vertexData.at(5).UVs = DirectX::XMFLOAT2(UV.x + this->UPerNumber, 1);


}

void TextRenderer::RenderTextRow(Vector3 pos, string str, float size)
{
	int a = 0;
	for (int i = 0; i < str.size(); i++)
	{
		this->RenderText(pos, &str.at(i), size);

		char* cha = &str.at(i);
		int index = cha[0] - 32;

		int textSize = this->fontChars[index].size;

		pos.x += (4.0f * size) + (textSize * 2.0f);
	}

	//pos.x = 0;
	//pos.y += 40;
}

