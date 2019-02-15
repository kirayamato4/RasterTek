#include "pch.h"
#include "Renderer.h"

Renderer::Renderer()
	: m_pD3DContext{ nullptr }
	, m_pCamera{ nullptr }
	, m_pLight{ nullptr }
	, m_pLightShader{ nullptr }
	, m_pCube{ nullptr }
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

	m_pCube = new Cube;
	if( nullptr == m_pCube ) return false;
	if( !m_pCube->Init( GetDevice(), "cube.txt", L"a.dds" ) )
		return false;

	return true;
}

void Renderer::Terminate()
{
	SAFE_TERMINATE( m_pCube );

	SAFE_TERMINATE( m_pLightShader );
	SAFE_DELETE( m_pLight );
	SAFE_DELETE( m_pCamera );
	SAFE_TERMINATE( m_pD3DContext );
}

bool Renderer::Update()
{
	static float rotation = 0.0f;
	static float red = 0.0f;

	rotation += 3.141592f * 0.005f;
	if( rotation > 90.0f )
		rotation -= 180.0f;


	m_pCamera->Update();
	

	return Render( rotation );
}

void Renderer::CameraUpdate( unsigned int key )
{
	switch( key )
	{
		case VK_UP:		m_pCamera->MoveForward();	break;
		case VK_DOWN:	m_pCamera->MoveBackward();	break;
		case VK_LEFT:	m_pCamera->MoveLeft();		break;
		case VK_RIGHT:	m_pCamera->MoveRight();		break;
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

	matrixBuffer._world *= XMMatrixRotationY( rotation );

	CameraBuffer cameraBuffer;
	cameraBuffer._position = m_pCamera->GetPositionVector();
	LightBuffer lightbuffer = m_pLight->GetLightBuffer();

	// Render Start
	m_pD3DContext->BeginScene( 0.0f, 0.0f, 1.0f, 1.0f );
	
	m_pLightShader->Render( GetDeviceContext(), m_pCube->GetIndexCount(), matrixBuffer, cameraBuffer, lightbuffer, m_pCube->GetTexture() );

	m_pCube->Render( GetDeviceContext() );

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
