#pragma once

class D3DContext
	: public Allocate16
{
	DELETE_ASSIGN_COPY( D3DContext );

public:
	D3DContext();
	~D3DContext();

	bool Init( int width, int height, bool vsync, HWND hWnd, bool fullscreen, float screenNear, float screenDepth );
	void Terminate();

	void BeginScene( float r, float g, float b, float a );
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	XMMATRIX GetWorld() const;
	XMMATRIX GetProjection() const;
	XMMATRIX GetOrtho() const;

	void GetWorld( XMMATRIX& matrix ) const;
	void GetProjection( XMMATRIX& matrix ) const;
	void GetOrtho( XMMATRIX& matrix ) const;

	bool GetVSync() const;
	bool GetFullScreen() const;

private:
	bool InitDeviceAndSwapChain( HWND hWnd );
	bool InitRenderTargetView();
	bool InitDepthStencilBuffer();
	bool InitDepthStencilState();
	bool InitDepthStencilView();
	bool InitBlendState();
	bool InitRasterizer();
	bool InitViewport();
	bool InitMatrix();

private:
	IDXGISwapChain*				m_pSwapChain = nullptr;
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
	ID3D11RenderTargetView*		m_pRenderTargetView = nullptr;
	ID3D11Texture2D*			m_pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilState*	m_pDepthStencilState = nullptr;
	ID3D11DepthStencilState*	m_pDepthStencilDisableState = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	ID3D11RasterizerState*		m_pRasterState = nullptr;
	ID3D11BlendState*			m_pAlphaEnableState = nullptr;
	ID3D11BlendState*			m_pAlphaDisableState = nullptr;

	XMMATRIX m_projection;
	XMMATRIX m_world;
	XMMATRIX m_ortho;

	int m_width;
	int m_height;
	bool m_vsync;
	bool m_fullscreen;
	float m_screenDepth;
	float m_screenNear;
};