//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014. All rights reserved.
// Added some comments (Francisco)
//--------------------------------------------------------------------------------------
#include "Engine.h" //model, camera, globaldata included here
#include "Physics.h"
#include "TimeModifier.h"
#include "TextRenderer.h"
#include "InfoHandler.h"
#include <crtdbg.h.>
#include <vector>
#include "PhysicsHandler.h"
#include "scaleModifier.h"

#include "../physicsDLL/physicsDLL/PhysicsLibrary.h"


#pragma comment (lib, "../PhysicsApp/Debug/physicsDLL")


HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SimpleSimulation();
void CompleteSimulation();
void CreateSolarSystem();
void CreateEarthOrbitTest();
void CreateEarthAndMoon();
void CreateNewtonsCannonball();

//create bodies
std::vector<Body>bodies;
//create model
Model ball;
Model box;
Model planet;
//create Parser
Parser parser = Parser();
//create engine
Engine engine;
//scaleHandler
scaleModifier scaleMod = scaleModifier();

ID3D11ShaderResourceView* SRV;
ID3D11ShaderResourceView* SRV2;

//PhysicsHandler PH;
PhysicsLibrary::PhysicsHandler PH;
 
TimeModifier timeModifier;
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

	//create infoHandler for presenting active planet
	InfoHandler infoHandler;

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

				PhysicsLibrary::PhysicsComponent* ptr = PH.getPComponents();

				box.setTranslationMatrix(Vector3(0, 5, 0));
				box.setUniformScale(10);
				box.update();
				engine.fillCBuffers(box.getWorldModel(), gameCamera, 0);
				engine.drawObject(box, SRV);

				for (int i = 0; i < nrOfObjects; i++)
				{
					DirectX::XMVECTOR pos = ptr[i].GetPos();
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

void SimpleSimulation()
{
	//only sun affects bodies

	int nrOfBodies = bodies.size();
	//index 0 == sun
	for (int i = 1; i < nrOfBodies; i++)
	{
		Physics::attraction(bodies.at(0), bodies.at(i), timeModifier.getTime());
		bodies.at(i).update(timeModifier.getTime());
	}
}

void CompleteSimulation()
{
	//all bodies affects all bodies

	int nrOfBodies = bodies.size();
	//index 0 == sun
	for (int i = 0; i < nrOfBodies; i++)
	{
		for (int z = 0; z < nrOfBodies; z++)
		{
			if (i != z)
			{
				Physics::attraction(bodies.at(i), bodies.at(z), timeModifier.getTime());
			}
		}
		bodies.at(i).update(timeModifier.getTime());
	}
}

void CreateSolarSystem()
{

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // sun
	bodies.at(0).setName("Sun");
	bodies.at(0).setMass(1.98892f * pow(10.0f, 30.0f));
	bodies.at(0).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "Sun.tga"));
	bodies.at(0).setSize(1.0f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); //mercery
	bodies.at(1).setName("Mercery");
	bodies.at(1).setMass(3.3022* pow(10, 23));
	bodies.at(1).setPosition(Vector3(-0.387098 * AU, 0, 0));
	bodies.at(1).setVelocity(Vector3(0, 41.87 * 1000, 0)); //47.87 km/s from wiki
	bodies.at(1).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "mercurycyl2.tga"));
	bodies.at(1).setSize(0.5 * 0.055);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // Venus
	bodies.at(2).setName("Venus");
	bodies.at(2).setMass(4.8685 * pow(10, 24));
	bodies.at(2).setPosition(Vector3(-0.7 * AU, 0, 0));
	bodies.at(2).setVelocity(Vector3(0, 29.02 * 1000, 0)); // 35.02 km/s from wiki
	bodies.at(2).setSize(0.5f * 0.815f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // earth
	bodies.at(3).setName("Earth");
	bodies.at(3).setMass(5.9742 * pow(10, 24));
	bodies.at(3).setPosition(Vector3(-1.0f * AU, 0, 0));
	bodies.at(3).setVelocity(Vector3(0, 23.783 * 1000, 0)); //29.783 km/s from wiki
	bodies.at(3).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(3).setSize(0.5f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); //Mars
	bodies.at(4).setName("Mars");
	bodies.at(4).setMass(6.4174 * pow(10, 23));
	bodies.at(4).setPosition(Vector3(-1.523 * AU, 0, 0)); //halv storaxel?
	bodies.at(4).setVelocity(Vector3(0, 18.077 * 1000, 0)); // 24.077 km/s
	bodies.at(4).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "MarsMap_2500x1250.tga"));
	bodies.at(4).setSize(0.5f * 0.1f);
}

void CreateEarthOrbitTest()
{
	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // sun
	bodies.at(0).setName("Sun");
	bodies.at(0).setMass(1.98892f * pow(10.0f, 30.0f));
	bodies.at(0).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "Sun.tga"));
	bodies.at(0).setSize(1.0f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // earth
	bodies.at(1).setName("Earth");
	bodies.at(1).setMass(5.9742 * pow(10, 24));
	bodies.at(1).setPosition(Vector3(-0.98329 * AU, 0, 0));
	bodies.at(1).setVelocity(Vector3(0, -29.783 * 1000, 0));
	bodies.at(1).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(1).setSize(0.5f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // earth
	bodies.at(2).setName("Earth");
	bodies.at(2).setMass(5.9742 * pow(10, 24));
	bodies.at(2).setPosition(Vector3(-0.98329 * AU, 0, 0));
	bodies.at(2).setVelocity(Vector3(0, 0, 0));
	bodies.at(2).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(2).setSize(0.5f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // earth
	bodies.at(3).setName("Earth");
	bodies.at(3).setMass(5.9742 * pow(10, 24));
	bodies.at(3).setPosition(Vector3(1.0167 * AU, 0, 0));
	bodies.at(3).setVelocity(Vector3(0, 0, 0));
	bodies.at(3).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(3).setSize(0.5f);



}

void CreateEarthAndMoon()
{
	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // earth
	bodies.at(0).setName("Earth");
	bodies.at(0).setMass(5.9742 * pow(10, 24));
	bodies.at(0).setPosition(Vector3(0, 0, 0));
	bodies.at(0).setVelocity(Vector3(0, 0, 0));
	bodies.at(0).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(0).setSize(0.5f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // Moon
	bodies.at(1).setName("Moon");
	bodies.at(1).setMass(7.342 * pow(10, 22));
	bodies.at(1).setPosition(Vector3(384400000, 0, 0));
	bodies.at(1).setVelocity(Vector3(0, 1022, 0));
	bodies.at(1).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(1).setSize(0.1f);
}

void CreateNewtonsCannonball()
{
	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // earth
	bodies.at(0).setName("Earth");
	bodies.at(0).setMass(5.9742 * pow(10, 24));
	bodies.at(0).setPosition(Vector3(0, 0, 0));
	bodies.at(0).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(0).setSize(0.5f);

	bodies.push_back(Body(&planet, Vector3(0, 0, 0))); // coannonball
	bodies.at(1).setName("coannonball");
	bodies.at(1).setMass(50);
	bodies.at(1).setPosition(Vector3(-7378 * 1000, 0, 0));
	bodies.at(1).setVelocity(Vector3(0, 7000, 0));
	bodies.at(1).setSRV(parser.LoadTarga(engine.getDevice(), engine.getDeviceContext(), "PathfinderMap.tga"));
	bodies.at(1).setSize(0.1f);
	
	scaleMod.setModifier(60.0);

}
