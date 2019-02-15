#pragma once

class Texture
{
	DELETE_ASSIGN_COPY( Texture );

public:
	Texture();
	~Texture();

	bool Init( ID3D11Device* pDevice, const wchar_t* fileName );
	void Terminate();

	ID3D11ShaderResourceView* GetTexture() const;

private:
	ID3D11Resource*				m_pResource;
	ID3D11ShaderResourceView*	m_pTextureView;
	
};