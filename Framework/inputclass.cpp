////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"

#pragma comment(lib, "dinput8.lib")

InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


void InputClass::Initialize()
{
	int i;
	

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	// 마우스 커서의 위치 지정에 사용될 화면 크기를 저장합니다.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 화면에서 마우스의 위치를 ​​초기화 합니다.
	m_mouseX = 0;
	m_mouseY = 0;

	// 기본 직접 입력 인터페이스를 초기화 합니다.
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 키보드의 직접 입력 인터페이스를 초기화 합니다.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 데이터 형식을 설정합니다. 이 경우 키보드이므로 사전 정의 된 데이터 형식을 사용할 수 있습니다.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정합니다.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// 이제 키보드를 구합니다.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// 마우스의 직접 입력 인터페이스를 초기화 합니다.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정합니다.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// 다른 프로그램과 공유 할 수 있도록 마우스의 협업 수준을 설정합니다.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// 이제 마우스를 구합니다.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return true;
}


void InputClass::Shutdown()
{
	// 마우스를 해제합니다.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// 키보드를 해제합니다.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// 직접 입력으로 기본 인터페이스를 해제합니다.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}


bool InputClass::Frame()
{
	// 키보드의 현재 상태를 읽습니다.
	if (!ReadKeyboard())
	{
		return false;
	}

	// 마우스의 현재 상태를 읽습니다.
	if (!ReadMouse())
	{
		return false;
	}

	// 마우스와 키보드의 변경 사항을 처리합니다.
	ProcessInput();

	return true;
}


bool InputClass::ReadKeyboard()
{
	// 키보드 장치를 읽습니다.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 키보드가 초점을 잃었거나 획득되지 않은 경우 제어권을 다시 얻어옵니다.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool InputClass::ReadMouse()
{
	// 마우스 장치를 읽습니다.
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// 마우스가 초점을 잃었거나 획득되지 않은 경우 컨트롤을 다시 얻어옵니다.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void InputClass::ProcessInput()
{
	// 프레임 동안 마우스 위치의 변경을 기반으로 마우스 커서의 위치를 ​​업데이트 하십시오.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// 마우스 위치가 화면 너비 또는 높이를 초과하지 않는지 확인합니다.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}


void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}


bool InputClass::IsEscapePressed()
{
	// escape 키가 현재 눌려지고 있는지 bit 값을 확인합니다.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsLeftMouseButtonDown()
{
	// 마우스 왼쪽 버튼이 현재 눌러져 있는지 확인합니다.
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}


void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	//return m_keys[key];

	switch (key)
	{
	case 'A':
		return m_keyboardState[DIK_A] & 0x80;
	case 'B':
		return m_keyboardState[DIK_B] & 0x80;
	case 'C':
		return m_keyboardState[DIK_C] & 0x80;
	case 'D':
		return m_keyboardState[DIK_D] & 0x80;
	case 'E':
		return m_keyboardState[DIK_E] & 0x80;
	case 'F':
		return m_keyboardState[DIK_F] & 0x80;
	case 'G':
		return m_keyboardState[DIK_G] & 0x80;
	case 'H':
		return m_keyboardState[DIK_H] & 0x80;
	case 'I':
		return m_keyboardState[DIK_I] & 0x80;
	case 'J':
		return m_keyboardState[DIK_J] & 0x80;
	case 'K':
		return m_keyboardState[DIK_K] & 0x80;
	case 'L':
		return m_keyboardState[DIK_L] & 0x80;
	case 'M':
		return m_keyboardState[DIK_M] & 0x80;
	case 'N':
		return m_keyboardState[DIK_N] & 0x80;
	case 'O':
		return m_keyboardState[DIK_O] & 0x80;
	case 'P':
		return m_keyboardState[DIK_P] & 0x80;
	case 'Q':
		return m_keyboardState[DIK_Q] & 0x80;
	case 'R':
		return m_keyboardState[DIK_R] & 0x80;
	case 'S':
		return m_keyboardState[DIK_S] & 0x80;
	case 'T':
		return m_keyboardState[DIK_T] & 0x80;
	case 'U':
		return m_keyboardState[DIK_U] & 0x80;
	case 'V':
		return m_keyboardState[DIK_V] & 0x80;
	case 'W':
		return m_keyboardState[DIK_W] & 0x80;
	case 'X':
		return m_keyboardState[DIK_X] & 0x80;
	case 'Y':
		return m_keyboardState[DIK_Y] & 0x80;
	case 'Z':
		return m_keyboardState[DIK_Z] & 0x80;
	default:
		break;
	}
}