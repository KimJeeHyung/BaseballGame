////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "GameManagerClass.h"
#include "SkyDomeClass.h"
#include "SkyDomeShaderClass.h"
#include "skyplaneclass.h"
#include "skyplaneshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(XMFLOAT3, XMFLOAT3, int, int, float, int, int);

private:
	bool Render(float, float, float);
	bool UpdateOutCountColor(XMMATRIX, XMMATRIX, XMMATRIX, int);

public:
	void SetMousePosition(int, int);
	void TestIntersection(int, int);
	bool RaySphereIntersect(XMFLOAT3, XMFLOAT3, float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	GameManagerClass* m_Manager;

	// Models
	ModelClass* m_Ground;
	ModelClass* m_Hitter;
	ModelClass* m_Pitcher;
	ModelClass* m_Ball;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_OutCount[3];
	//BitmapClass* m_OutCount1;
	//BitmapClass* m_OutCount2;
	//BitmapClass* m_OutCount3;
	BitmapClass* m_Zone;
	BitmapClass* m_Mouse;

	TextClass* m_Text;

	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	SkyPlaneClass* m_SkyPlane;
	SkyPlaneShaderClass* m_SkyPlaneShader;

private:
	int m_screenWidth = 0;
	int m_screenHeight = 0;

	int m_mouseX = 0;
	int m_mouseY = 0;

	XMFLOAT3 m_ballPos;
};

#endif