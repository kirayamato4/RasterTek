#include "pch.h"
#include "ModelList.h"

ModelList::ModelList()
	: m_pModelInfoList{ nullptr }
	, m_modelCount{ 0 }
{
}

ModelList::~ModelList()
{
}

bool ModelList::Init( size_t modelCount )
{
	m_modelCount = modelCount;
	m_pModelInfoList = new ModelInfoType[ m_modelCount ];
	if( nullptr == m_pModelInfoList )
		return false;

	srand( (unsigned)::time( nullptr ) );

	float r, g, b;
	for( size_t i = 0; i < m_modelCount; ++i )
	{
		r = (float)rand() / RAND_MAX;
		g = (float)rand() / RAND_MAX;
		b = (float)rand() / RAND_MAX;

		m_pModelInfoList[ i ]._color = XMFLOAT4( r, g, b, 1.0f );

		m_pModelInfoList[ i ]._x = ( ( (float)rand() - (float)rand() ) / RAND_MAX ) * 10.0f;
		m_pModelInfoList[ i ]._y = ( ( (float)rand() - (float)rand() ) / RAND_MAX ) * 10.0f;
		m_pModelInfoList[ i ]._z = ( ( (float)rand() - (float)rand() ) / RAND_MAX ) * 10.0f + 5.0f;
	}

	return true;
}

void ModelList::Terminate()
{
	SAFE_DELETE_ARRAY( m_pModelInfoList );
}

size_t ModelList::GetModelCount() const
{
	return m_modelCount;
}

void ModelList::GetData( size_t index, float & x, float & y, float & z, XMFLOAT4 & color ) const
{
	const ModelInfoType& modelInfo = m_pModelInfoList[ index ];

	x = modelInfo._x;
	y = modelInfo._y;
	z = modelInfo._z;

	color = modelInfo._color;
}
