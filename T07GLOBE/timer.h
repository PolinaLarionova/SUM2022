/* FILE NAME: timer.h
 * PROGRAMMER: PL6
 * DATE: 07.06.2022
 * PURPOSE: Timer declaration module.
 */

#ifndef __timer_h_
#define __timer_h_

#include <windows.h>

/* Global timer data */
extern DOUBLE
  GLB_Time,      /* Full timer in seconds */
  GLB_DeltaTime, /* Interframe time in seconds */
  GLB_FPS;       /* Frame per seconds counter */
extern BOOL
  GLB_IsPause;   /* Pause flag */

/* Timer initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID GLB_TimerInit( VOID );

/* Response timer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID GLB_TimerResponse( VOID );

#endif /* __timer_h_ */

/* END OF 'timer.h' FILE */