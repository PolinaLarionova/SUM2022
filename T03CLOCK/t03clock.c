/* FILE NAME: EYES.C
 * PROGRAMMER: PL6
 * DATE: 02.06.2020
 * PURPOSE: eye
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

/**/
#define WND_CLASS_NAME "My window"
#define MAX 10

/* Forward declaration */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawHand( HDC hDC, INT X, INT Y, DOUBLE angle, INT R, INT t, INT cr, INT cg, INT cb, INT cr1, INT cg1, INT cb1 );
VOID FlipFullScreen( HWND hWnd );

/**/
INT WINAPI WinMain( HINSTANCE hIstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  HBRUSH hbr;

  hbr = CreateSolidBrush(RGB(255, 255, 200));
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = hbr;
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

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  DeleteObject(hbr);
  return msg.wParam;
} /**/

/* Callback enumarate font function */
INT CALLBACK EnumFontFamExProc( ENUMLOGFONTEX *Font, NEWTEXTMETRICEX *lpntme,
                                DWORD FontType, LPARAM lParam )
{
  *(LOGFONT *)lParam = Font->elfLogFont;
  return 0;
} /* End of 'EnumFontFamExProc' function */


/**/
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  INT i, j, r, sizes;
  PAINTSTRUCT ps;
  BITMAP bm;
  SYSTEMTIME st;
  CHAR Buf[102], *Days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thersday", "Friday", "Saturday"}, *Month[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  RECT rc;
  HFONT hFnt, hFntOld;
  MINMAXINFO *minmax;
  POINT pt;
  static LOGFONT Logfont = {0};
  static POINT CursorPt[MAX];
  static INT w, h;
  static HBITMAP hBm, hBmFaceAnd, hBmFaceXor, hBmAnd, hBmXor, hBmFon;
  static HDC hMemDC, hMemFon, hMemSprite, hMemFace;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      PostMessage(hWnd, WM_CLOSE, 0, 0);
    else
      if (wParam == VK_F11)
        FlipFullScreen(hWnd);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 29, 30, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemFon = CreateCompatibleDC(hDC);
    hMemFace = CreateCompatibleDC(hDC);
    hMemSprite = CreateCompatibleDC(hDC);

    strcpy(Logfont.lfFaceName, "Castellar");
    EnumFontFamiliesEx(hDC, &Logfont, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&Logfont, 0);

    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    hBmFaceAnd = LoadImage(NULL, "clockand.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmFaceXor = LoadImage(NULL, "clockxor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmFon = LoadImage(NULL, "fon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAnd = LoadImage(NULL, "and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    for (i = 0; i < MAX; i++)
    {
      CursorPt[i].x = pt.x;
      CursorPt[i].y = pt.y;
    }
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
    /*Fon*/
    GetObject(hBmFon, sizeof(BITMAP), &bm);
    SelectObject(hMemFon, hBmFon);
    for (j = 0; j < h; j += bm.bmHeight)
      for (i = 0; i < w; i += bm.bmWidth)
        BitBlt(hMemDC, i, j, bm.bmWidth, bm.bmHeight, hMemFon, 0, 0, SRCCOPY);

    /*Clockface*/
    GetObject(hBmFaceAnd, sizeof(BITMAP), &bm);
    SelectObject(hMemFace, hBmFaceAnd);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hMemFace, 0, 0, SRCAND);
    SelectObject(hMemFace, hBmFaceXor);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hMemFace, 0, 0, SRCINVERT);

    GetLocalTime(&st);
    r = (INT)(bm.bmWidth / 2 - 600);
    /*Hands*/
    DrawHand(hMemDC, w / 2, h / 2, (st.wHour % 12 + st.wMinute / 60.0 + st.wSecond / 3600.0) * 2 * 3.14 / 12.0, r + 50, 20, 56, 37, 14, 56, 0, 0);
    DrawHand(hMemDC, w / 2, h / 2, (st.wMinute + st.wSecond / 60.0) * 2 * 3.14 / 60.0, r, 14, 56, 37, 14, 114, 0, 0);
    DrawHand(hMemDC, w / 2, h / 2, st.wSecond * 2 * 3.14 / 60.0, r, 8, 223, 30, 38, 255, 166, 85);

    /*Sprite*/
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    GetObject(hBmAnd, sizeof(BITMAP), &bm);
    for (i = MAX - 1; i > 0; i--)
    {
      CursorPt[i].x = CursorPt[i - 1].x;
      CursorPt[i].y = CursorPt[i - 1].y;
      SelectObject(hMemSprite, hBmAnd);
      BitBlt(hMemDC, CursorPt[i].x - bm.bmWidth / 2, CursorPt[i].y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemSprite, 0, 0, SRCAND);
      SelectObject(hMemSprite, hBmXor);
      BitBlt(hMemDC, CursorPt[i].x - bm.bmWidth / 2, CursorPt[i].y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemSprite, 0, 0, SRCINVERT);
    }
    CursorPt[0].x = pt.x;
    CursorPt[0].y = pt.y;
    SelectObject(hMemSprite, hBmAnd);
    BitBlt(hMemDC, pt.x - bm.bmWidth / 2, pt.y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemSprite, 0, 0, SRCAND);
    SelectObject(hMemSprite, hBmXor);
    BitBlt(hMemDC, pt.x - bm.bmWidth / 2, pt.y - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemSprite, 0, 0, SRCINVERT);

    /*Text*/
    sizes = wsprintf(Buf, "%02d %s %d (%s)", st.wDay, Month[st.wMonth], st.wYear, Days[st.wDayOfWeek]);
    SetBkMode(hMemDC, TRANSPARENT);
    /*
    hFnt = CreateFont(80, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
      RUSSIAN_CHARSET,
      OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
      VARIABLE_PITCH| FF_ROMAN, "Castellar");
    */
    Logfont.lfHeight = 100;
    Logfont.lfWeight = 0;
    Logfont.lfWeight = FW_BOLD;
    hFnt = CreateFontIndirect(&Logfont);
    hFntOld = SelectObject(hMemDC, hFnt);
    GetClientRect(hWnd, &rc);
    rc.bottom -= 40;
    SetTextColor(hMemDC, RGB(150, 75, 0));
    DrawText(hMemDC, Buf, sizes, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
    OffsetRect(&rc, -3, -3);
    SetTextColor(hMemDC, RGB(254, 209, 93));
    DrawText(hMemDC, Buf, sizes, &rc, DT_BOTTOM | DT_CENTER | DT_SINGLELINE);
    SelectObject(hMemDC, hFntOld);
    DeleteObject(hFnt);

    hDC = GetDC(hWnd);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_DESTROY:
    if (hBm != NULL)
      DeleteObject(hBm);
    if (hBmFon != NULL)
      DeleteObject(hBmFon);
    if (hBmFaceAnd != NULL)
      DeleteObject(hBmFaceAnd);
    if (hBmFaceXor != NULL)
      DeleteObject(hBmFaceXor);
    if (hBmAnd != NULL)
      DeleteObject(hBmAnd);
    if (hBmXor != NULL)
      DeleteObject(hBmXor);
    DeleteDC(hMemDC);
    DeleteDC(hMemFon);
    DeleteDC(hMemFace);
    DeleteDC(hMemSprite);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/*Draw hand*/
VOID DrawHand( HDC hDC, INT X, INT Y, DOUBLE angle, INT R, INT t, INT cr, INT cg, INT cb, INT cr1, INT cg1, INT cb1 )
{
  DOUBLE si, co, pi = 3.14159265358979323846;
  HPEN hPen;
  INT i;
  POINT pnts[] = {{X, Y}, {X, Y + R * 4 / 5}, {X - t / 2, Y + R * 13 / 15}, {X, Y + R}, {X + t / 2, Y + R * 13 / 15}, {X, Y + R * 4 / 5}, {X, Y + R}}, pnts1[sizeof(pnts) / sizeof(pnts[0])];

  si = sin(angle);
  co = cos(angle);
  for (i = 0; i < sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pnts1[i].x = (pnts[i].x - X) * co - (pnts[i].y - Y) * si + X;
    pnts1[i].y = (pnts[i].y - Y) * co + (pnts[i].x - X) * si + Y;
  }
  hPen = CreatePen(PS_SOLID, t / 4, RGB(cr, cg, cb));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(cr1, cg1, cb1));
  SelectObject(hDC, hPen);
  Polygon(hDC, pnts1, sizeof(pnts) / sizeof(pnts[0]));
  DeleteObject(hPen);
}

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
 
  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;
 
    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRect);
 
    /* Go to full screen mode */
 
    /* oBTAIN NEAREST MONITOR */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);
 
    /* Expand window */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
 
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Restore from full screen mode */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */