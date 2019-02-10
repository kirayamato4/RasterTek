#pragma once

// pre-processing
#define DELETE_ASSIGN_CONSTRUCTOR( x )  x& operator =( const x& ) = delete;
#define DELETE_COPY_CONSTRUCTOR( x ) x( const x& ) = delete;
#define DELETE_ASSIGN_COPY( x ) DELETE_ASSIGN_CONSTRUCTOR( x ) DELETE_COPY_CONSTRUCTOR( x )

#define SAFE_RELEASE( x ) if( x ) x->Release();


#define WIND32_LEAN_AND_MEAN
// Windows header
#include <Windows.h>

// C++ header
#include <iostream>
