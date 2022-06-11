/* FILE       : anim.h
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              .
 */
#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define PL6_MAX_UNITS 3000

#define UNIT_BASE_FIELDS \
  VOID (*Init)( pl6UNIT *Uni, pl6ANIM *Ani );      \
  VOID (*Close)( pl6UNIT *Uni, pl6ANIM *Ani );     \
  VOID (*Response)( pl6UNIT *Uni, pl6ANIM *Ani );  \
  VOID (*Render)( pl6UNIT *Uni, pl6ANIM *Ani )

typedef struct tagpl6UNIT pl6UNIT;
typedef struct tagpl6ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;

  pl6UNIT *Units[PL6_MAX_UNITS];
  INT NumOfUnits;

  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */

} pl6ANIM;

struct tagpl6UNIT
{
  UNIT_BASE_FIELDS;
};

VOID PL6_AnimUnitAdd( pl6UNIT *Uni );
VOID PL6_AnimInit( HWND hWnd );
VOID PL6_AnimClose( VOID );
VOID PL6_AnimResize( INT W, INT H );
VOID PL6_AnimCopyFrame( HDC hDC );
VOID PL6_AnimRender( VOID );
VOID PL6_AnimFlipFullScreen( VOID );
VOID PL6_AnimExit( VOID );
VOID TimerInit( VOID );
VOID TimerResponse( VOID );
pl6UNIT * PL6_AnimUnitCreate( INT Size );

extern pl6ANIM PL6_Anim;

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */