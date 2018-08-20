#include "stdafx.h"
#include "main.h"

LRESULT CALLBACK SceneWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;

	static int cxClient;       // �ͻ��˵Ŀ�Ⱥ͸߶ȣ��ڴ��ڷ����仯��ʱ��windows���ᷢ��WM_SIZE��Ϣ������Ϣ��lParam��λ����߶ȣ���λ������   
	static int cyClient;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		hdc = GetDC(hwnd);                // �ڷ�WM_PAINT��Ϣ������ô˷������HDC   

		ReleaseDC(hwnd, hdc);
	}
	break;

	case WM_SIZE:
	{
		// �ڴ���ÿ�η����仯��ʱ�򣬻�ȡ�ͻ��˸߶ȺͿ��   
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);  // WM_PAINT ��Ϣ����ʹ�ø÷������HDC   




		EndPaint(hwnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);  // ���� WM_CLOSE, �β� 0 ���ᴫ�ݸ� WPARAM     
	}
	break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd); // ���ٴ���       
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

HWND CreateTestWindow(HINSTANCE hInstance)
{
	// ���һ��������       
	WNDCLASS wndclass;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.lpfnWndProc = SceneWndProc;   // ��windowsϵͳ����     
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WINLOGO));
	wndclass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("TestWnd");  // �˴���ָ�����������ƣ����ڹ������ڣ�CreateWindow��һ������     

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
								  // ���´���    					     
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