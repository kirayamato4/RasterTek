#pragma once

#define WIN32_LEAN_AND_MEAN

// WindWolf Project.pch
#define DELETE_ASSIGN_CONSTRUCTOR( x ) x& operator = ( const x& ) = delete;
#define DELETE_COPY_CONSTRUCTOR( x ) x( const x& ) = delete;
#define DELETE_ASSIGN_COPY( x ) DELETE_ASSIGN_CONSTRUCTOR( x ) DELETE_COPY_CONSTRUCTOR( x )

#define SAFE_DELETE( x ) if( x ) { delete x; x = nullptr; }
#define SAFE_DELETE_ARRAY( x ) if( x ) { delete[] x; x = nullptr; }
#define SAFE_RELEASE( x ) if( x ) { x->Release(); x = nullptr; }
#define SAFE_TERMINATE( x ) if( x ) { x->Terminate(); delete x; x = nullptr; }

// Linking
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dsound.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "pdh.lib" )

// windows header
#include <Windows.h>
#include <mmsystem.h>

// DirectX header
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <dsound.h>

#include "../_Common/DDSTextureLoader.h"
using namespace DirectX;

// STL header
#include <iostream>
#include <fstream>
#include <sstream>

// C++ header

// Custom

__declspec( align( 16 ) ) struct Allocate16
{
	void* operator new( size_t size )
	{
		return _mm_malloc( size, 16 );
	}

	void operator delete( void* memory )
	{
		_mm_free( memory );
	}
};

#include "Common.h"