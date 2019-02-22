#pragma once

class ModelList
{
	DELETE_ASSIGN_COPY( ModelList );

public:
	ModelList();
	~ModelList();

	bool Init( size_t modelCount );
	void Terminate();

	size_t GetModelCount() const;
	void GetData( size_t index, float& x, float& y, float& z, XMFLOAT4& color ) const;

private:
	size_t m_modelCount;
	ModelInfoType* m_pModelInfoList;
};