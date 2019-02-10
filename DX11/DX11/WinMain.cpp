#include "pch.h"
#include "SystemClass.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdShow )
{
	SystemClass* system = new SystemClass();

	if( !system )
		return -1;

	if( !system->Initialize() )
		return -1;

	system->Run();

	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}