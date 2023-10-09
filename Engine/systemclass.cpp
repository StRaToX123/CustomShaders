////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Timer = 0;
	m_Position = 0;
	Blur_Size = 0.1f;
	Blur_Samples_Count = 10;
	Blur_Standard_Deviation = 0.002f;
	upsamplingLowerLimit = 0.144f;
	upsamplingUpperLimit = 0.531f;
	inside_ImGui = false;
	screenWidth = 0;
	screenHeight = 0;
	hwnd_Pos_X = 0;
	hwnd_Pos_Y = 0;
	ImGui_Loaded = false;
}


SystemClass::SystemClass(const SystemClass& other)
{

}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	bool result;


	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.

	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}
	
	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, &m_hwnd);
	if(!result)
	{
		return false;
	}
	
	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(0.0f, 2.0f, -10.0f);
	
	// IMGUI Initialization
#ifdef _USE_IMGUI_
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(m_Graphics->Get_D3D()->GetDevice(), m_Graphics->Get_D3D()->GetDeviceContext());

	ImGui_Loaded = true;
#endif
	
	


	return true;
}


void SystemClass::Shutdown()
{
	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

#ifdef _USE_IMGUI_
	if (ImGui_Loaded == true)
	{
		// ShutDown ImGUI
		ImGui_Loaded = false;
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
#endif
	
	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemClass::Frame()
{
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	
	// Read the user input.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}
	

	// Update the system stats.
	m_Timer->Frame();

	// Do the frame input processing.
	result = HandleInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}
	
	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(posX, posY, posZ, rotX, rotY, rotZ, Blur_Size, Blur_Samples_Count, Blur_Standard_Deviation, upsamplingLowerLimit, upsamplingUpperLimit);
	if(!result)
	{
		return false;
	}

	
	// ImGUI STUFF
#ifdef _USE_IMGUI_
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
	ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::SliderFloat("Blur Size", &Blur_Size, 0.0f, 0.5f);
	ImGui::SliderInt("Blur Sample Count", &Blur_Samples_Count, 1, 100);
	ImGui::SliderFloat("Blur Standard Deviation", &Blur_Standard_Deviation, 0.001f, 0.3f);
	ImGui::SliderFloat("Upsampling Correction Lower Limit", &upsamplingLowerLimit, 0.0f, 1.0f);
	ImGui::SliderFloat("Upsampling Correction Upper Limitn", &upsamplingUpperLimit, 0.0f, 1.0f);
	ImGui::Text("ShowCursor : %d", Show_Cursor_Return_Value);
	ImGui::End();


	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	
	
	m_Graphics->Get_D3D()->EndScene();
	
	return true;
}

bool SystemClass::HandleInput(float frameTime)
{
	bool keyDown;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the Input
	
	if (m_Input->IsAltPressed())
	{
		if (inside_ImGui == false)
		{
			m_Input->DisableMouse();
			SetCursorPos(hwnd_Pos_X + (screenWidth / 2), hwnd_Pos_Y + (screenHeight / 2));
			Show_Cursor_Return_Value = ShowCursor(true);
			inside_ImGui = true;
		}
	}
	else
	{
		if (inside_ImGui == true)
		{
			Show_Cursor_Return_Value = ShowCursor(false);
			m_Input->EnableMouse();
			inside_ImGui = false;
		}
	}
	
	keyDown = m_Input->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsLeftPressed();
	m_Position->MoveLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->MoveRight(keyDown);

	keyDown = m_Input->IsSpacePressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsCTRLPressed();
	m_Position->MoveDownward(keyDown);

	if (m_Input->IsMouseMovingLeftOrRight())
	{
		m_Position->TurnLeftOrRight(m_Input->GetMouseDelta_X());
	}

	if (m_Input->IsMouseMovingUpOrDown())
	{
		m_Position->TurnUpOrDown(m_Input->GetMouseDelta_Y());
	}
	
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";
	
	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		hwnd_Pos_X = hwnd_Pos_Y = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		hwnd_Pos_X = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		hwnd_Pos_Y = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    hwnd_Pos_X, hwnd_Pos_Y, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
							
	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	EnableWindow(m_hwnd, true);

	// Hide the mouse cursor.
    Show_Cursor_Return_Value = ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{

	if (inside_ImGui == true)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		{
			return true;
		}
	}
	
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		case WM_SYSCOMMAND:
		{
			if ((wparam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			{
				return 0;
			}
			break;
		}
			
		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}