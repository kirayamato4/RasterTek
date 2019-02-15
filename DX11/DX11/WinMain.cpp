#include "pch.h"
#include "WolfApp.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdShow )
{
	WolfApp* app = new WolfApp();

	if( !app )
		return -1;

	if( !app->Init() )
		return -1;

	app->Run();

	app->Release();

	SAFE_DELETE( app );

	return 0;
}