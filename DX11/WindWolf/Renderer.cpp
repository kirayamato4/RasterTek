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
	
	return true;
}
void Renderer::Terminate()
{
	SAFE_TERMINATE(m_pShaderMan);
	SAFE_TERMINATE( m_pD3DContext );
}
bool Renderer::Update()
{
	return Render();
}

bool Renderer::Render()
{
	m_pD3DContext->BeginScene( 1.0f, 1.0f, 1.0f, 1.0f );


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