/* FILE NAME: timer.c
 * PROGRAMMER: PL6
 * DATE: 07.06.2022
 * PURPOSE: Timer implementation module.
 */

#include <time.h>

#include "timer.h"

/* Global timer data */
DOUBLE
  GLB_Time,      /* Full timer in seconds */
  GLB_DeltaTime, /* Interframe time in seconds */
  GLB_FPS;       /* Frame per seconds counter */
BOOL
  GLB_IsPause;   /* Pause flag */

/* Local timer data */
static LONG
  StartTime,  /* Program start time in clocks */
  OldTime,    /* Previous frame time in clocks */
  PauseTime,  /* Pause full time in clocks */
  OldFPSTime, /* FPS measure time in clocks */
  FrameCount; /* Frame counter for FPS measure */

/* Timer initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID GLB_TimerInit( VOID )
{
  StartTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  PauseTime = 0;
  GLB_IsPause = FALSE;
} /* End of 'GLB_TimerInit' function */

/* Response timer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID GLB_TimerResponse( VOID )
{
  LONG t = clock();

  if (!GLB_IsPause)
  {
    GLB_Time = (DOUBLE)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
    GLB_DeltaTime = (DOUBLE)(t - OldTime) / CLOCKS_PER_SEC;
  }
  else
  {
    PauseTime += t - OldTime;
    GLB_DeltaTime = 0;
  }
  FrameCount++;
  if (t - OldFPSTime > CLOCKS_PER_SEC)
  {
    GLB_FPS = FrameCount / ((DOUBLE)(t - OldFPSTime) / CLOCKS_PER_SEC);
    FrameCount = 0;
    OldFPSTime = t;
  }
  OldTime = t;
} /* End of 'GLB_TimerResponse' function */

/* END OF 'timer.c' FILE */