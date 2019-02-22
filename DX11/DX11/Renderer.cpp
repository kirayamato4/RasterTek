#include "pch.h"
#include "Renderer.h"

Renderer::Renderer()
	: m_pD3DContext{ nullptr }
	, m_pCamera{ nullptr }
	, m_pLight{ nullptr }
	, m_pLightShader{ nullptr }
	, m_pTextureShader{ nullptr }
	, m_pImage{ nullptr }
	, m_pText{ nullptr }
	, m_pCube{ nullptr }
	, m_pModelList{ nullptr }
	, m_pFrustum{ nullptr }
{

}

Renderer::~Renderer()
{
}

bool Renderer::Init( int width, int height, HWND hWnd )
{
	m_pD3DContext = new D3DContext;
	if( nullptr == m_pD3DContext ) return false;
	if( !m_pD3DContext->Init( width, height, RendererValue::VSYNC_ENABLED, hWnd, RendererValue::FULL_SCREEN, RendererValue::SCREEN_DEPTH, RendererValue::SCREEN_NEAR ) )
	{
		MessageBox( hWnd, L"Could not initialize Direct3D", L"Error", MB_OK );
		return false;
	}

	m_pCamera = new Camera;
	if( nullptr == m_pCamera ) return false;
	m_pCamera->SetPosition( 0.0f, 0.0f, -5.0f );
	m_pCamera->Update();

	XMMATRIX view;
	m_pCamera->GetViewMatrix( view );

	m_pLight = new Light;
	if( nullptr == m_pLight ) return false;
	m_pLight->SetAmbientColor( 0.15f, 0.15f, 0.15f, 1.0f );
	m_pLight->SetDiffuseColor( 1.0f, 1.0f, 1.0f, 1.0f );
	m_pLight->SetDirection( 0.0f, 0.0f, 1.0f );
	m_pLight->SetSpecularColor( 1.0f, 1.0f, 1.0f, 1.0f );
	m_pLight->SetSpecularPower( 32.0f );

	m_pLightShader = new LightShader;
	if( nullptr == m_pLightShader ) return false;
	if( !m_pLightShader->Init( GetDevice(), hWnd ) )
	{
		MessageBox( hWnd, L"Could not initialize LightShader", L"Error", MB_OK );
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////

	m_pTextureShader = new TextureShader;
	if( nullptr == m_pTextureShader ) return false;
	if( !m_pTextureShader->Init( GetDevice(), hWnd ) )
		return false;

	m_pImage = new WImage;
	if( !m_pImage->Init( GetDevice(), SIZE{ width, height }, L"Resource/c.dds", SIZE{ 256, 256 } ) )
		return false;

	////////////////////////////////////////////////////////////////////////////////////
	m_pText = new WText;
	if( !m_pText->Init( GetDevice(), GetDeviceContext(), hWnd, width, height, view ) )
		return false;

	////////////////////////////////////////////////////////////////////////////////////

	m_pCube = new Cube;
	if( nullptr == m_pCube ) return false;
	if( !m_pCube->Init( GetDevice(), "Resource/sphere.txt", L"Resource/seafloor.dds" ) )
		return false;

	m_pModelList = new ModelList;
	if( !m_pModelList->Init( 25 ) )
		return false;

	m_pFrustum = new Frustum;

	return true;
}

void Renderer::Terminate()
{
	SAFE_DELETE( m_pFrustum );
	SAFE_TERMINATE( m_pModelList );

	SAFE_TERMINATE( m_pCube );

	SAFE_TERMINATE( m_pText );
	SAFE_TERMINATE( m_pImage );
	SAFE_TERMINATE( m_pTextureShader );

	SAFE_TERMINATE( m_pLightShader );
	SAFE_DELETE( m_pLight );
	SAFE_DELETE( m_pCamera );
	SAFE_TERMINATE( m_pD3DContext );
}

bool Renderer::Update( const POINT& mouse, const int& FPS, const int& CPU, const float& Time )
{
	m_pText->SetMousePosition( mouse, GetDeviceContext() );
	m_pText->SetFPS( FPS, GetDeviceContext() );
	m_pText->SetCPU( CPU, GetDeviceContext() );
	

	m_pCamera->Update();

	static float rotation = 0.0f;

	if( m_pD3DContext->GetVSync() )
	{
		rotation += 3.141592f * 0.005f;
	}
	else
	{
		rotation += 0.0001f;
	}
	
	if( rotation > 360.0f )
		rotation -= 360.0f;

	return Render( rotation );
}

void Renderer::CameraUpdate( unsigned int key )
{
	switch( key )
	{
		case DIK_UP:	m_pCamera->MoveForward();	break;
		case DIK_DOWN:	m_pCamera->MoveBackward();	break;
		case DIK_LEFT:	m_pCamera->MoveLeft();		break;
		case DIK_RIGHT:	m_pCamera->MoveRight();		break;
		case DIK_W: m_pCamera->MoveUp(); break;
		case DIK_S: m_pCamera->MoveDown(); break;
	}

	m_pCamera->Update();
}

bool Renderer::Render( float rotation )
{
	ID3D11Device* pDevice = m_pD3DContext->GetDevice();
	ID3D11DeviceContext* pDeviceContext = m_pD3DContext->GetDeviceContext();

	MatrixBuffer matrixBuffer;
	m_pD3DContext->GetWorldMatrix( matrixBuffer._world );
	m_pCamera->GetViewMatrix( matrixBuffer._view );
	m_pD3DContext->GetProjectionMatrix( matrixBuffer._projection );

	MatrixBuffer orthoBuffer = matrixBuffer;
	m_pD3DContext->GetOrthoMatrix( orthoBuffer._projection );

	CameraBuffer cameraBuffer;
	cameraBuffer._position = m_pCamera->GetPositionVector();
	LightBuffer lightBuffer = m_pLight->GetLightBuffer();

	m_pFrustum->ConstructFrustum( RendererValue::SCREEN_DEPTH, matrixBuffer._projection, matrixBuffer._view );
	size_t modelCount = m_pModelList->GetModelCount();

	float x, y, z;
	XMFLOAT4 color;
	float radius = 1.0f;

	// Render Start
	m_pD3DContext->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	for( size_t i = 0; i < modelCount; ++i )
	{
		m_pModelList->GetData( i, x, y, z, color );

		bool check = m_pFrustum->CheckSphere( x, y, z, radius );
		if( !check ) continue;

		matrixBuffer._world = XMMatrixTranslation( x, y, z );
		matrixBuffer._world *= XMMatrixRotationY( rotation );

		lightBuffer._diffuse = color;


		// Render 3D
		m_pCube->Render( GetDeviceContext() );
		m_pLightShader->Render( GetDeviceContext(), m_pCube->GetIndexCount(), matrixBuffer, cameraBuffer, lightBuffer, m_pCube->GetTexture() );
	}

	// ZBuffer Off
	m_pD3DContext->ZBufferOff();

	// AlphaBlend On
	m_pD3DContext->AlphaBlendOn();

	// Draw Text
	if( !m_pText->Render( GetDeviceContext(), orthoBuffer._world, orthoBuffer._projection ) )
		return false;

	// Alpha Blend Off
	m_pD3DContext->AlphaBlendOff();

	// ZBuffer On
	m_pD3DContext->ZBufferOn();

	m_pD3DContext->EndScene();
	// Render End

	return true;
}

ID3D11Device * Renderer::GetDevice() const
{
	return m_pD3DContext->GetDevice();
}

ID3D11DeviceContext * Renderer::GetDeviceContext() const
{
	return m_pD3DContext->GetDeviceContext();
}
