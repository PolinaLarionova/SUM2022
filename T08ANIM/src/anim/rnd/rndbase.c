/* FILE       : rndbase.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 09.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */
#include "rnd.h"

VOID PL6_RndInit( HWND hWnd )
{
  HDC hDC;

  PL6_hRndWnd = hWnd;
  PL6_hRndBmFrame = NULL;
  hDC = GetDC(hWnd);
  PL6_hRndDCFrame = CreateCompatibleDC(hDC);
  PL6_RndCamSet(VecSet(0, 0, 10), VecSet(0, 0, 0), VecSet(0, 1, 0));
  ReleaseDC(hWnd, hDC);
}

VOID PL6_RndClose( VOID )
{
  if (PL6_hRndBmFrame != NULL)
    DeleteObject(PL6_hRndBmFrame);
  DeleteDC(PL6_hRndDCFrame);
}

VOID PL6_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(PL6_hRndWnd);

  if (PL6_hRndBmFrame)
    DeleteObject(PL6_hRndBmFrame);
  PL6_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(PL6_hRndWnd, hDC);
  SelectObject(PL6_hRndDCFrame, PL6_hRndBmFrame);

  /* сохраняем размеры */
  PL6_RndFrameW = W;
  PL6_RndFrameH = H;

  /* пересчитываем проекию */
  PL6_RndProjSet();
}

VOID PL6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = PL6_RndProjSize;
  /* Correct aspect ratio */
  if (PL6_RndFrameW > PL6_RndFrameH)
    rx *= (DBL)PL6_RndFrameW / PL6_RndFrameH;
  else
    ry *= (DBL)PL6_RndFrameH / PL6_RndFrameW;

  PL6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2, PL6_RndProjDist, PL6_RndProjFarClip);
  PL6_RndMatrVP = MatrMulMatr(PL6_RndMatrView, PL6_RndMatrProj);
}

VOID PL6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, PL6_RndFrameW, PL6_RndFrameH, PL6_hRndDCFrame, 0, 0, SRCCOPY);
}

VOID PL6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  PL6_RndMatrView = MatrView(Loc, At, Up);
  PL6_RndMatrVP = MatrMulMatr(PL6_RndMatrView, PL6_RndMatrProj);
}

VOID PL6_RndStart( VOID )
{
  SelectObject(PL6_hRndDCFrame, GetStockObject(DC_PEN));
  SetDCPenColor(PL6_hRndDCFrame, RGB(0, 0, 0));
  SelectObject(PL6_hRndDCFrame, GetStockObject(DC_BRUSH));
  SetDCBrushColor(PL6_hRndDCFrame, RGB(255, 255, 200));
  Rectangle(PL6_hRndDCFrame, 0, 0, PL6_RndFrameW + 1, PL6_RndFrameH + 1);
}

VOID PL6_RndEnd( VOID )
{
}

/* END OF 'rndbase.c' FILE */