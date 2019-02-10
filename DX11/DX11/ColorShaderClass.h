#pragma once

class ColorShaderClass
	: public Align16
{
	DELETE_ASSIGN_COPY( ColorShaderClass );

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShaderClass();
	~ColorShaderClass();

	

	bool Initialize( ID3D11Device* pDevice, HWND hWnd );
	void Shutdown();
	bool Render( ID3D11DeviceContext* pDeviceContext, int indexCount, const XMMATRIX& world, const  XMMATRIX& view, const XMMATRIX& projection );

private:
	bool InitializeShader( ID3D11Device* pDevice, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName );
	void ShutdownShader();
	void OutputShaderErrorMessage( ID3D10Blob* pBlob, HWND hWnd, WCHAR* fileName );

	bool SetShaderParameters( ID3D11DeviceContext* pDeviceContext, const  XMMATRIX& world, const  XMMATRIX& view, const  XMMATRIX& projection );
	void RenderShader( ID3D11DeviceContext* pDeviceContext, int indexCount );

private:
	ID3D11VertexShader*	m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
	ID3D11InputLayout*	m_pInputLayout;
	ID3D11Buffer*		m_pMatrixBuffer;
};