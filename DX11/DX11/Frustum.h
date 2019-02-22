#pragma once

class Frustum
	: public Align16
{
	DELETE_ASSIGN_COPY( Frustum );

public:
	enum PLANE_TYPE : size_t
	{
		N = 0,		// NEAR
		F,			// FAR
		L,			// LEFT
		R,			// RIGHT
		T,			// TOP
		B,			// BOTTOM
		COUNT		// PLANE COUNT
	};

	Frustum();
	~Frustum();

	void ConstructFrustum( float screenDepth, const XMMATRIX& projection, const XMMATRIX& view );

	bool CheckPoint( float x, float y, float z );
	bool CheckCube( float x, float y, float z, float radius );
	bool CheckSphere( float x, float y, float z, float radius );
	bool CheckRectangle( float x, float y, float z, float sizeX, float sizeY, float sizeZ );

private:
	XMFLOAT4 m_planes[ PLANE_TYPE::COUNT ];
	XMVECTOR m_planeVectors[ PLANE_TYPE::COUNT ];
};