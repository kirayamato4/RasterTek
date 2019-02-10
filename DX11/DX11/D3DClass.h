#pragma once

class D3DClass
	: public Align16
{
	DELETE_ASSIGN_COPY( D3DClass )
public:
	D3DClass();
	~D3DClass();

	bool Initialize( int width, int height, bool vsync, HWND hWnd, bool fullscreen, float screenDepth, float screenNear );
	void Shutdown();

	void BeginScene( float red, float green, float blue, float alpha );
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	void GetProjectionMatrix( XMMATRIX& projection ) const;
	void GetWorldMatrix( XMMATRIX& world ) const;
	void GetOrthoMatrix( XMMATRIX& ortho ) const;

	void GetVideoCardInfo( char* videoCard, int& memory ) const;

private:
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;

	XMMATRIX m_projection;
	XMMATRIX m_world;
	XMMATRIX m_ortho;

	char	m_videoName[ 128 ];
	bool	m_vsync;
	int		m_videoMemory;
};