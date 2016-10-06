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
		planet.setUniformScale(0.09f);
		//create bodies
		std::vector<Body>bodies;

		//bodies.push_back(Body(&planet, Vector3(0, 0, 0)));
		//bodies.push_back(Body(&planet, Vector3(0, 4, 0)));
		////1022 m/s 1.022 km/s (0.029 works)
		//bodies.at(1).setVelocity(Vector3(1022.0f * SCALE, 0.0, 0.0)); // m/s

		//bodies.at(0).setMass(5.973 * pow(10, 24));
		//bodies.at(1).setMass(7.342 * pow(10, 22));


		bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // sun
		bodies.at(0).setMass(1.98892f * pow(10.0f,30.0f));

		bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // earth
		bodies.at(1).setMass(5.9742 * pow(10, 24));
		bodies.at(1).setPosition(Vector3(-1 * AU, 0, 0));
		bodies.at(1).setVelocity(Vector3(0, 29.783 * 1000, 0));

		bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // Venus
		bodies.at(2).setMass(5.9742 * pow(10, 24) * 0.815);
		bodies.at(2).setPosition(Vector3(0.7 * AU, 0, 0));
		bodies.at(2).setVelocity(Vector3(0, 10.36 * 1000, 0));

		//bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // Mercury
		//bodies.at(1).setMass(5.9742 * pow(10, 24) * 0.055);
		//bodies.at(1).setPosition(Vector3(0.4 * AU, 0, 0));
		//bodies.at(1).setVelocity(Vector3(0, 4.25 * 1000, 0));


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

				int nrOfTicks = 3600;
				//Physics::doPhysics(bodies.at(0), bodies.at(1));
				for (int i = 0; i < nrOfTicks; i++)
				{
					Physics::atracttion(bodies.at(0), bodies.at(1));
					Physics::atracttion(bodies.at(0), bodies.at(2));
					//gameCamera.SetPosition(bodies.at(1).getPosition() * SCALE);

					bodies.at(2).update();
					bodies.at(1).update();
					bodies.at(0).update();
					//Vector3 pos = bodies.at(1).getPosition() * SCALE * 0.01f;
					//planet.setTranslationMatrix(pos);
					//planet.update();

					//engine.fillCBuffers(planet.getWorldModel(), gameCamera);
					//engine.drawObject(planet);




					//pos = bodies.at(0).getPosition() * SCALE * 0.01f;
					//planet.setTranslationMatrix(pos);
					//planet.update();

					//engine.fillCBuffers(planet.getWorldModel(), gameCamera);
					//engine.drawObject(planet);
				}
				for (int i = 0; i < 3; i++)
				{
					Vector3 pos = bodies.at(i).getPosition() * SCALE * 0.04f;
					planet.setTranslationMatrix(pos);
					planet.update();

					engine.fillCBuffers(planet.getWorldModel(), gameCamera);
					engine.drawObject(planet);
				}
				/*for (int i = 0; i < 2; i++)
				{


					//set wordpos
					bodies.at(i).update();
					Vector3 pos = bodies.at(i).getPosition() * SCALE * 0.01f;
					planet.setTranslationMatrix(pos);
					planet.update();

					engine.fillCBuffers(planet.getWorldModel(), gameCamera);
					engine.drawObject(planet);

				}*/


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