#pragma once

#include "WFont.h"
#include "FontShader.h"

class WText
	: public Align16
{
	DELETE_ASSIGN_COPY( WText );
public:
	WText();
	~WText();

	bool Init( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, HWND hWnd, int width, int height, XMMATRIX& view );
	void Terminate();
	bool Render( ID3D11DeviceContext* pDeviceContext, const XMMATRIX& world, const XMMATRIX& ortho );

private:
	bool InitSentence( SentenceType** ppSentence, size_t maxLength, ID3D11Device* pDevice );
	bool UpdateSentence( SentenceType* pSentence, char* text, int x, int y, float red, float green, float blue, ID3D11DeviceContext* pDeviceContext );
	void TerminateSentence( SentenceType** ppSentence );
	bool RenderSentence( ID3D11DeviceContext* pDeviceContext, SentenceType* pSentence, const XMMATRIX& world, const XMMATRIX& ortho );

private:
	WFont* m_pFont;
	FontShader* m_pFontShader;
	int m_screenWidth;
	int m_screenHeight;
	XMMATRIX m_view;

	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};