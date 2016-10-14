#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <windows.h>
#include"GlobalData.h"
#include "Body.h"
#include "Parser.h"
#include <string>
#include <vector>

class TextRenderer
{
private:
	
	float textWidth;
	float textHeight;
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

	FontChar* fontChars;

	void UpdateVertexBuffer();
	void UpdateQuadPos(Vector3 pos);
	void UpdateUVCoords(int number);

public:
	TextRenderer();
	~TextRenderer();

	bool Initialize(ID3D11Device *gDevice, ID3D11DeviceContext *gDeviceContext);

	void RenderBodyInfo(Body* body, Vector3 pos, float size);
	void RenderNumber(Vector3 pos,float number);
	void RenderText(Vector3 pos, char *text, float size);
	void RenderTextRow(Vector3 pos, string str, float size);

	float getTextWidth() const;
	float getTextHeight() const;
	float getUPerNumber() const;

	void setTextWidth(float newTextWidth);
	void setTextHeight(float newTextHeight);
	void setUPerNumber(float newUPerNumber);

};



#endif
