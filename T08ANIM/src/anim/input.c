/* FILE       : input.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 11.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

INT PL6_MouseWheel;

static BYTE KeysOld[256];  /* state of the keys on the previous frame */
static BYTE JButOld[32];

static VOID PL6_AnimKeyboardInit( VOID )
{
  GetKeyboardState(PL6_Anim.Keys);
  memcpy(KeysOld, PL6_Anim.Keys, 256);
}
static VOID PL6_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(PL6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    PL6_Anim.Keys[i] >>= 7;
    PL6_Anim.KeysClick[i] = PL6_Anim.Keys[i] && !KeysOld[i];
  }
  memcpy(KeysOld, PL6_Anim.Keys, 256);
}

static VOID PL6_AnimMouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(PL6_Anim.hWnd, &pt);

  /* increment of coordinates on the screen */
  PL6_Anim.Mdx = 0;
  PL6_Anim.Mdy = 0;
  /* absolute values */
  PL6_Anim.Mx = pt.x;
  PL6_Anim.My = pt.y;
}

static VOID PL6_AnimMouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(PL6_Anim.hWnd, &pt);

  /* increment of coordinates on the screen */
  PL6_Anim.Mdx = pt.x - PL6_Anim.Mx;
  PL6_Anim.Mdy = pt.y - PL6_Anim.My;
  /* absolute values */
  PL6_Anim.Mx = pt.x;
  PL6_Anim.My = pt.y;

  PL6_Anim.Mdz = PL6_MouseWheel;
  PL6_Anim.Mz += PL6_MouseWheel;
  PL6_MouseWheel = 0;
}

static VOID PL6_AnimJoystickResponse( VOID )
{
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;
    INT i;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;


      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          PL6_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          PL6_Anim.JButClick[i] = PL6_Anim.JBut[i] && !JButOld[i];
          JButOld[i] = PL6_Anim.JBut[i];
        }
        /* Axes */
        PL6_Anim.JX = PL6_GET_JOYSTIC_AXIS(X);
        PL6_Anim.JY = PL6_GET_JOYSTIC_AXIS(Y);
        PL6_Anim.JZ = PL6_GET_JOYSTIC_AXIS(Z);
        PL6_Anim.JR = PL6_GET_JOYSTIC_AXIS(R);
      }
    }
  }
}

static VOID PL6_AnimJoystickInit( VOID )
{
  INT i;

  PL6_AnimJoystickResponse();
  for (i = 0; i < 32; i++)
    PL6_Anim.JBut[i] = 0;
}

VOID PL6_AnimInputInit( VOID )
{
  PL6_AnimKeyboardInit();
  PL6_AnimMouseInit();
  PL6_AnimJoystickInit();
}

VOID PL6_AnimInputResponse( VOID )
{
  PL6_AnimKeyboardResponse();
  PL6_AnimMouseResponse();
  PL6_AnimJoystickResponse();
}

/* END OF 'input.c' FILE */