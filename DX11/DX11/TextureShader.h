#pragma once

class TextureShader
{
	DELETE_ASSIGN_COPY( TextureShader );

public:
	TextureShader();
	~TextureShader();

	bool Init( ID3D11Device* pDevice, HWND hWnd );
	void Terminate();
	bool Render( ID3D11DeviceContext* pDeviceContext, int indexCount, MatrixBuffer& matrixBuffer, ID3D11ShaderResourceView* pTexture );

private:
	bool InitShader( ID3D11Device* pDevice, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName );
	void TerminateShader();
	void OutputShaderErrorMessage( ID3D10Blob* pBlob, HWND hWnd, WCHAR* fileName );

	bool SetShaderParam( ID3D11DeviceContext* pDeviceContext, MatrixBuffer& matrixBuffer, ID3D11ShaderResourceView* pTexture );
	void RenderShader( ID3D11DeviceContext* pDeviceContext, int indexCount );

private:
	ID3D11VertexShader*	m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
	ID3D11InputLayout*	m_pInputLayout;
	ID3D11Buffer*		m_pMatrixBuffer;

	ID3D11SamplerState* m_pSamplerState;
};