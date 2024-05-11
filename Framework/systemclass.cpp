////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
	m_Position = 0;
	m_ScreenWidth = 1280;
	m_ScreenHeight = 720;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	//m_Input->Initialize();
	if (!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight))
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
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}
	
	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();
	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();
	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	// 위치 개체를 생성합니다.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// 뷰어의 초기 위치를 초기 카메라 위치와 동일하게 설정합니다.
	m_Position->SetPosition(-70.0f, 6.0f, -70.0f);
	m_Position->SetRotation(0.f, 45.f, 0.f);

	return true;
}


void SystemClass::Shutdown()
{
	// 위치 객체 반환
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

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

	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	result = HandleInput(m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	/*if(m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}*/

	int mouseX = 0;
	int mouseY = 0;
	m_Input->GetMouseLocation(mouseX, mouseY);
	m_Graphics->SetMousePosition(mouseX, mouseY);

	// 시점 위치 / 회전을 가져옵니다.
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Position->GetPosition(pos);
	m_Position->GetRotation(rot);

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(pos, rot, m_Fps->GetFps(),
		m_Cpu->GetCpuPercentage(), m_Timer->GetTime(),
		m_ScreenWidth, m_ScreenHeight);
	if (!result)
	{
		return false;
	}


	return true;
}

bool SystemClass::HandleInput(float frameTime)
{
	// 갱신된 위치를 계산하기 위한 프레임 시간을 설정 합니다.
	m_Position->SetFrameTime(frameTime);

	int mouseX = 0;
	int mouseY = 0;

	// 입력 프레임 처리를 수행합니다.
	if (m_Input->Frame() == false)
	{
		return false;
	}

	// 사용자가 이스케이프를 눌렀을 때 응용 프로그램을 종료 할 것인지 확인합니다.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// 마우스 왼쪽 버튼이 눌러 졌는지 확인하십시오
	if (m_Input->IsLeftMouseButtonDown() == true)
	{
		// 마우스로 화면을 클릭 한 다음 교차 테스트를 수행합니다
		if (m_beginCheck == false)
		{
			m_beginCheck = true;
			m_Input->GetMouseLocation(mouseX, mouseY);
			m_Graphics->TestIntersection(mouseX, mouseY);
		}
	}

	// 왼쪽 마우스 단추가 놓여 졌는지 확인하십시오
	if (m_Input->IsLeftMouseButtonDown() == false)
	{
		m_beginCheck = false;
	}

	// 입력을 처리합니다.
	bool keyDown = m_Input->IsKeyDown('A');
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsKeyDown('D');
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsKeyDown('W');
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsKeyDown('S');
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsKeyDown('Q');
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsKeyDown('E');
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsKeyDown('Z');
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsKeyDown('X');
	m_Position->LookDownward(keyDown);

	keyDown = m_Input->IsKeyDown('R');
	m_Position->ResetPosition(keyDown);

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


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
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = m_ScreenWidth;
		screenHeight = m_ScreenHeight;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

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


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
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

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}