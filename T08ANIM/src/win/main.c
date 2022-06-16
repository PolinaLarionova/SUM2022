/* FILE       : main.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 08.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */
#include "pl6.h"

/**/
#define WND_CLASS_NAME "CGSG'Summer Practice'2022"

/* Forward declaration */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hIstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HELP);
  wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
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

  PL6_AnimUnitAdd(PL6_UnitCreateCow());
  PL6_AnimUnitAdd(PL6_UnitCreateControl());

  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 47, 0);

  return msg.wParam;
} /* End of 'WinMain' function */

/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - message type (see WM_***):
 *       UINT Msg;
 *   - message 'word' parameter:
 *       WPARAM wParam;
 *   - message 'long' parameter:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam) -> ptMaxTrackSize.y = 
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      PostMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 29, 30, NULL);
    PL6_AnimInit(hWnd);
    return 0;

  case WM_SIZE:
    PL6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    PL6_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_MOUSEWHEEL:
    PL6_MouseWheel += (SHORT)HIWORD(wParam);
  return 0;

  case WM_TIMER:
    PL6_RndCamSet(VecSet(0, 0, 6), VecSet(0, 0, 0), VecSet(0, 1, 0));

    PL6_AnimRender();

    hDC = GetDC(hWnd);
    PL6_AnimCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
  return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
  return 0;

  case WM_DESTROY:
    KillTimer(hWnd, 29);
    PL6_AnimClose();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}/* End of 'WinFunc' function */

/* END OF 'main.c' FILE */