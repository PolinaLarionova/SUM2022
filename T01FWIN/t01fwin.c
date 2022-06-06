/*PL6 01.06*/
#include <windows.h>

#define WND_CLASS_NAME "My window"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hIstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd, hWnd1;
  MSG msg;
  HBRUSH hbr;

  hbr = CreateSolidBrush(RGB(255, 0, 0));
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = hbr;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_HAND);
  wc.hInstance = hIstance;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;
  
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = 
    CreateWindow(WND_CLASS_NAME, ":)", WS_OVERLAPPEDWINDOW, 30, 30, 1000, 900, NULL, NULL, hIstance, NULL);
  hWnd1 = 
    CreateWindow("BUTTON", "Hello)", WS_CHILD, 300, 300, 300, 100, hWnd, (HMENU)222, hIstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  ShowWindow(hWnd1, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  DeleteObject(hbr);
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  switch (Msg)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
