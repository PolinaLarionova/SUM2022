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

#define PL6_GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

#define UNIT_BASE_FIELDS \
  VOID (*Init)( pl6UNIT *Uni, pl6ANIM *Ani );      \
  VOID (*Close)( pl6UNIT *Uni, pl6ANIM *Ani );     \
  VOID (*Response)( pl6UNIT *Uni, pl6ANIM *Ani );  \
  VOID (*Render)( pl6UNIT *Uni, pl6ANIM *Ani )

typedef struct tagpl6UNIT pl6UNIT;
typedef struct tagpl6ANIM
{
  HWND hWnd;
  INT W, H;

  pl6UNIT *Units[PL6_MAX_UNITS];
  INT NumOfUnits;

  /* Timer */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */

  /* keyboard */
  BYTE Keys[256];                /* the state of the keys on the current frame */
  BYTE KeysClick[256];           /* signs of a single keystroke */

  /* mouse */
  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  /* Joystick */
  BYTE
    JBut[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1,0..7] */
  DBL
    JX, JY, JZ, JR;                       /* Joystick axes */

} pl6ANIM;

struct tagpl6UNIT
{
  UNIT_BASE_FIELDS;
};

VOID PL6_AnimUnitAdd( pl6UNIT *Uni );

/* Animation system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_AnimInit( HWND hWnd );

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_AnimClose( VOID );

/* Animation system frame resize handle function.
 * ARGUMENTS: 
 *     - new frame size:
 *          INT W, INT H;
 * RETURNS: None.
 */
VOID PL6_AnimResize( INT W, INT H );

/* Animation system resize window handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_AnimCopyFrame( VOID );

/* Main animation rendering function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_AnimRender( VOID );

/* Goto full screen mode and restore back function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_AnimFlipFullScreen( VOID );
VOID PL6_AnimExit( VOID );

/* Timer initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_TimerInit( VOID );

/* Response timer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_TimerResponse( VOID );

/* Input initialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_AnimInputInit( VOID );

/* Input inter frame events handle function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_AnimInputResponse( VOID );

/* Unit create function.
 * ARGUMENTS:
 *   - size of unit:
 *       INT Size;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_AnimUnitCreate( INT Size );

extern pl6ANIM PL6_Anim;
extern INT PL6_MouseWheel;

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */