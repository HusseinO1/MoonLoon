#include "Window.h"
#include "WinBuffer.h"
#include "resource.h"
WinBuffer Window::winBuffer{};
BITMAPINFO Window::bitmapInfo{};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(hWnd, &Paint);

			HDC hdc = GetDC(hWnd);

			StretchDIBits(
				hdc,
				0, 0, Window::winBuffer.w, Window::winBuffer.h,
				0, 0, Window::winBuffer.w, Window::winBuffer.h,
				Window::winBuffer.memory,
				&Window::bitmapInfo,
				DIB_RGB_COLORS,
				SRCCOPY
			);

			ReleaseDC(hWnd, hdc);
			
			EndPaint(hWnd, &Paint);

			break;
		}
		case WM_SIZE:
		{
			WinBuffer::ResetWindowBuffer(&Window::winBuffer, &Window::bitmapInfo, hWnd);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window()
{
    hInst = GetModuleHandle(nullptr);
    const wchar_t* className = L"MoonLoon";

    // Load the icon directly from the project directory
    HICON hIcon = (HICON)LoadImageW(
        NULL,
        L"MoonImage.ico", // Located directly in your working directory
        IMAGE_ICON,
        0, 0,
        LR_LOADFROMFILE | LR_DEFAULTSIZE
    );

    WNDCLASS wndClass = {};
    wndClass.hInstance = hInst;
    wndClass.lpszClassName = className;
    wndClass.hIcon = hIcon; // <-- Assign loaded handle to wndClass
    wndClass.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClass(&wndClass);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    int width = 1280;
    int height = 720;

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    hWnd = CreateWindowEx(
        0,
        className,
        L"MoonLoon",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInst,
        nullptr
    );

    // Explicitly set the taskbar and title bar icons on the created window
    if (hIcon)
    {
        SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessageW(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

    WinBuffer::ResetWindowBuffer(&Window::winBuffer, &Window::bitmapInfo, hWnd);

    ShowWindow(hWnd, SW_SHOW);
}

Window::~Window()
{
	const wchar_t* className = L"MoonLoon";

	UnregisterClass(className, hInst);
}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

HWND Window::GetHWND() const
{
	return hWnd;
}
