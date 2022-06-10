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
  //VEC p1, p2, p;
  //MATR m;
  //POINT pnts[2];
  static pl6PRIM Pr1, Pr2;


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
    PL6_RndInit(hWnd);
    //ZeroMemory(Pr2, sizeof(pl6PRIM));
    PL6_RndPrimLoad(&Pr2, "bmp/models/cow.obj");
    Pr2.Trans = MatrScale(VecSet1(0.1));

    PL6_RndPrimCreate(&Pr1, 4, 6);
    Pr1.V[0].P = VecSet(0, 0, 0);
    Pr1.V[1].P = VecSet(2, 0, 0);
    Pr1.V[2].P = VecSet(0, 2, 0);
    Pr1.V[3].P = VecSet(2, 2, 0);

    Pr1.I[0] = 0;
    Pr1.I[1] = 1;
    Pr1.I[2] = 2;

    Pr1.I[3] = 2;
    Pr1.I[4] = 1;
    Pr1.I[5] = 3;
    return 0;

  case WM_SIZE:
    PL6_RndResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    PL6_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_TIMER:
    PL6_RndStart();
    PL6_RndCamSet(VecSet(0, 0, 6), VecSet(0, 0, 0), VecSet(0, 1, 0));

    PL6_RndPrimDraw(&Pr2, MatrRotateY(30 * clock() / 1000.0));
    PL6_RndEnd();
    hDC = GetDC(hWnd);
    PL6_RndCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY:
    KillTimer(hWnd, 29);
    PL6_RndClose();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}/* End of 'WinFunc' function */

/* END OF 'main.c' FILE */