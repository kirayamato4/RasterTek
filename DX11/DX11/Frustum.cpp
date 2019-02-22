#include "pch.h"
#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum( float screenDepth, const XMMATRIX & projection, const XMMATRIX & view )
{
	XMMATRIX _projection = projection;
	XMMATRIX _view = view;

	XMMATRIX matrix;

	// Calculate the minimum Z distance in the frustum.
	float minZ = -_projection.r[ 3 ].m128_f32[ 2 ] / _projection.r[2].m128_f32[2];
	float r = screenDepth / ( screenDepth - minZ );
	_projection.r[ 2 ].m128_f32[ 2 ] = r;
	_projection.r[ 3 ].m128_f32[ 2 ] = -r * minZ;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	matrix = XMMatrixMultiply( _view, _projection );

	// Calculate near plane of frustum.
	m_planes[ PLANE_TYPE::N ].x = matrix.r[ 0 ].m128_f32[ 3 ] + matrix.r[ 0 ].m128_f32[ 2 ];
	m_planes[ PLANE_TYPE::N ].y = matrix.r[ 1 ].m128_f32[ 3 ] + matrix.r[ 1 ].m128_f32[ 2 ];
	m_planes[ PLANE_TYPE::N ].z = matrix.r[ 2 ].m128_f32[ 3 ] + matrix.r[ 2 ].m128_f32[ 2 ];
	m_planes[ PLANE_TYPE::N ].w = matrix.r[ 3 ].m128_f32[ 3 ] + matrix.r[ 3 ].m128_f32[ 2 ];
	XMVECTOR nPlane = XMLoadFloat4( &m_planes[ PLANE_TYPE::N ] );
	m_planeVectors[ PLANE_TYPE::N ] = XMPlaneNormalize( nPlane );
	
	// Calculate far plane of frustum.
	m_planes[ PLANE_TYPE::F ].x = matrix.r[ 0 ].m128_f32[ 3 ] - matrix.r[ 0 ].m128_f32[ 2 ];
	m_planes[ PLANE_TYPE::F ].y = matrix.r[ 1 ].m128_f32[ 3 ] - matrix.r[ 1 ].m128_f32[ 2 ];
	m_planes[ PLANE_TYPE::F ].z = matrix.r[ 2 ].m128_f32[ 3 ] - matrix.r[ 2 ].m128_f32[ 2 ];
	m_planes[ PLANE_TYPE::F ].w = matrix.r[ 3 ].m128_f32[ 3 ] - matrix.r[ 3 ].m128_f32[ 2 ];
	XMVECTOR fPlane = XMLoadFloat4( &m_planes[ PLANE_TYPE::F ] );
	m_planeVectors[ PLANE_TYPE::F ] = XMPlaneNormalize( fPlane );

	// Calculate left plane of frustum.
	m_planes[ PLANE_TYPE::L ].x = matrix.r[ 0 ].m128_f32[ 3 ] + matrix.r[ 0 ].m128_f32[ 0 ];
	m_planes[ PLANE_TYPE::L ].y = matrix.r[ 1 ].m128_f32[ 3 ] + matrix.r[ 1 ].m128_f32[ 0 ];
	m_planes[ PLANE_TYPE::L ].z = matrix.r[ 2 ].m128_f32[ 3 ] + matrix.r[ 2 ].m128_f32[ 0 ];
	m_planes[ PLANE_TYPE::L ].w = matrix.r[ 3 ].m128_f32[ 3 ] + matrix.r[ 3 ].m128_f32[ 0 ];
	XMVECTOR lPlane = XMLoadFloat4( &m_planes[ PLANE_TYPE::L ] );
	m_planeVectors[ PLANE_TYPE::L ] = XMPlaneNormalize( lPlane );

	// Calculate right plane of frustum.
	m_planes[ PLANE_TYPE::R ].x = matrix.r[ 0 ].m128_f32[ 3 ] - matrix.r[ 0 ].m128_f32[ 0 ];
	m_planes[ PLANE_TYPE::R ].y = matrix.r[ 1 ].m128_f32[ 3 ] - matrix.r[ 1 ].m128_f32[ 0 ];
	m_planes[ PLANE_TYPE::R ].z = matrix.r[ 2 ].m128_f32[ 3 ] - matrix.r[ 2 ].m128_f32[ 0 ];
	m_planes[ PLANE_TYPE::R ].w = matrix.r[ 3 ].m128_f32[ 3 ] - matrix.r[ 3 ].m128_f32[ 0 ];
	XMVECTOR rPlane = XMLoadFloat4( &m_planes[ PLANE_TYPE::R ] );
	m_planeVectors[ PLANE_TYPE::R ] = XMPlaneNormalize( rPlane );

	// Calculate left plane of frustum.
	m_planes[ PLANE_TYPE::T ].x = matrix.r[ 0 ].m128_f32[ 3 ] - matrix.r[ 0 ].m128_f32[ 1 ];
	m_planes[ PLANE_TYPE::T ].y = matrix.r[ 1 ].m128_f32[ 3 ] - matrix.r[ 1 ].m128_f32[ 1 ];
	m_planes[ PLANE_TYPE::T ].z = matrix.r[ 2 ].m128_f32[ 3 ] - matrix.r[ 2 ].m128_f32[ 1 ];
	m_planes[ PLANE_TYPE::T ].w = matrix.r[ 3 ].m128_f32[ 3 ] - matrix.r[ 3 ].m128_f32[ 1 ];
	XMVECTOR tPlane = XMLoadFloat4( &m_planes[ PLANE_TYPE::T ] );
	m_planeVectors[ PLANE_TYPE::T ] = XMPlaneNormalize( tPlane );

	// Calculate right plane of frustum.
	m_planes[ PLANE_TYPE::B ].x = matrix.r[ 0 ].m128_f32[ 3 ] + matrix.r[ 0 ].m128_f32[ 1 ];
	m_planes[ PLANE_TYPE::B ].y = matrix.r[ 1 ].m128_f32[ 3 ] + matrix.r[ 1 ].m128_f32[ 1 ];
	m_planes[ PLANE_TYPE::B ].z = matrix.r[ 2 ].m128_f32[ 3 ] + matrix.r[ 2 ].m128_f32[ 1 ];
	m_planes[ PLANE_TYPE::B ].w = matrix.r[ 3 ].m128_f32[ 3 ] + matrix.r[ 3 ].m128_f32[ 1 ];
	XMVECTOR bPlane = XMLoadFloat4( &m_planes[ PLANE_TYPE::B ] );
	m_planeVectors[ PLANE_TYPE::B ] = XMPlaneNormalize( bPlane );
}

bool Frustum::CheckPoint( float x, float y, float z )
{
	auto DotCoord = []( const XMVECTOR& v, const float& x, const float& y, const float& z ) -> float
	{
		return v.m128_f32[ 0 ] * x + v.m128_f32[ 1 ] * y + v.m128_f32[ 2 ] * z + v.m128_f32[ 3 ];
		
	};

	for( size_t i = 0; i < PLANE_TYPE::COUNT; ++i )
	{
		if( DotCoord( m_planeVectors[ i ], x, y, z ) < 0.0f )
			return false;
	}

	return true;
}

bool Frustum::CheckCube( float x, float y, float z, float radius )
{
	auto DotCoord = []( const XMVECTOR& v, const float& x, const float& y, const float& z ) -> float
	{
		return v.m128_f32[ 0 ] * x + v.m128_f32[ 1 ] * y + v.m128_f32[ 2 ] * z + v.m128_f32[ 3 ];
	};

	for( size_t i = 0; i < PLANE_TYPE::COUNT; ++i )
	{
		const XMVECTOR& v = m_planeVectors[ i ];

		if( DotCoord( v, x - radius, y - radius, z - radius ) >= 0.0f )
			continue;

		if( DotCoord( v, x + radius, y - radius, z - radius ) >= 0.0f )
			continue;

		if( DotCoord( v, x - radius, y + radius, z - radius ) >= 0.0f )
			continue;

		if( DotCoord( v, x + radius, y + radius, z - radius ) >= 0.0f )
			continue;

		if( DotCoord( v, x - radius, y - radius, z + radius ) >= 0.0f )
			continue;

		if( DotCoord( v, x + radius, y - radius, z + radius ) >= 0.0f )
			continue;

		if( DotCoord( v, x - radius, y + radius, z + radius ) >= 0.0f )
			continue;

		if( DotCoord( v, x + radius, y + radius, z + radius ) >= 0.0f )
			continue;

		return false;
	}

	return true;
}

bool Frustum::CheckSphere( float x, float y, float z, float radius )
{
	auto DotCoord = []( const XMVECTOR& v, const float& x, const float& y, const float& z ) -> float
	{
		return v.m128_f32[ 0 ] * x + v.m128_f32[ 1 ] * y + v.m128_f32[ 2 ] * z + v.m128_f32[ 3 ];
	};

	for( size_t i = 0; i < PLANE_TYPE::COUNT; ++i )
	{
		if( DotCoord( m_planeVectors[ i ], x, y, z ) < -radius )
			return false;
	}

	return true;
}

bool Frustum::CheckRectangle( float x, float y, float z, float sizeX, float sizeY, float sizeZ )
{
	auto DotCoord = []( const XMVECTOR& v, const float& x, const float& y, const float& z ) -> float
	{
		return v.m128_f32[ 0 ] * x + v.m128_f32[ 1 ] * y + v.m128_f32[ 2 ] * z + v.m128_f32[ 3 ];
	};

	for( size_t i = 0; i < PLANE_TYPE::COUNT; ++i )
	{
		const XMVECTOR& v = m_planeVectors[ i ];

		if( DotCoord( v, x - sizeX, y - sizeY, z - sizeZ ) >= 0.0f )
			continue;

		if( DotCoord( v, x + sizeX, y - sizeY, z - sizeZ ) >= 0.0f )
			continue;

		if( DotCoord( v, x - sizeX, y + sizeY, z - sizeZ ) >= 0.0f )
			continue;

		if( DotCoord( v, x + sizeX, y + sizeY, z - sizeZ ) >= 0.0f )
			continue;

		if( DotCoord( v, x - sizeX, y - sizeY, z + sizeZ ) >= 0.0f )
			continue;

		if( DotCoord( v, x + sizeX, y - sizeY, z + sizeZ ) >= 0.0f )
			continue;

		if( DotCoord( v, x - sizeX, y + sizeY, z + sizeZ ) >= 0.0f )
			continue;

		if( DotCoord( v, x + sizeX, y + sizeY, z + sizeZ ) >= 0.0f )
			continue;

		return false;
	}

	return true;
}
