//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014. All rights reserved.
// Added some comments (Francisco)
//--------------------------------------------------------------------------------------
#include "Engine.h" //model, camera, globaldata included here
#include "Physics.h"
#include <crtdbg.h.>
#include <vector>

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/*
 Entry point for our program
 This part of the code uses extensively Windows Datatypes, please the following
 link for more detail about them,
 https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
*/
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MSG msg = { 0 };

	// create window
	HWND wndHandle = InitWindow(hInstance);

	//create engine
	Engine engine;

	//create camera, no initialization needed
	Camera gameCamera;

	//create model
	Model planet;

	//timeClock
	//GameTimer gameTime;

	// window is valid
	if (wndHandle)
	{
		// display window
		ShowWindow(wndHandle, nCmdShow);

		//initialize engine
		engine.initialize(&wndHandle);
		
		//initialize model
		planet.initialize(engine.getDevice(), engine.getDeviceContext(), DirectX::XMFLOAT3(0.0f ,0.0f , 2.0f));

		//create bodies
		std::vector<Body>bodies;

		bodies.push_back(Body(&planet, Vector3(0, 0, 2)));
		bodies.push_back(Body(&planet, Vector3(0, 0, 4)));
		bodies.at(1).setVelocity(Vector3(1622, 0.0, 0.0)); // m/s

		bodies.at(0).setMass(5.973 * pow(10, 24));
		bodies.at(1).setMass(7.342 * pow(10, 22));



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
				//gameTime.Tick();

				//update camera with delta time
				gameCamera.Update(0.01f);

				//without rotation
				//for each body
				//engine.clearFrame();

				Physics::doPhysics(bodies.at(0), bodies.at(1));

				for (int i = 0; i < 2; i++)
				{
					//set wordpos
					bodies.at(i).update();

					planet.setTranslationMatrix(bodies.at(i).getPosition());
					planet.update();

					engine.fillCBuffers(planet.getWorldModel(), gameCamera);
					engine.drawObject(planet);

				}

				engine.present();
				engine.clearFrame();
			}
		}

		// finish the program
		DestroyWindow(wndHandle);

		//release resourses
		engine.shutdown();

		planet.shutdown();


	}

	// return how the program finished.
	return (int) msg.wParam;
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