/* FILE NAME: EYES.C
 * PROGRAMMER: PL6
 * DATE: 02.06.2020
 * PURPOSE: eye
 */
#include <stdlib.h>
#include <math.h>
#include <windows.h>

/**/
#define WND_CLASS_NAME "My window"

/* Forward declaration */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawEye( HDC hMemDC, INT x, INT y, INT r, INT rl, INT Mx, INT My );

/**/
INT WINAPI WinMain( HINSTANCE hIstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  HBRUSH hbr;

  hbr = CreateSolidBrush(RGB(255, 0, 0));
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = hbr;
  wc.hCursor = LoadCursor(NULL, IDC_HELP);
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
    CreateWindow(WND_CLASS_NAME, ":)", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 30, 30, 1000, 1000, NULL, NULL, hIstance, NULL);

  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  DeleteObject(hbr);
  return msg.wParam;
}

/**/
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  POINT pt;
  INT i, j;
  PAINTSTRUCT ps;
  static INT w, h;
  static HBITMAP hBm;
  static HDC hMemDC;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 29, 30, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_TIMER:
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 0, 0));
    Rectangle(hMemDC, 0, 0, w, h);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    for (i = 50; i < h; i += 100)
      for (j = 50; j < w; j += 100)
        DrawEye(hMemDC, j, i, 50, 13, pt.x, pt.y);
    hDC = GetDC(hWnd);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_DESTROY:
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteDC(hMemDC);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID DrawEye( HDC hMemDC, INT x, INT y, INT r, INT r1, INT Mx, INT My )
{
  INT x1, y1;
  DOUBLE gi;

  SelectObject(hMemDC, GetStockObject(DC_BRUSH));
  SelectObject(hMemDC, GetStockObject(DC_PEN));
  SetDCBrushColor(hMemDC, RGB(255, 255, 255));
  SetDCPenColor(hMemDC, RGB(0, 0, 0));
  Ellipse(hMemDC, x - r, y - r, x + r, y + r);
  SetDCBrushColor(hMemDC, RGB(0, 0, 0));
  gi = sqrt((Mx - x) * (Mx - x) + (My - y) * (My - y));
  if (gi < r - r1)
  {
    Ellipse(hMemDC, Mx - r1, My - r1, Mx + r1, My + r1);
  } else
  {
    x1 = (INT)(x + (r - r1) * ((Mx - x) / gi));
    y1 = (INT)(y + (r - r1) * ((My - y) / gi));
    Ellipse(hMemDC, x1 - r1, y1 - r1, x1 + r1, y1 + r1);
  }
  
}

