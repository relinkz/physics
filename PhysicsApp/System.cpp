//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014. All rights reserved.
// Added some comments (Francisco)
//--------------------------------------------------------------------------------------
#include "Engine.h" //model, camera, globaldata included here
#include "TextRenderer.h"
#include <crtdbg.h.>
#include <vector>
//#include "PhysicsHandler.h"

#include "../physicsDLL/physicsDLL/PhysicsHandler.h"


#pragma comment (lib, "../PhysicsApp/Debug/physicsDLL")


HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//create model
Model ball;
Model box;
Model planet;
//create Parser
Parser parser = Parser();
//create engine
Engine engine;

ID3D11ShaderResourceView* SRV;
ID3D11ShaderResourceView* SRV2;

//PhysicsHandler PH;
PhysicsHandler PH;
 
/*
 Entry point for our program
 This part of the code uses extensively Windows Datatypes, please the following
 link for more detail about them,
 https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
*/
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MSG msg = { 0 };

	// create window
	HWND wndHandle = InitWindow(hInstance);


	//create camera, no initialization needed
	Camera gameCamera;

	//create textHandler
	TextRenderer textHandler;


	//timeClock
	//GameTimer gameTime;


	// window is valid
	if (wndHandle)
	{
		// display window
		ShowWindow(wndHandle, nCmdShow);

		//initialize engine
		engine.initialize(&wndHandle);

		ball.initialize(engine.getDevice(), engine.getDeviceContext(), DirectX::XMFLOAT3(0, 0, 0));
		box.initializeSkyBox(engine.getDevice(), engine.getDeviceContext(), DirectX::XMFLOAT3(0, 5, 0));
		box.setUniformScale(10.0f);


		SRV = (parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "cube_box.tga"));
		SRV2 = (parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "Gray.tga"));


		textHandler.Initialize(engine.getDevice(), engine.getDeviceContext());

		PH.Initialize();

		//CreateEarthOrbitTest();
		//CreateSolarSystem();
		//CreateEarthAndMoon();
		//CreateNewtonsCannonball();

		//infoHandler.setNumOfPlanets(bodies.size());
		//gameTime.Reset();
		// enter message loop, loop until the message WM_QUIT is received.
		while (WM_QUIT != msg.message)
		{
			// read messages
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				// this will call the function WndProc below!
				DispatchMessage(&msg);
			}
			else
			{
;

				//update camera with delta time
				gameCamera.Update(0.01f);

				PH.Update();

				int nrOfObjects = PH.getNrOfComponents();


				box.setTranslationMatrix(Vector3(0, 5, 0));
				box.setUniformScale(10);
				box.update();
				engine.fillCBuffers(box.getWorldModel(), gameCamera, 0);
				engine.drawObject(box, SRV);

				for (int i = 0; i < nrOfObjects; i++)
				{
					PhysicsComponent* ptr = PH.getDynamicComponents(i);
					DirectX::XMVECTOR pos = ptr->m_pos;
					Vector3 posv;
					posv.x = DirectX::XMVectorGetX(pos);
					posv.y = DirectX::XMVectorGetY(pos);
					posv.z = DirectX::XMVectorGetZ(pos);

					ball.setTranslationMatrix(posv);
					ball.setUniformScale(0.5f);
					ball.update();


					engine.fillCBuffers(ball.getWorldModel(), gameCamera, 0);
					engine.drawObject(ball, SRV2);
				}


				engine.present();
				engine.clearFrame();
			}
		}

		// finish the program
		DestroyWindow(wndHandle);

		//release resourses
		//engine.shutdown();
		//
		//planet.shutdown();
		//
		//int size = bodies.size();
		//ID3D11ShaderResourceView *SRVptr = nullptr;
		//for (int i = 0; i < size; i++)
		//{
		//	SRVptr = bodies.at(i).getSRV();
		//	if (SRVptr != nullptr)
		//	{
		//		SRVptr->Release();
		//	}
		//}


	}

	// return how the program finished.
	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance)
{
	// Every window created must belong to a CLASS, so
	// we first create a new class.
	// fill in the WNDCLASSEX structure
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;			// Which function is called for events
	wcex.hInstance      = hInstance;
	wcex.lpszClassName  = L"BasicWindow";
	// use the struct to register the new class.
	// the name of the class is "BasicWindow"
	if( !RegisterClassEx(&wcex) )
		return false;

	// define a struct for the window size
	RECT rc = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
	// create the window to this size and with additional properties
	// (border, menu, etc)
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	
	HWND handle = CreateWindow(
		L"BasicWindow",			// CLASS, if does not exists fails!
		L"Group fab physics",		// Window name (title)
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,		// width
		rc.bottom - rc.top,		// height
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;		
	}

	// if we do not handle the message here, simply call the Default handler function
	return DefWindowProc(hWnd, message, wParam, lParam);
}

