#pragma once

#include "Keyboard.h"
#include "Renderer.h"

class WolfAppBase
{
	DELETE_ASSIGN_COPY( WolfAppBase );
public:
	WolfAppBase( int width, int height, std::wstring appName = L"", std::wstring className = L"" );
	~WolfAppBase();

	bool Init();
	void Terminate();
	void Run();

	LRESULT CALLBACK MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
protected:
	bool Update();
	
	bool InitWindow();
	void TerminateWindow();

protected:
	std::wstring m_appName = L"WindWolf";
	std::wstring m_className = L"WindWolf";

	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_width = 1024;
	int m_height = 768;

	Keyboard* m_pKeyboard = nullptr;

	Renderer* m_pRenderer = nullptr;
};

static WolfAppBase* gWolfApp = nullptr;
static LRESULT CALLBACK MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );