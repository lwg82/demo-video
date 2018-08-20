#include "stdafx.h"
#include "main.h"

LRESULT CALLBACK SceneWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;

	static int cxClient;       // 客户端的宽度和高度，在窗口发生变化的时候，windows都会发送WM_SIZE消息，该消息的lParam高位保存高度，地位保存宽度   
	static int cyClient;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		hdc = GetDC(hwnd);                // 在非WM_PAINT消息里面调用此方法获得HDC   

		ReleaseDC(hwnd, hdc);
	}
	break;

	case WM_SIZE:
	{
		// 在窗口每次发生变化的时候，获取客户端高度和宽度   
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);  // WM_PAINT 消息里面使用该方法获得HDC   




		EndPaint(hwnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);  // 发送 WM_CLOSE, 形参 0 将会传递给 WPARAM     
	}
	break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd); // 销毁窗口       
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

HWND CreateTestWindow(HINSTANCE hInstance)
{
	// 设计一个窗口类       
	WNDCLASS wndclass;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.lpfnWndProc = SceneWndProc;   // 由windows系统调用     
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WINLOGO));
	wndclass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("TestWnd");  // 此处是指定窗口类名称，用于关联窗口，CreateWindow第一个参数     

	if (!RegisterClass(&wndclass))
		return 0;

	return CreateWindow(_T("TestWnd"), _T("TestWnd"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);    // WM_CREATE   
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	HWND hwnd = NULL;

	hwnd = CreateTestWindow(hInstance);


	ShowWindow(hwnd, SW_SHOW);    // WM_SIZE  
								  // 更新窗口    					     
	UpdateWindow(hwnd);    // WM_PAINT   

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(_T("TestWnd"), hInstance);

	return msg.wParam;
}