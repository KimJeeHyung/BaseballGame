////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"
#include "AlignedAllocationPolicy.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass : public AlignedAllocationPolicy<16>
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, XMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX);
	bool SetFPS(int, ID3D11DeviceContext*);
	bool SetCPU(int, ID3D11DeviceContext*);
	bool SetScreenSize(int, int, ID3D11DeviceContext*);
	bool SetNumberOfObjects(int, ID3D11DeviceContext*);
	bool SetNumberOfPolygons(int, ID3D11DeviceContext*);

	bool SetIntersection(bool, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX, XMMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	XMMATRIX m_baseViewMatrix;
	SentenceType* m_sentence1;	// FPS 텍스트
	SentenceType* m_sentence2;	// CPU 텍스트
	SentenceType* m_sentence3;	// 상호작용 텍스트
	SentenceType* m_sentence4;	// 스크린 사이즈 텍스트
	SentenceType* m_sentence5;	// 폴리곤 수 텍스트
	SentenceType* m_sentence6;	// 오브젝트 수 텍스트
};

#endif