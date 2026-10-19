#include "Common.h"
#include "CustomMath.h"
#include <Windows.h>

#pragma once
class WinBuffer
{
public:
	inline static uchar* memory;
	inline static int w;
	inline static int h;

	inline static void DrawAtSafe(int x, int y, uchar r, uchar g, uchar b)
	{
		if (x >= w || y >= h || x < 0 || y < 0) return;

		memory[4 * (y * w + x) + 0] = b;
		memory[4 * (y * w + x) + 1] = g;
		memory[4 * (y * w + x) + 2] = r;
		memory[4 * (y * w + x) + 3] = 0;
	}

	inline static void Clear(uchar r = 0, uchar g = 0, uchar b = 0)
	{
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				memory[4 * (y * w + x) + 0] = b;
				memory[4 * (y * w + x) + 1] = g;
				memory[4 * (y * w + x) + 2] = r;
				memory[4 * (y * w + x) + 3] = 0;
			}
		}
	}

	inline static void ResetWindowBuffer(WinBuffer* winBuffer, BITMAPINFO* bitmapInfo, HWND wind)
	{
		RECT rect = {};
		GetClientRect(wind, &rect);

		winBuffer->w = rect.right - rect.left;
		winBuffer->h = rect.bottom - rect.top;

		if (winBuffer->memory) delete[] winBuffer->memory;

		winBuffer->memory = new uchar[4 * w * h];

		bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo->bmiHeader.biWidth = winBuffer->w;
		bitmapInfo->bmiHeader.biHeight = -winBuffer->h;
		bitmapInfo->bmiHeader.biPlanes = 1;
		bitmapInfo->bmiHeader.biBitCount = 32;
		bitmapInfo->bmiHeader.biCompression = BI_RGB;
	}

	inline static void FillRect(int x, int y, int width, int height, uchar r, uchar g, uchar b)
	{
		int x0 = CustomMath::Max(0, x);
		int y0 = CustomMath::Max(0, y);
		int x1 = CustomMath::Min(width + x, w);
		int y1 = CustomMath::Min(height + y, h);

		if (x0 >= x1 || y0 >= y1) return;

		for (int j = y0; j < y1; ++j)
		{
			for (int i = x0; i < x1; ++i)
			{
				int idx = 4 * (j * w + i);
				memory[idx + 0] = b;
				memory[idx + 1] = g;
				memory[idx + 2] = r;
				memory[idx + 3] = 0;
			}
		}
	}
};

