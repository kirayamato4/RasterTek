#include "pch.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
	: m_pDirect3D{ nullptr }
	, m_pCamera{ nullptr }
	, m_pModel{ nullptr }
	, m_pLightShader{ nullptr }
	, m_pLight{ nullptr }
{

}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize( int width, int height, HWND hWnd )
{
	m_pDirect3D = new D3DClass();

	if( !m_pDirect3D->Initialize( width, height, GCV::VSYNC_ENABLED, hWnd, GCV::FULL_SCREEN, GCV::SCREEN_DEPTH, GCV::SCREEN_NEAR ) )
	{
		MessageBox( hWnd, L"Could not initialize Direct3D", L"Error", MB_OK );
		return false;
	}

	m_pCamera = new CameraClass();
	m_pCamera->SetPosition( 0.0f, 0.0f, -5.0f );

	m_pModel = new ModelClass();
	if( !m_pModel->Initialize( m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext(), "stone01.tga" ) )
	{
		MessageBox( hWnd, L"ModelClass initialize fail", L"Error", MB_OK );
		return false;
	}

	m_pLightShader = new LightShaderClass();
	if( !m_pLightShader->Initialize( m_pDirect3D->GetDevice(), hWnd ) )
	{
		MessageBox( hWnd, L"TextureShaderClass initialize fail", L"Error", MB_OK );
		return false;
	}

	m_pLight = new LightClass();
	m_pLight->SetDiffuseColor( 1.0f, 1.0f, 0.0f, 1.0f );
	m_pLight->SetDirection( 0.0f, 0.0f, 1.0f );

	return true;
}

void GraphicsClass::Shutdown()
{
	SAFE_DELETE( m_pLight );
	SAFE_SHUTDOWN( m_pLightShader );
	SAFE_SHUTDOWN( m_pModel );
	SAFE_DELETE( m_pCamera );
	SAFE_SHUTDOWN( m_pDirect3D );
}

bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;

	rotation += 3.141592f * 0.01f;
	if( rotation > 90.0f )
		rotation -= 180.0f;

	if( !Render( rotation ) )
		return false;

	return true;
}

bool GraphicsClass::Render( float rotation )
{
	XMMATRIX world, view, projection;

	ID3D11Device* pDevice = m_pDirect3D->GetDevice();
	ID3D11DeviceContext* pDeviceContext = m_pDirect3D->GetDeviceContext();

	m_pDirect3D->BeginScene( 0.0f, 0.0f, 1.0f, 1.0f );

	m_pCamera->Render();

	m_pDirect3D->GetWorldMatrix( world );
	m_pCamera->GetViewMatrix( view );
	m_pDirect3D->GetProjectionMatrix( projection );

	world *= XMMatrixRotationY( rotation );

	m_pModel->Render( pDeviceContext );

	if( !m_pLightShader->Render( pDeviceContext, m_pModel->GetIndexCount(), world, view, projection, m_pModel->GetTexture(), m_pLight->GetDiffuseColor(), m_pLight->GetDirectoin() ) )
		return false;

	m_pDirect3D->EndScene();

	return true;
}
