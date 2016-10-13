#include "Engine.h"
#include <atlbase.h>

Engine::Engine()
{
	this->gDeviceContext = nullptr;
	this->gDevice = nullptr;
	
	this->g4xMsaaQuality = 0;
	this->enable4xMSAA = false;

	this->gSwapChain = nullptr;
	this->gRenderTargetView = nullptr;

	this->mDepthStencilBuffer = nullptr;
	this->mDepthStencilView = nullptr;

	this->vertexShader = nullptr;
	this->pixelShader = nullptr;

	this->indexBuffer = nullptr;
	this->inputLayout = nullptr;

	this->matrixBuffer = nullptr;

	this->bakcGroundSRV = nullptr;
	this->SRVtest = nullptr;
}


Engine::~Engine()
{
}

bool Engine::initialize(HWND* window)
{
	HRESULT result;
	//*************************
	//Creating the interfaces**
	//*************************
	D3D_FEATURE_LEVEL fLevel;

	result = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		0,
		0,
		D3D11_SDK_VERSION,
		&this->gDevice,
		&fLevel,
		&this->gDeviceContext
		);

	if (FAILED(result))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}
	if (fLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature level 11 unsupported",0 ,0);
		return false;
	}

	//*************************
	//4XMSAA Quality support***
	//*************************
	//with this technique, the back and depth buffer are rendered with
	//a resulotion 4 times bigger than the monitors resolution. When it's time
	//for the backbuffer to be presented (becoming "front buffer") the buffer is 
	//resolved (downsampled), taking the average of 4 pixels, smoothing edges.

	result = this->gDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &this->g4xMsaaQuality);
	if (this->g4xMsaaQuality < 0)
	{
		this->g4xMsaaQuality = 0;
	}

	//*************************
	//Swap chain            ***
	//*************************
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = CLIENT_WIDTH;
	sd.BufferDesc.Height = CLIENT_HEIGHT;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (this->enable4xMSAA)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = this->g4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = *window;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	//haxxing required to avoid incluing libs, to get a factory
	IDXGIDevice* dxgiDevice = 0;
	result = this->gDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	//getting the COM factory
	IDXGIFactory* dxgiFactory = 0;
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	//create the swapchain
	result = dxgiFactory->CreateSwapChain(this->gDevice, &sd, &this->gSwapChain);

	//realeasing the com object since they are no longer needed
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	//*************************
	//render target view    ***
	//*************************
	ID3D11Texture2D* backBuffer;
	this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	
	result = this->gDevice->CreateRenderTargetView(backBuffer, 0, &this->gRenderTargetView);

	//the getBuffer increases the com reference count, therefore releasing it,
	//but what happens whith the backbuffer??????
	backBuffer->Release();

	/*
	Depth stencil / z-buffer and view
	*/

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = CLIENT_WIDTH;
	depthStencilDesc.Height = CLIENT_HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (this->enable4xMSAA)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = this->g4xMsaaQuality;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	result = this->gDevice->CreateTexture2D(&depthStencilDesc, 0, &this->mDepthStencilBuffer);

	if (FAILED(result))
	{
		MessageBox(0, L"Creating Depth-buffer failed.", 0, 0);
		return false;
	}

	result = this->gDevice->CreateDepthStencilView(this->mDepthStencilBuffer, 0, &this->mDepthStencilView);

	if (FAILED(result))
	{
		MessageBox(0, L"Creating Depth-stencil view failed.", 0, 0);
		return false;
	}

	this->gDeviceContext->OMSetRenderTargets(1, &gRenderTargetView, this->mDepthStencilView);

	/*
	Set viewport
	*/
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(CLIENT_WIDTH);
	vp.Height = static_cast<float>(CLIENT_HEIGHT);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	this->gDeviceContext->RSSetViewports(1,&vp);

	/*
	creating shaders
	*/

	ID3DBlob* errorBlob = nullptr;
	//creating the vertexshader

	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		);

	result = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->vertexShader);

	//creating the input layout
	D3D11_INPUT_ELEMENT_DESC desc1[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = gDevice->CreateInputLayout(desc1, 3, pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->inputLayout);
	pVS->Release();

	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Fragment.hlsl", // filename		//L"PixelShader.hlsl"
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
		);

	result = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->pixelShader);

	Parser parser = Parser();

	this->SRVtest		= parser.LoadTarga(this->gDevice, this->gDeviceContext, "PathfinderMap.tga");
	this->bakcGroundSRV	= parser.LoadTarga(this->gDevice, this->gDeviceContext, "backGround.tga");
	this->backGroundModel.initializeSkyBox(this->gDevice, this->gDeviceContext, DirectX::XMFLOAT3(0,0,0));
	
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

	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	depthStateDesc.DepthEnable = TRUE;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStateDesc.StencilEnable = TRUE;
	depthStateDesc.StencilReadMask = 0xFF;
	depthStateDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	result = this->gDevice->CreateDepthStencilState(&depthStateDesc, &this->mDpethState1);
	depthStateDesc.DepthEnable = false;
	result = this->gDevice->CreateDepthStencilState(&depthStateDesc, &this->mDpethState2);

	this->gDeviceContext->OMSetDepthStencilState(this->mDpethState1, 0);

	return true;
}

void Engine::shutdown()
{
	if (this->vertexShader != nullptr)
	{
		this->vertexShader->Release();
		this->vertexShader = nullptr;
	}

	if (this->pixelShader != nullptr)
	{
		this->pixelShader->Release();
		this->pixelShader = nullptr;
	}

	if (this->inputLayout != nullptr)
	{
		this->inputLayout->Release();
		this->inputLayout = nullptr;
	}

	if (this->indexBuffer != nullptr)
	{
		this->indexBuffer->Release();
		this->indexBuffer = nullptr;
	}

	if (this->gSwapChain != nullptr)
	{
		this->gSwapChain->Release();
		this->gSwapChain = nullptr;
	}

	if (this->gRenderTargetView != nullptr)
	{
		this->gRenderTargetView->Release();
		this->gRenderTargetView = nullptr;
	}

	if (this->mDepthStencilBuffer != nullptr)
	{
		this->mDepthStencilBuffer->Release();
		this->mDepthStencilBuffer = nullptr;
	}

	if (this->mDepthStencilView != nullptr)
	{
		this->mDepthStencilView->Release();
		this->mDepthStencilView = nullptr;
	}

	if (this->matrixBuffer != nullptr)
	{
		this->matrixBuffer->Release();
		this->matrixBuffer = nullptr;
	}

	if (this->gDeviceContext != nullptr)
	{
		this->gDeviceContext->Release();
		this->gDeviceContext = nullptr;
	}

	if (this->gDevice != nullptr)
	{
		this->gDevice->Release();
		this->gDevice = nullptr;
	}

	if (this->SRVtest != nullptr)
	{
		this->SRVtest->Release();
		this->SRVtest = nullptr;
	}

	if (this->bakcGroundSRV != nullptr)
	{
		this->bakcGroundSRV->Release();
		this->bakcGroundSRV = nullptr;
	}

	if (this->samplerState != nullptr)
	{
		this->samplerState->Release();
		this->samplerState = nullptr;
	}
	if (this->mDpethState1 != nullptr)
	{
		this->mDpethState1->Release();
		this->mDpethState1 = nullptr;
	}
	if (this->mDpethState2 != nullptr)
	{
		this->mDpethState2->Release();
		this->mDpethState2 = nullptr;
	}
}

ID3D11Device* Engine::getDevice()
{
	return this->gDevice;
}

ID3D11DeviceContext* Engine::getDeviceContext()
{
	return this->gDeviceContext;
}

void Engine::fillCBuffers(const DirectX::XMMATRIX &modelWorldMatrix, const Camera &gameCamera, bool isSelected)
{
	HRESULT result;

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(dynamicResource);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = false;
	desc.StructureByteStride = 0;

	if (this->matrixBuffer == nullptr)
	{
		//dynamic buffers does not need initial data
		result = gDevice->CreateBuffer(&desc, nullptr, &this->matrixBuffer);
	}

	D3D11_MAPPED_SUBRESOURCE mappedData;

	//map the memory, so that it cant be used in gpu while we change it
	result = gDeviceContext->Map(this->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	//read the data and save them in a variable
	dynamicResource* v = reinterpret_cast<dynamicResource*>(mappedData.pData);

	//finally write the data into the vertex buffer
	v->worldMatrix = modelWorldMatrix;
	v->viewMatrix = gameCamera.getViewMatrix();
	v->projectionMatrix = gameCamera.getProjectionMatrix();
	if (isSelected == true)
	{
		v->extraColor = DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 0);
	}
	else
	{
		v->extraColor = DirectX::XMFLOAT4(0, 0, 0, 0);
	}

	

	gDeviceContext->Unmap(this->matrixBuffer, 0);
}

void Engine::drawObject(Model &toDraw, ID3D11ShaderResourceView *SRV)
{	
	this->gDeviceContext->IASetInputLayout(this->inputLayout);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//send ShaderResources to PixelShader
	if (SRV == nullptr)
	{
		this->gDeviceContext->PSSetShaderResources(0, 1, &this->SRVtest);
	}
	else
	{
		this->gDeviceContext->PSSetShaderResources(0, 1, &SRV);
	}

	//set vertex shader and data
	this->gDeviceContext->VSSetShader(this->vertexShader, nullptr, 0);
	this->gDeviceContext->VSSetConstantBuffers(0, 1, &this->matrixBuffer);
	
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);

	this->gDeviceContext->PSSetShader(this->pixelShader, nullptr, 0);
	this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->matrixBuffer);


	UINT stride = sizeof(Vertex2);
	UINT offset = 0;

	ID3D11Buffer* vBuffer= nullptr;
	vBuffer = toDraw.getVertexBuffer();

	this->gDeviceContext->PSSetSamplers(0, 1, &this->samplerState);
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

	this->gDeviceContext->Draw(toDraw.getNrOfVertex(), 0);
	//HRESULT result = this->gSwapChain->Present(0, 0);
}

void Engine::RenderSkyBox(const Camera & gameCamera)
{
	//this->gDeviceContext->OMSetRenderTargets(1, &gRenderTargetView, NULL);
	this->gDeviceContext->OMSetDepthStencilState(this->mDpethState2, 0);


	Vector3 cameraPos = gameCamera.GetCameraPos();
	this->backGroundModel.setTranslationMatrix(cameraPos);
	this->backGroundModel.update();


	this->fillCBuffers(this->backGroundModel.getWorldModel(), gameCamera, false);

	this->gDeviceContext->IASetInputLayout(this->inputLayout);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//send ShaderResources to PixelShade
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->bakcGroundSRV);


	//set vertex shader and data
	this->gDeviceContext->VSSetShader(this->vertexShader, nullptr, 0);
	this->gDeviceContext->VSSetConstantBuffers(0, 1, &this->matrixBuffer);

	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->pixelShader, nullptr, 0);


	UINT stride = sizeof(Vertex2);
	UINT offset = 0;

	ID3D11Buffer* vBuffer = nullptr;
	vBuffer = this->backGroundModel.getVertexBuffer();

	this->gDeviceContext->PSSetSamplers(0, 1, &this->samplerState);
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

	this->gDeviceContext->Draw(this->backGroundModel.getNrOfVertex(), 0);


	//this->gDeviceContext->OMSetRenderTargets(1, &gRenderTargetView, this->mDepthStencilView);
	this->gDeviceContext->OMSetDepthStencilState(this->mDpethState1, 0);
}

void Engine::clearFrame()
{
	float black[4] = { 0.0f,0.0f,0.0f, 1.0f };

	this->gDeviceContext->ClearRenderTargetView(this->gRenderTargetView, black);
	this->gDeviceContext->ClearDepthStencilView(this->mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Engine::present()
{
	HRESULT result = this->gSwapChain->Present(0, 0);
}
