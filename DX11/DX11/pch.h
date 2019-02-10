#pragma once

// pre-processing
#define DELETE_ASSIGN_CONSTRUCTOR( x )  x& operator =( const x& ) = delete;
#define DELETE_COPY_CONSTRUCTOR( x ) x( const x& ) = delete;
#define DELETE_ASSIGN_COPY( x ) DELETE_ASSIGN_CONSTRUCTOR( x ) DELETE_COPY_CONSTRUCTOR( x )

#define SAFE_RELEASE( x ) if( x ) x->Release();
#define SAFE_DELETE( x ) if( x ) { delete x; x = nullptr; }
#define SAFE_DELETE_ARRAY( x ) if( x ) { delete[] x; x = nullptr; }


#define WIND32_LEAN_AND_MEAN

// linking
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

// Windows header
#include <Windows.h>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

// C++ header
#include <iostream>
