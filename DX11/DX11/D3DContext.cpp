#include "pch.h"
#include "D3DContext.h"

D3DContext::D3DContext()
	: m_pSwapChain{ nullptr }
	, m_pDevice{ nullptr }
	, m_pDeviceContext{ nullptr }
	, m_pRenderTargetView{ nullptr }
	, m_pDepthStencilBuffer{ nullptr }
	, m_pDepthStencilState{ nullptr }
	, m_pDepthStencilView{ nullptr }
	, m_pRasterState{ nullptr }
	, m_pAlphaEnableState{ nullptr }
	, m_pAlphaDisableState{ nullptr }
{
	m_vsync = false;
	m_videoMemory = 0;
	m_videoName[ 0 ] = '\0';
}

D3DContext::~D3DContext()
{
}

bool D3DContext::Init( int width, int height, bool vsync, HWND hWnd, bool fullscreen, float screenDepth, float screenNear )
{
	m_vsync = vsync;

	HRESULT hr = E_FAIL;

	IDXGIFactory* pFactory = nullptr;
	hr = CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)&pFactory );
	HR_ERROR_RETURN( hr, L"CreateDXGIFactory" );

	IDXGIAdapter* pAdapter = nullptr;
	hr = pFactory->EnumAdapters( 0, &pAdapter );
	HR_ERROR_RETURN( hr, L"EnumAdapters" );

	IDXGIOutput* pAdapterOutput = nullptr;
	hr = pAdapter->EnumOutputs( 0, &pAdapterOutput );
	HR_ERROR_RETURN( hr, L"EnumOutputs" );

	UINT numModes = 0;
	hr = pAdapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr );
	HR_ERROR_RETURN( hr, L"GetDisplayModeList" );

	DXGI_MODE_DESC* pDisplayModeDesc = new DXGI_MODE_DESC[ numModes ];
	hr = pAdapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeDesc );
	HR_ERROR_RETURN( hr, L"GetDisplayModeList" );

	UINT numerator = 0, denominator = 0;
	for( UINT i = 0; i < numModes; ++i )
	{
		if( pDisplayModeDesc[ i ].Width == (unsigned int)width &&
			pDisplayModeDesc[ i ].Height == (unsigned int)height )
		{
			numerator = pDisplayModeDesc[ i ].RefreshRate.Numerator;
			denominator = pDisplayModeDesc[ i ].RefreshRate.Denominator;
			break;
		}
	}
	if( 0 == numerator || 0 == denominator )
		return false;

	DXGI_ADAPTER_DESC adapterDesc;
	hr = pAdapter->GetDesc( &adapterDesc );
	HR_ERROR_RETURN( hr, L"Adatper GetDesc" );

	m_videoMemory = (int)( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );
	size_t stringLength = 0;
	if( 0 != wcstombs_s( &stringLength, m_videoName, 128, adapterDesc.Description, 128 ) )
		return false;

	SAFE_DELETE_ARRAY( pDisplayModeDesc );
	SAFE_RELEASE( pAdapterOutput );
	SAFE_RELEASE( pAdapter );
	SAFE_RELEASE( pFactory );

#pragma region SWAP_CHAIN
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = m_vsync ? numerator : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = m_vsync ? denominator : 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = fullscreen ? false : true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceContext
	);
	HR_ERROR_RETURN( hr, L"D3D11CreateDeviceAndSwapChain" );
#pragma endregion

#pragma region RENDER_TARGET_VIEW
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer );
	HR_ERROR_RETURN( hr, L"ID3D11SwapChain GetBuffer" );

	hr = m_pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &m_pRenderTargetView );
	HR_ERROR_RETURN( hr, L"ID3D11Device CreateRenderTargetView" );

	SAFE_RELEASE( pBackBuffer );
#pragma endregion

#pragma region DEPTH_STENCIL_BUFFER
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory( &depthStencilBufferDesc, sizeof( depthStencilBufferDesc ) );
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	hr = m_pDevice->CreateTexture2D( &depthStencilBufferDesc, nullptr, &m_pDepthStencilBuffer );
	HR_ERROR_RETURN( hr, L"ID3D11Device CreateTexture2D" );
#pragma endregion

#pragma region DEPTH_STENCIL_STATE
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	// Depth
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	// Stencil
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = m_pDevice->CreateDepthStencilState( &depthStencilDesc, &m_pDepthStencilState );
	HR_ERROR_RETURN( hr, L"ID3D11Device CreateDepthStencilState" );

	m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilState, 1 );
#pragma endregion

#pragma region DEPTH_STENCIL_DISABLE_STATE
	D3D11_DEPTH_STENCIL_DESC depthStencilDisableState;
	ZeroMemory( &depthStencilDisableState, sizeof( depthStencilDisableState ) );
	// Depth
	depthStencilDisableState.DepthEnable = false;
	depthStencilDisableState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDisableState.DepthFunc = D3D11_COMPARISON_LESS;
	// Stencil
	depthStencilDisableState.StencilEnable = true;
	depthStencilDisableState.StencilReadMask = 0xFF;
	depthStencilDisableState.StencilWriteMask = 0xFF;
	// Stencil operations if pixel front-facing
	depthStencilDisableState.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisableState.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDisableState.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisableState.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel back-facing
	depthStencilDisableState.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisableState.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDisableState.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDisableState.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = m_pDevice->CreateDepthStencilState( &depthStencilDisableState, &m_pDepthStencilDisableState );
	HR_ERROR_RETURN( hr, L"ID3D11Device CreateDepthStencilDisableState" );
#pragma endregion

#pragma region DEPTH_STENCIL_VIEW
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory( &depthStencilViewDesc, sizeof( depthStencilViewDesc ) );
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_pDevice->CreateDepthStencilView( m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView );
	HR_ERROR_RETURN( hr, L"ID3D11Device CreateDepthStencilView" );

	m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );
#pragma endregion

#pragma region BLEND_STATE
	D3D11_BLEND_DESC blendState;
	ZeroMemory( &blendState, sizeof( blendState ) );
	blendState.RenderTarget[ 0 ].BlendEnable = TRUE;
	blendState.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_ONE;
	blendState.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendState.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendState.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendState.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0f;

	hr = m_pDevice->CreateBlendState( &blendState, &m_pAlphaEnableState );
	HR_CHECK_RETURN( hr, L"ID3D11Device CreateBlendState" );

	blendState.RenderTarget[ 0 ].BlendEnable = FALSE;
	hr = m_pDevice->CreateBlendState( &blendState, &m_pAlphaDisableState );
	HR_CHECK_RETURN( hr, L"ID3D11Device CreateBlendState" );
#pragma endregion

#pragma region RASTERIZER
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( rasterDesc ) );
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = m_pDevice->CreateRasterizerState( &rasterDesc, &m_pRasterState );
	HR_ERROR_RETURN( hr, L"ID3D11Device CreateRasterizerState" );

	m_pDeviceContext->RSSetState( m_pRasterState );
#pragma endregion

#pragma region VIEWPORT
	D3D11_VIEWPORT viewport;
	ZeroMemory( &viewport, sizeof( viewport ) );
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_pDeviceContext->RSSetViewports( 1, &viewport );
#pragma endregion


	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)width / (float)height;

	m_projection = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, screenNear, screenDepth );
	m_world = XMMatrixIdentity();
	m_ortho = XMMatrixOrthographicLH( (float)width, (float)height, 0.1f, 10.0f );
	return true;
}

void D3DContext::Terminate()
{
	if( m_pSwapChain )
		m_pSwapChain->SetFullscreenState( false, nullptr );

	SAFE_RELEASE( m_pRasterState );
	SAFE_RELEASE( m_pAlphaDisableState );
	SAFE_RELEASE( m_pAlphaEnableState );
	SAFE_RELEASE( m_pDepthStencilView );
	SAFE_RELEASE( m_pDepthStencilDisableState );
	SAFE_RELEASE( m_pDepthStencilState );
	SAFE_RELEASE( m_pDepthStencilBuffer );
	SAFE_RELEASE( m_pRenderTargetView );
	SAFE_RELEASE( m_pDeviceContext );
	SAFE_RELEASE( m_pDevice );
	SAFE_RELEASE( m_pSwapChain );
}

void D3DContext::BeginScene( float red, float green, float blue, float alpha )
{
	float color[ 4 ] = { red, green, blue, alpha };

	m_pDeviceContext->ClearRenderTargetView( m_pRenderTargetView, color );
	m_pDeviceContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

void D3DContext::EndScene()
{
	if( m_vsync )
	{
		m_pSwapChain->Present( 1, 0 );
	}
	else
	{
		m_pSwapChain->Present( 0, 0 );
	}
}

ID3D11Device * D3DContext::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext * D3DContext::GetDeviceContext() const
{
	return m_pDeviceContext;
}

void D3DContext::GetProjectionMatrix( XMMATRIX & projection ) const
{
	projection = m_projection;
}

void D3DContext::GetWorldMatrix( XMMATRIX & world ) const
{
	world = m_world;
}

void D3DContext::GetOrthoMatrix( XMMATRIX & ortho ) const
{
	ortho = m_ortho;
}

void D3DContext::GetVideoCardInfo( char * videoCard, int & memory ) const
{
	strcpy_s( videoCard, 128, m_videoName );
	memory = m_videoMemory;
}

bool D3DContext::GetVSync() const
{
	return m_vsync;
}

void D3DContext::ZBufferOn()
{
	m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilState, 1 );
}

void D3DContext::ZBufferOff()
{
	m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilDisableState, 1 );
}

void D3DContext::AlphaBlendOn()
{
	float blend[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->OMSetBlendState( m_pAlphaEnableState, blend, 0xffffffff );

}

void D3DContext::AlphaBlendOff()
{
	float blend[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->OMSetBlendState( m_pAlphaDisableState, blend, 0xffffffff );
}
