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

	// ��ī�� �� ��ü�� �ʱ�ȭ �մϴ�.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// ��ī�� �� ���̴� ��ü�� �����մϴ�.
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

	// �ϴ� ��� ��ü�� �����մϴ�.
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// �ϴ� ��� ��ü�� �ʱ�ȭ �մϴ�.
	result = m_SkyPlane->Initialize(m_D3D->GetDevice(), L"data/cloud001.dds", L"data/perturb001.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}

	// �ϴ� ��� ���̴� ��ü�� �����մϴ�.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// �ϴ� ��� ���̴� ��ü�� �ʱ�ȭ �մϴ�.
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
	// �ϴ� ��� ���̴� ��ü�� �����մϴ�.
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// �ϴ� ��� ��ü�� �����մϴ�.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}

	// ��ī�̵� ���̴� ����
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// ��ī�� �� ��ü ����
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// �ؽ�Ʈ ����
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// ���콺 ��Ʈ�� ����
	if (m_Mouse)
	{
		m_Mouse->Shutdown();
		delete m_Mouse;
		m_Mouse = 0;
	}

	// ��Ʈ����ũ �� ��Ʈ�� ����
	if (m_Zone)
	{
		m_Zone->Shutdown();
		delete m_Zone;
		m_Zone = 0;
	}

	// �ƿ�ī��Ʈ ��Ʈ�� �迭 ����
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
		//// �ƿ�ī��Ʈ3 ��Ʈ�� ����
		//if (m_OutCount3)
		//{
		//	m_OutCount3->Shutdown();
		//	delete m_OutCount3;
		//	m_OutCount3 = 0;
		//}

		//// �ƿ�ī��Ʈ2 ��Ʈ�� ����
		//if (m_OutCount2)
		//{
		//	m_OutCount2->Shutdown();
		//	delete m_OutCount2;
		//	m_OutCount2 = 0;
		//}

		//// �ƿ�ī��Ʈ1 ��Ʈ�� ����
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
	// ���� �̵���
	static float moveValue = 0.f;
	// ���� ��ġ�� �����ϰ� �ٲٱ� ���� ������
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
	// ���� ���� �Ÿ� �̻� �������ų� Ÿ�ݵƴٸ�
	if (moveValue > 50.f || m_Manager->IsHit() == true)
	{
		m_Manager->SetHit(false);
		// �� Z��ġ �ʱ�ȭ
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
	// UI�� ����� �÷�����
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

	// ī�޶� ��ġ�� ��´�.
	cameraPosition = m_Camera->GetPosition();

	// ��ī�� ���� ī�޶� ��ġ�� �߽����� ��ȯ�մϴ�.
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// ǥ�� �ø��� ���ϴ�.
	m_D3D->TurnOffCulling();

	// Z ���۸� ����.
	m_D3D->TurnZBufferOff();

	// ��ī�� �� ���̴��� ����Ͽ� �ϴ� ���� �������մϴ�.
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// �ٽ� ǥ�� �ø��� �ǵ����ϴ�.
	m_D3D->TurnOnCulling();

	// ������ sky dome color�� ȥ�յǵ��� ÷���� ������ �����ϰ��մϴ�.
	m_D3D->EnableSecondBlendState();

	// �ϴ� ��� ���̴��� ����Ͽ� �ϴ� ����� �������մϴ�.
	m_SkyPlane->Render(m_D3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_D3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
		m_SkyPlane->GetBrightness());

	// ���带 ���ϴ�.
	m_D3D->TurnOffAlphaBlending();

	// Z ���۸� �ٽ� �մϴ�.
	m_D3D->TurnZBufferOn();

	// �� ���
	worldMatrix = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	worldMatrix2 = XMMatrixRotationZ(rotation);
	worldMatrix = XMMatrixMultiply(worldMatrix, worldMatrix2);
	m_Ball->GetPosition(posX, posY, posZ);
	// �������� �̿��� ���� X�� Y��ġ�� �������� ����
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

	// �ٴ� ���
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

	// Ÿ�� ���
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

	// ���� ���
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

	// Ÿ�� Ÿ�̹��̶��
	if (m_Manager->IsTiming(m_ballPos.z) == true)
	{
		// ���콺 �̹����� ���������� ����
		checkColor = redColor;
	}
	else
	{
		// �ƴ϶�� ���콺 �̹����� ��������� ����
		checkColor = yellowColor;
	}

	// ���콺 �̹��� ���
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

	// ���� �ƿ�ī��Ʈ�� ���� �ƿ�ī��Ʈ �̹����� ���� ����
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

	// ��Ʈ����ũ �� �̹��� ���
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

	// ���콺 Ŀ�� ��ǥ�� -1���� +1 ������ �̵��մϴ�
	float pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	float pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;

	// ����Ʈ�� ��Ⱦ�� ����Ͽ� ���� ����� ����Ͽ� ���� �����մϴ�
	m_D3D->GetProjectionMatrix(projectionMatrix);

	XMFLOAT3X3 projectionMatrix4;
	XMStoreFloat3x3(&projectionMatrix4, projectionMatrix);

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;

	// �� ����� ���Լ��� ���մϴ�.
	m_Camera->GetViewMatrix(viewMatrix);
	inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	XMFLOAT3X3 inverseViewMatrix4;
	XMStoreFloat3x3(&inverseViewMatrix4, inverseViewMatrix);

	// �� �������� ��ŷ ������ ������ ����մϴ�.
	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	// ī�޶��� ��ġ �� picking ray�� ������ �����ɴϴ�.
	origin = m_Camera->GetPosition();

	// ���� ����� ������ ���� ��ġ�� ��ȯ�մϴ�.
	m_D3D->GetWorldMatrix(worldMatrix);
	//translateMatrix = XMMatrixTranslation(-62.f, 0.f, -55.f);
	translateMatrix = XMMatrixTranslation(m_ballPos.x, m_ballPos.y, m_ballPos.z);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	// ���� ���� �� ����� ���Լ��� ���Ͻʽÿ�.
	inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

	// ���� ���� ������ ���� ������ �� �������� ���� �������� ��ȯ�մϴ�.
	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorldMatrix));
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorldMatrix));

	// ���� ������ ǥ��ȭ�մϴ�.
	XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));

	// Ÿ�� Ÿ�ֿ̹� ���� ��ŷ�ƴ��� Ȯ��
	if (m_Manager->IsTiming(m_ballPos.z) == true)
	{
		if (RaySphereIntersect(rayOrigin, rayDirection, 1.0f) == true)
		{
			// �����ϴ� ��� ȭ�鿡 ǥ�õǴ� �ؽ�Ʈ ���ڿ����� ������ "yes"�� �����մϴ�.
			m_Text->SetIntersection(true, m_D3D->GetDeviceContext());
			m_Manager->SetHit(true);
		}
		else
		{
			// �׷��� ������ "No"�� ������ �����Ͻʽÿ�.
			m_Text->SetIntersection(false, m_D3D->GetDeviceContext());
			m_Manager->SetHit(false);
			// �ƿ�ī��Ʈ 1 �߰�
			m_Manager->AddOutCount();
			// �ƿ�ī��Ʈ�� 3�� �Ǹ� 0���� �ʱ�ȭ
			if (m_Manager->IsThreeOut() == true)
			{
				m_Manager->ResetCount();
			}
		}
	}
}


bool GraphicsClass::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	// a, b �� c ����� ����մϴ�.
	float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// ������� ��´�
	float discriminant = (b * b) - (4 * a * c);

	// ������� �����̸� ��ŷ ���� ���� ��� ���Դϴ�. �׷��� ������ ���� �����մϴ�.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}