/* FILE       : anim.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              .
 */
#include <stdio.h>

#include "anim.h"

pl6ANIM PL6_Anim;

VOID PL6_AnimUnitAdd( pl6UNIT *Uni )
{
  if (PL6_Anim.NumOfUnits < PL6_MAX_UNITS)
    PL6_Anim.Units[PL6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &PL6_Anim);
} /* End of 'PL6_AnimUnitAdd' function */

/* Animation system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_AnimInit( HWND hWnd )
{
  PL6_Anim.hWnd = hWnd;
  PL6_RndInit(hWnd);
  PL6_TimerInit();
  PL6_AnimInputInit();
} /* End of 'PL6_AnimInit' function */

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < PL6_Anim.NumOfUnits; i++)
  {
    PL6_Anim.Units[i]->Close(PL6_Anim.Units[i], &PL6_Anim);
    free(PL6_Anim.Units[i]);
    PL6_Anim.Units[i] = NULL;
  }
  PL6_Anim.NumOfUnits = 0;
  PL6_RndClose();
} /* End of 'PL6_AnimClose' function */

/* Animation system frame resize handle function.
 * ARGUMENTS: 
 *     - new frame size:
 *          INT W, INT H;
 * RETURNS: None.
 */
VOID PL6_AnimResize( INT W, INT H )
{
  PL6_RndResize(W, H);
  PL6_Anim.W = W;
  PL6_Anim.H = H;
} /* End of 'PL6_AnimResize' function */

/* Animation system resize window handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_AnimCopyFrame( VOID )
{
  PL6_RndCopyFrame();
}/* End of 'PL6_AnimCopyFrame' function */

/* Main animation rendering function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_AnimRender( VOID )
{
  INT i;

  PL6_TimerResponse();
  PL6_AnimInputResponse();

  for (i = 0; i < PL6_Anim.NumOfUnits; i++)
    PL6_Anim.Units[i]->Response(PL6_Anim.Units[i], &PL6_Anim);

  PL6_RndStart();
  for (i = 0; i < PL6_Anim.NumOfUnits; i++)
    PL6_Anim.Units[i]->Render(PL6_Anim.Units[i], &PL6_Anim);
  PL6_RndEnd();
} /* End of 'PL6_AnimRender' function */

/* Goto full screen mode and restore back function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
 
  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;
 
    /* Save old window size and position */
    GetWindowRect(PL6_Anim.hWnd, &SaveRect);
 
    /* Go to full screen mode */
 
    /* oBTAIN NEAREST MONITOR */
    hmon = MonitorFromWindow(PL6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);
 
    /* Expand window */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(PL6_Anim.hWnd, GWL_STYLE), FALSE);
 
    SetWindowPos(PL6_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Restore from full screen mode */
    SetWindowPos(PL6_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'PL6_AnimFlipFullScreen' function */

VOID PL6_AnimExit( VOID )
{
  PostMessage(PL6_Anim.hWnd, WM_CLOSE, 0, 0);
}/* End of 'PL6_AnimExit' function */

/* END OF 'anim.c' FILE */

