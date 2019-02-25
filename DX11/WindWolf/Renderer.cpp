#include "pch.h"
#include "Renderer.h"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{

}

bool Renderer::Init( int width, int height, HWND hWnd )
{
	m_pD3DContext = new D3DContext;
	if( !m_pD3DContext )
		return false;

	if( !m_pD3DContext->Init( width, height, true, hWnd, false, 0.1f, 1000.0f ) )
		return false;

	m_pShaderMan = new WShaderMan;
	if (nullptr == m_pShaderMan)
		return false;

	if (!m_pShaderMan->Init(GetDevice()))
		return false;

	m_pShaderMan->SetShader(GetDeviceContext(), SHADER_TYPE::COLOR);

	m_pCamera = new Camera;
	if (nullptr == m_pCamera)
		return false;

	m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

	m_pScreen = new WScreen(width, height);
	if (nullptr == m_pScreen)
		return false;

	if (!m_pScreen->Init(GetDevice()))
		return false;
	
	return true;
}
void Renderer::Terminate()
{
	SAFE_TERMINATE(m_pScreen);
	SAFE_DELETE(m_pCamera);
	SAFE_TERMINATE(m_pShaderMan);
	SAFE_TERMINATE( m_pD3DContext );
}
bool Renderer::Update()
{
	m_pCamera->Update();

	ColorMatrixBuffer colorBuffer;
	colorBuffer._world = m_pD3DContext->GetWorld();
	colorBuffer._view = m_pCamera->GetViewMatrix();
	colorBuffer._projection = m_pD3DContext->GetOrtho();
	
	m_pShaderMan->UpdateColorShader(GetDeviceContext(), colorBuffer);

	return Render();
}

bool Renderer::Render()
{
	m_pD3DContext->BeginScene( 1.0f, 1.0f, 1.0f, 1.0f );

	m_pScreen->Render(GetDeviceContext());

	m_pD3DContext->EndScene();

	return true;
}

ID3D11Device* Renderer::GetDevice() const
{
	return m_pD3DContext->GetDevice();
}
ID3D11DeviceContext* Renderer::GetDeviceContext( ) const
{
	return m_pD3DContext->GetDeviceContext();
}