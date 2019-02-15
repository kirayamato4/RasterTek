#pragma once


class LightShaderClass
{
	DELETE_ASSIGN_COPY( LightShaderClass );

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMVECTOR ambientColor;
		XMVECTOR diffuseColor;
		XMVECTOR lightDirection;
	};

public:
	LightShaderClass();
	~LightShaderClass();

	bool Initialize( ID3D11Device* pDevice, HWND hWnd );
	void Shutdown();
	bool Render( 
		ID3D11DeviceContext* pDeviceContext, 
		int indexCount, 
		const XMMATRIX& world, 
		const XMMATRIX& view, 
		const XMMATRIX& projection, 
		ID3D11ShaderResourceView* pTexture, 
		XMFLOAT4 ambientColor,
		XMFLOAT4 diffuseColor, 
		XMFLOAT3 lightDirection 
	);

private:
	bool InitializeShader( ID3D11Device* pDevice, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName );
	void ShutdownShader();
	void OutputShaderErrorMessage( ID3D10Blob* pBlob, HWND hWnd, WCHAR* fileName );

	bool SetShaderParameters( 
		ID3D11DeviceContext* pDeviceContext, 
		const XMMATRIX& world, 
		const XMMATRIX& view, 
		const XMMATRIX& projection, 
		ID3D11ShaderResourceView* pTexture, 
		XMFLOAT4 ambientColor,
		XMFLOAT4 diffuseColor, 
		XMFLOAT3 lightDirection 
	);
	void RenderShader( ID3D11DeviceContext* pDeviceContext, int indexCount );

private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11SamplerState* m_pSamplerState;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pLightBuffer;
};