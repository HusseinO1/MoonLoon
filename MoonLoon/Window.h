#pragma once

#include <Windows.h>
#include "WinBuffer.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();


	bool ProcessMessages();
	HWND GetHWND() const;
	static BITMAPINFO bitmapInfo;
	static WinBuffer winBuffer;

private:
	HINSTANCE hInst;
	HWND hWnd;
};

