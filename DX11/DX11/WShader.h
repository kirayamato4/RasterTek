#pragma once

struct WSHADER_PARAM
{
	std::wstring	_path;
	std::string		_entry;
	std::string		_target;
};

class WShader
{
public:
	explicit WShader( const WSHADER_PARAM& VS, const WSHADER_PARAM& PS );
	virtual ~WShader();

	bool Init( ID3D11Device* pDevice );
	void Terminate();

	ID3D11VertexShader*	GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;

protected:
	bool InitVertexShader( ID3D11Device*& pDevice );
	bool InitPixelShader( ID3D11Device*& pDevice );

protected:
	WSHADER_PARAM _VS;
	WSHADER_PARAM _PS;

	ID3D11VertexShader*	m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
};