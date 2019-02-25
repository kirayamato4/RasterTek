#pragma once

struct WSHADER_PARAM
{
	std::wstring	_path;
	std::string		_entry;
	std::string		_target;

	WSHADER_PARAM();
	explicit WSHADER_PARAM(const WSHADER_PARAM& other);
};

// WShader
class WShader
{
public:
	WShader();
	explicit WShader(const WSHADER_PARAM& param);
	virtual ~WShader();

	virtual void Terminate() = 0;

protected:
	WSHADER_PARAM _param;
};


// WVertexShader
class WVertexShader : public WShader
{
public:
	WVertexShader();
	explicit WVertexShader(const WSHADER_PARAM& param);
	virtual ~WVertexShader();

	virtual bool Init(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutCount);
	virtual void Terminate() override;

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11InputLayout* GetInputLayout() const;

protected:
	virtual bool CompileShader(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutCount);

private:
	ID3D11VertexShader*	m_pVertexShader = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
};


// WPixelShader
class WPixelShader : public WShader
{
public:
	WPixelShader();
	explicit WPixelShader(const WSHADER_PARAM& param);
	virtual ~WPixelShader();

	virtual bool Init(ID3D11Device* pDevice);
	virtual void Terminate() override;

	ID3D11PixelShader* GetPixelShader() const;

protected:
	virtual bool CompileShader(ID3D11Device* pDevice);

private:
	ID3D11PixelShader*	m_pPixelShader = nullptr;
};

