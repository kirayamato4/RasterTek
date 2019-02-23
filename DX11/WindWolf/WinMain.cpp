#include "pch.h"

#include "WolfAppBase.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdShow )
{
	WolfAppBase* app = new WolfAppBase( 1024, 768 );

	if( !app )
		return -1;

	if( !app->Init() )
		return -1;

	app->Run();

	SAFE_TERMINATE( app );

	return 0;
}