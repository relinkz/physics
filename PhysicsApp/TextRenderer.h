#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <windows.h>
#include"GlobalData.h"
#include "Parser.h"
#include <string>
#include <vector>

class TextRenderer
{
private:
	
	float textSize;
	float UPerNumber;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	
	ID3D11InputLayout* inputLayout;

	ID3D11ShaderResourceView *textSRV;

	ID3D11SamplerState *samplerState;

	ID3D11Buffer *vertexBuffer;

	std::vector<Vertex3>vertexData;
	
	ID3D11Device *gDevice;
	ID3D11DeviceContext *gDeviceContext;


	void UpdateVertexBuffer();

public:
	TextRenderer();
	~TextRenderer();

	bool Initialize(ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext);

	void RenderNumber(Vector3 pos,float number);
	void RenderText(Vector3 pos, std::string str);

	float getTextSize() const;
	float getUPerNumber() const;

	void setTextSize(float newTextSize);
	void setUPerNumber(float newUPerNumber);

};



#endif
