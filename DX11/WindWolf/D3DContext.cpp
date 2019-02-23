#include "pch.h"
#include "D3DContext.h"

D3DContext::D3DContext()
{
	m_width = 1024;
	m_height = 768;
	m_vsync = false;
	m_fullscreen = false;
	m_screenDepth = 0.0f;
	m_screenNear = 0.0f;
}

D3DContext::~D3DContext()
{

}

bool D3DContext::Init( int width, int height, bool vsync, HWND hWnd, bool fullscreen, float screenNear, float screenDepth )
{
	m_width = width;
	m_height = height;
	m_vsync = vsync;
	m_fullscreen = fullscreen;
	m_screenDepth = screenDepth;
	m_screenNear = screenNear;

	if( !InitDeviceAndSwapChain( hWnd ) )
		return false;
	if( !InitRenderTargetView() )
		return false;
	if( !InitDepthStencilBuffer() )
		return false;
	if( !InitDepthStencilState() )
		return false;
	if( !InitDepthStencilView() )
		return false;
	if( !InitBlendState() )
		return false;
	if( !InitRasterizer() )
		return false;
	if( !InitViewport() )
		return false;
	if( !InitMatrix() )
		return false;

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

void D3DContext::BeginScene( float r, float g, float b, float a )
{
	float color[ 4 ] = { r, g, b, a };

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

ID3D11Device* D3DContext::GetDevice() const
{
	return m_pDevice;
}
ID3D11DeviceContext* D3DContext::GetDeviceContext() const
{
	return m_pDeviceContext;
}
XMMATRIX D3DContext::GetWorld() const
{
	return m_world;
}
XMMATRIX D3DContext::GetProjection() const
{
	return m_projection;
}
XMMATRIX D3DContext::GetOrtho() const
{
	return m_ortho;
}
void D3DContext::GetWorld( XMMATRIX & matrix ) const
{
	matrix = m_world;
}
void D3DContext::GetProjection( XMMATRIX & matrix ) const
{
	matrix = m_projection;
}
void D3DContext::GetOrtho( XMMATRIX & matrix ) const
{
	matrix = m_ortho;
}
bool D3DContext::GetVSync() const
{
	return m_vsync;
}
bool D3DContext::GetFullScreen() const
{
	return m_fullscreen;
}

bool D3DContext::InitDeviceAndSwapChain( HWND hWnd )
{
	HRESULT hr = E_FAIL;

#pragma region FACTORY_DISPLAYMODE
	IDXGIFactory* pFactory = nullptr;
	hr = CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)&pFactory );
	if( FAILED( hr ) )
		return false;

	IDXGIAdapter* pAdapter = nullptr;
	hr = pFactory->EnumAdapters( 0, &pAdapter );
	if( FAILED( hr ) )
	{
		SAFE_RELEASE( pFactory );
		return false;
	}

	IDXGIOutput* pAdapterOutput = nullptr;
	hr = pAdapter->EnumOutputs( 0, &pAdapterOutput );
	if( FAILED( hr ) )
	{
		SAFE_RELEASE( pAdapter );
		SAFE_RELEASE( pFactory );
		return false;
	}

	UINT numModes = 0;
	hr = pAdapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr );
	if( FAILED( hr ) )
	{
		SAFE_RELEASE( pAdapterOutput );
		SAFE_RELEASE( pAdapter );
		SAFE_RELEASE( pFactory );
		return false;
	}

	DXGI_MODE_DESC* pDisplayModeDesc = new DXGI_MODE_DESC[ numModes ];
	hr = pAdapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeDesc );
	if( FAILED( hr ) )
	{
		SAFE_DELETE_ARRAY( pDisplayModeDesc );
		SAFE_RELEASE( pAdapterOutput );
		SAFE_RELEASE( pAdapter );
		SAFE_RELEASE( pFactory );
		return false;
	}

	UINT numerator = 0, denominator = 0;
	for( UINT i = 0; i < numModes; ++i )
	{
		if( pDisplayModeDesc[ i ].Width == (unsigned int)m_width &&
			pDisplayModeDesc[ i ].Height == (unsigned int)m_height )
		{
			numerator = pDisplayModeDesc[ i ].RefreshRate.Numerator;
			denominator = pDisplayModeDesc[ i ].RefreshRate.Denominator;
			break;
		}
	}
	if( 0 == numerator || 0 == denominator )
		return false;

	SAFE_DELETE_ARRAY( pDisplayModeDesc );
	SAFE_RELEASE( pAdapterOutput );
	SAFE_RELEASE( pAdapter );
	SAFE_RELEASE( pFactory );
#pragma endregion

#pragma region CREATE_DEVICE_AND_SWAP_CHAIN
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = m_vsync ? numerator : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = m_vsync ? denominator : 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = m_fullscreen ? false : true;
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
#pragma endregion

	return S_OK == hr;
}

bool D3DContext::InitRenderTargetView()
{
	HRESULT hr = E_FAIL;

	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer );
	if( FAILED( hr ) )
		return false;

	hr = m_pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &m_pRenderTargetView );
	if( FAILED( hr ) )
	{
		SAFE_RELEASE( pBackBuffer );
		return false;
	}

	SAFE_RELEASE( pBackBuffer );

	return S_OK == hr;
}

bool D3DContext::InitDepthStencilBuffer()
{
	HRESULT hr = E_FAIL;

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory( &depthStencilBufferDesc, sizeof( depthStencilBufferDesc ) );
	depthStencilBufferDesc.Width = m_width;
	depthStencilBufferDesc.Height = m_height;
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

	return S_OK == hr;
}

bool D3DContext::InitDepthStencilState()
{
	HRESULT hr = E_FAIL;

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
	if( FAILED( hr ) )
		return false;

	m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilState, 1 );

	// Disable State
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

	return S_OK == hr;
}

bool D3DContext::InitDepthStencilView()
{
	HRESULT hr = E_FAIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory( &depthStencilViewDesc, sizeof( depthStencilViewDesc ) );
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_pDevice->CreateDepthStencilView( m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView );
	if( FAILED( hr ) )
		return false;

	m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

	return S_OK == hr;
}

bool D3DContext::InitBlendState()
{
	HRESULT hr = E_FAIL;

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
	if( FAILED( hr ) )
		return false;

	blendState.RenderTarget[ 0 ].BlendEnable = FALSE;
	hr = m_pDevice->CreateBlendState( &blendState, &m_pAlphaDisableState );
	if( FAILED( hr ) )
		return false;

	return S_OK == hr;
}

bool D3DContext::InitRasterizer()
{
	HRESULT hr = E_FAIL;

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
	if( FAILED( hr ) )
		return false;

	m_pDeviceContext->RSSetState( m_pRasterState );

	return S_OK == hr;
}

bool D3DContext::InitViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory( &viewport, sizeof( viewport ) );
	viewport.Width = (float)m_width;
	viewport.Height = (float)m_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_pDeviceContext->RSSetViewports( 1, &viewport );

	return true;
}

bool D3DContext::InitMatrix()
{
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)m_width / (float)m_height;

	m_projection = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, m_screenNear, m_screenDepth );
	m_world = XMMatrixIdentity();
	m_ortho = XMMatrixOrthographicLH( (float)m_width, (float)m_height, 0.1f, 10.0f );

	return true;
}
