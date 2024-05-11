////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Manager = 0;

	m_Ground = 0;
	m_Hitter = 0;
	m_Pitcher = 0;
	m_Ball = 0;

	m_LightShader = 0;
	m_Light = 0;

	m_TextureShader = 0;
	m_OutCount[0] = 0;
	m_OutCount[1] = 0;
	m_OutCount[2] = 0;
	//m_OutCount1 = 0;
	//m_OutCount2 = 0;
	//m_OutCount3 = 0;
	m_Zone = 0;
	m_Mouse = 0;

	m_Text = 0;

	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_SkyPlane = 0;
	m_SkyPlaneShader = 0;

	m_ballPos = XMFLOAT3(0.f, 0.f, 0.f);
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	XMMATRIX baseViewMatrix;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	//m_Camera->SetPosition(0.0f, 0.0f, -5.0f);	// for cube
	m_Camera->SetPosition(-70.0f, 6.0f, -70.0f);
	m_Camera->SetRotation(0.f, 45.f, 0.f);
	//m_Camera->SetPosition(0.0f, 0.5f, -3.0f);	// for chair
	
	m_Manager = new GameManagerClass;
	if (!m_Manager->Initialize())
	{
		return false;
	}

	// Create the model object.
	m_Ground = new ModelClass;
	if(!m_Ground)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/seafloor.dds");
	//result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/chair.obj", L"./data/chair_d.dds");
	result = m_Ground->Initialize(m_D3D->GetDevice(), L"./data/ground_1.obj", L"./data/field.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Hitter = new ModelClass;
	if (!m_Hitter)
	{
		return false;
	}

	result = m_Hitter->Initialize(m_D3D->GetDevice(), L"./data/Hitter_t.obj", L"./data/HitterColor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Hitter->SetPosition(-62.f, 0.f, -55.f);

	m_Pitcher = new ModelClass;
	if (!m_Pitcher)
	{
		return false;
	}

	result = m_Pitcher->Initialize(m_D3D->GetDevice(), L"./data/Pitcher_t.obj", L"./data/PitcherColor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Pitcher->SetPosition(-20.f, 5.f, -20.f);

	m_Ball = new ModelClass;
	if (!m_Ball)
	{
		return false;
	}

	result = m_Ball->Initialize(m_D3D->GetDevice(), L"./data/ball.obj", L"./data/baseball2.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Ball->SetPosition(-20.f, 4.f, -20.f);

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
//	m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
//	m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
//	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
//	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		m_OutCount[i] = new BitmapClass;
		if (!m_OutCount[i])
		{
			return false;
		}

		result = m_OutCount[i]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/OutCount.dds", 64, 64);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}
	}

	{
		//// Create the bitmap object.
		//m_OutCount1 = new BitmapClass;
		//if (!m_OutCount1)
		//{
		//	return false;
		//}

		//// Initialize the bitmap object.
		//result = m_OutCount1->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/OutCount.dds", 64, 64);
		//if (!result)
		//{
		//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		//	return false;
		//}

		////m_OutCount1->SetBitmapSize(64, 64);

		//m_OutCount2 = new BitmapClass;
		//if (!m_OutCount2)
		//{
		//	return false;
		//}

		//// Initialize the bitmap object.
		//result = m_OutCount2->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/OutCount.dds", 64, 64);
		//if (!result)
		//{
		//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		//	return false;
		//}

		//m_OutCount3 = new BitmapClass;
		//if (!m_OutCount3)
		//{
		//	return false;
		//}

		//// Initialize the bitmap object.
		//result = m_OutCount3->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/OutCount.dds", 64, 64);
		//if (!result)
		//{
		//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		//	return false;
		//}
	}

	m_Zone = new BitmapClass;
	if (!m_Zone)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Zone->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Zone.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	//m_Zone->SetBitmapSize(64, 64);

	m_Mouse = new BitmapClass;
	if (!m_Mouse)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Mouse->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/OutCount.dds", 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
//	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// 스카이 돔 객체를 초기화 합니다.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// 스카이 돔 쉐이더 객체를 생성합니다.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// 하늘 평면 객체를 생성합니다.
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// 하늘 평면 객체를 초기화 합니다.
	result = m_SkyPlane->Initialize(m_D3D->GetDevice(), L"data/cloud001.dds", L"data/perturb001.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}

	// 하늘 평면 쉐이더 객체를 생성합니다.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// 하늘 평면 쉐이더 객체를 초기화 합니다.
	result = m_SkyPlaneShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// 하늘 평면 쉐이더 객체를 해제합니다.
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// 하늘 평면 객체를 해제합니다.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}

	// 스카이돔 셰이더 제거
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// 스카이 돔 객체 제거
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// 텍스트 제거
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// 마우스 비트맵 제거
	if (m_Mouse)
	{
		m_Mouse->Shutdown();
		delete m_Mouse;
		m_Mouse = 0;
	}

	// 스트라이크 존 비트맵 제거
	if (m_Zone)
	{
		m_Zone->Shutdown();
		delete m_Zone;
		m_Zone = 0;
	}

	// 아웃카운트 비트맵 배열 제거
	for (int i = 3; i > 0; i--)
	{
		if (m_OutCount[i - 1])
		{
			m_OutCount[i - 1]->Shutdown();
			delete m_OutCount[i - 1];
			m_OutCount[i - 1] = 0;
		}
	}

	{
		//// 아웃카운트3 비트맵 제거
		//if (m_OutCount3)
		//{
		//	m_OutCount3->Shutdown();
		//	delete m_OutCount3;
		//	m_OutCount3 = 0;
		//}

		//// 아웃카운트2 비트맵 제거
		//if (m_OutCount2)
		//{
		//	m_OutCount2->Shutdown();
		//	delete m_OutCount2;
		//	m_OutCount2 = 0;
		//}

		//// 아웃카운트1 비트맵 제거
		//if (m_OutCount1)
		//{
		//	m_OutCount1->Shutdown();
		//	delete m_OutCount1;
		//	m_OutCount1 = 0;
		//}
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_Ball)
	{
		m_Ball->Shutdown();
		delete m_Ball;
		m_Ball = 0;
	}

	if (m_Pitcher)
	{
		m_Pitcher->Shutdown();
		delete m_Pitcher;
		m_Pitcher = 0;
	}

	if (m_Hitter)
	{
		m_Hitter->Shutdown();
		delete m_Hitter;
		m_Hitter = 0;
	}

	// Release the model object.
	if (m_Ground)
	{
		m_Ground->Shutdown();
		delete m_Ground;
		m_Ground = 0;
	}

	if (m_Manager)
	{
		m_Manager->Shutdown();
		delete m_Manager;
		m_Manager = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	//bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	/*result = Render(rotation);
	if(!result)
	{
		return false;
	}*/

	return true;
}

bool GraphicsClass::Frame(XMFLOAT3 pos, XMFLOAT3 rot, int fps, int cpu,
	float deltaTime, int screenWidth, int screenHeight)
{
	bool result;
	static float rotation = 0.0f;
	// 공의 이동값
	static float moveValue = 0.f;
	// 공의 위치를 랜덤하게 바꾸기 위한 난수값
	static float randomValue = 0.f;

	m_Camera->SetPosition(pos.x, pos.y, pos.z);
	m_Camera->SetRotation(rot.x, rot.y, rot.z);

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	srand((unsigned int)time(NULL));
	moveValue += 0.03f * deltaTime;
	// 공이 일정 거리 이상 지나가거나 타격됐다면
	if (moveValue > 50.f || m_Manager->IsHit() == true)
	{
		m_Manager->SetHit(false);
		// 공 Z위치 초기화
		moveValue -= 50.f;
		randomValue = (float)(rand() % 21) * 0.1f - 1.f;
	}

	// Set the frames per second.
	result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetScreenSize(screenWidth, screenHeight, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetNumberOfObjects(4, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	m_SkyPlane->Frame();

	// Render the graphics scene.
	result = Render(rotation, moveValue, randomValue);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation, float moveValue, float randomValue)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX worldMatrix2;
	// UI에 사용할 컬러값들
	XMFLOAT4 blackColor = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	XMFLOAT4 whiteColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMFLOAT4 redColor = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	XMFLOAT4 yellowColor = XMFLOAT4(1.f, 0.8f, 0.f, 1.f);
	float posX, posY, posZ;
	//float rotX, rotY, rotZ;
	XMFLOAT3 cameraPosition;
	bool result;
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.6f, 0.8f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(worldMatrix2);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 카메라 위치를 얻는다.
	cameraPosition = m_Camera->GetPosition();

	// 스카이 돔을 카메라 위치를 중심으로 변환합니다.
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// 표면 컬링을 끕니다.
	m_D3D->TurnOffCulling();

	// Z 버퍼를 끈다.
	m_D3D->TurnZBufferOff();

	// 스카이 돔 셰이더를 사용하여 하늘 돔을 렌더링합니다.
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// 다시 표면 컬링을 되돌립니다.
	m_D3D->TurnOnCulling();

	// 구름이 sky dome color와 혼합되도록 첨가물 블렌딩을 가능하게합니다.
	m_D3D->EnableSecondBlendState();

	// 하늘 평면 쉐이더를 사용하여 하늘 평면을 렌더링합니다.
	m_SkyPlane->Render(m_D3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_D3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
		m_SkyPlane->GetBrightness());

	// 블렌드를 끕니다.
	m_D3D->TurnOffAlphaBlending();

	// Z 버퍼를 다시 켭니다.
	m_D3D->TurnZBufferOn();

	// 공 출력
	worldMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	worldMatrix2 = XMMatrixRotationZ(rotation);
	worldMatrix = XMMatrixMultiply(worldMatrix, worldMatrix2);
	m_Ball->GetPosition(posX, posY, posZ);
	// 난수값을 이용해 공의 X와 Y위치를 무작위로 설정
	m_ballPos = XMFLOAT3(randomValue + posX - moveValue, randomValue + posY, posZ - moveValue);
	worldMatrix2 = XMMatrixTranslation(m_ballPos.x, m_ballPos.y, m_ballPos.z);
	worldMatrix = XMMatrixMultiply(worldMatrix, worldMatrix2);

	m_Ball->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Ball->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		m_Ball->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(worldMatrix2);

	// 바닥 출력
	m_Ground->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Ground->GetIndexCount(), 
		worldMatrix, viewMatrix, projectionMatrix,
		m_Ground->GetTexture(), 
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	
	if(!result)
	{
		return false;
	}

	// 타자 출력
	worldMatrix = XMMatrixRotationRollPitchYaw(0.f, -(float)XM_PI * 0.44f, 0.f);
	m_Hitter->GetPosition(posX, posY, posZ);
	worldMatrix2 = XMMatrixTranslation(posX, posY, posZ);
	worldMatrix = XMMatrixMultiply(worldMatrix, worldMatrix2);

	m_Hitter->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Hitter->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		m_Hitter->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(worldMatrix2);

	// 투수 출력
	worldMatrix = XMMatrixRotationRollPitchYaw(0.f, (float)XM_PI * 0.22f, 0.f);
	m_Pitcher->GetPosition(posX, posY, posZ);
	worldMatrix2 = XMMatrixTranslation(posX, posY, posZ);
	worldMatrix = XMMatrixMultiply(worldMatrix, worldMatrix2);

	m_Pitcher->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Pitcher->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		m_Pitcher->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	XMFLOAT4 checkColor = XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	// 타격 타이밍이라면
	if (m_Manager->IsTiming(m_ballPos.z) == true)
	{
		// 마우스 이미지를 빨간색으로 변경
		checkColor = redColor;
	}
	else
	{
		// 아니라면 마우스 이미지를 노란색으로 변경
		checkColor = yellowColor;
	}

	// 마우스 이미지 출력
	result = m_Mouse->Render(m_D3D->GetDeviceContext(), m_mouseX, m_mouseY);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Mouse->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Mouse->GetTexture(), checkColor);
	if (!result)
	{
		return false;
	}

	// 현재 아웃카운트에 따라 아웃카운트 이미지의 색깔 결정
	int OutCount = m_Manager->GetOutCount();
	result = UpdateOutCountColor(worldMatrix, viewMatrix, orthoMatrix, OutCount);
	if (!result)
	{
		return false;
	}

	{
		//// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		//result = m_OutCount1->Render(m_D3D->GetDeviceContext(), 900, 50);
		//if (!result)
		//{
		//	return false;
		//}

		//// Render the bitmap with the texture shader.
		//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_OutCount1->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_OutCount1->GetTexture(), redColor);
		//if (!result)
		//{
		//	return false;
		//}

		//result = m_OutCount2->Render(m_D3D->GetDeviceContext(), 1000, 50);
		//if (!result)
		//{
		//	return false;
		//}

		//// Render the bitmap with the texture shader.
		//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_OutCount2->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_OutCount2->GetTexture(), redColor);
		//if (!result)
		//{
		//	return false;
		//}

		//result = m_OutCount3->Render(m_D3D->GetDeviceContext(), 1100, 50);
		//if (!result)
		//{
		//	return false;
		//}

		//// Render the bitmap with the texture shader.
		//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_OutCount3->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_OutCount3->GetTexture(), redColor);
		//if (!result)
		//{
		//	return false;
		//}
	}

	// 스트라이크 존 이미지 출력
	result = m_Zone->Render(m_D3D->GetDeviceContext(), 512, 400);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Zone->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Zone->GetTexture(), whiteColor);
	if (!result)
	{
		return false;
	}

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::UpdateOutCountColor(XMMATRIX world, XMMATRIX view, XMMATRIX ortho, int count)
{
	XMFLOAT4 blackColor = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	XMFLOAT4 redColor = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	bool result;

	for (int i = 0; i < 3; i++)
	{
		result = m_OutCount[i]->Render(m_D3D->GetDeviceContext(), 900 + i * 100, 50);
		if (!result)
		{
			return false;
		}

		if (i <= count - 1)
		{
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_OutCount[i]->GetIndexCount(), world, view, ortho, m_OutCount[i]->GetTexture(), redColor);
			if (!result)
			{
				return false;
			}
		}
		else
		{
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_OutCount[i]->GetIndexCount(), world, view, ortho, m_OutCount[i]->GetTexture(), blackColor);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

void GraphicsClass::SetMousePosition(int mouseX, int mouseY)
{
	m_mouseX = mouseX;
	m_mouseY = mouseY;
}

void GraphicsClass::TestIntersection(int mouseX, int mouseY)
{
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	XMFLOAT3 direction, origin, rayOrigin, rayDirection;

	// 마우스 커서 좌표를 -1에서 +1 범위로 이동합니다
	float pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	float pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;

	// 뷰포트의 종횡비를 고려하여 투영 행렬을 사용하여 점을 조정합니다
	m_D3D->GetProjectionMatrix(projectionMatrix);

	XMFLOAT3X3 projectionMatrix4;
	XMStoreFloat3x3(&projectionMatrix4, projectionMatrix);

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;

	// 뷰 행렬의 역함수를 구합니다.
	m_Camera->GetViewMatrix(viewMatrix);
	inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	XMFLOAT3X3 inverseViewMatrix4;
	XMStoreFloat3x3(&inverseViewMatrix4, inverseViewMatrix);

	// 뷰 공간에서 피킹 레이의 방향을 계산합니다.
	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	// 카메라의 위치 인 picking ray의 원점을 가져옵니다.
	origin = m_Camera->GetPosition();

	// 세계 행렬을 가져와 공의 위치로 변환합니다.
	m_D3D->GetWorldMatrix(worldMatrix);
	//translateMatrix = XMMatrixTranslation(-62.f, 0.f, -55.f);
	translateMatrix = XMMatrixTranslation(m_ballPos.x, m_ballPos.y, m_ballPos.z);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// 이제 번역 된 행렬의 역함수를 구하십시오.
	inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

	// 이제 광선 원점과 광선 방향을 뷰 공간에서 월드 공간으로 변환합니다.
	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorldMatrix));
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorldMatrix));

	// 광선 방향을 표준화합니다.
	XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));

	// 타격 타이밍에 공이 피킹됐는지 확인
	if (m_Manager->IsTiming(m_ballPos.z) == true)
	{
		if (RaySphereIntersect(rayOrigin, rayDirection, 1.0f) == true)
		{
			// 교차하는 경우 화면에 표시되는 텍스트 문자열에서 교차를 "yes"로 설정합니다.
			m_Text->SetIntersection(true, m_D3D->GetDeviceContext());
			m_Manager->SetHit(true);
		}
		else
		{
			// 그렇지 않으면 "No"로 교차를 설정하십시오.
			m_Text->SetIntersection(false, m_D3D->GetDeviceContext());
			m_Manager->SetHit(false);
			// 아웃카운트 1 추가
			m_Manager->AddOutCount();
			// 아웃카운트가 3이 되면 0으로 초기화
			if (m_Manager->IsThreeOut() == true)
			{
				m_Manager->ResetCount();
			}
		}
	}
}


bool GraphicsClass::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	// a, b 및 c 계수를 계산합니다.
	float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// 결과값을 얻는다
	float discriminant = (b * b) - (4 * a * c);

	// 결과값이 음수이면 피킹 선이 구를 벗어난 것입니다. 그렇지 않으면 구를 선택합니다.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}