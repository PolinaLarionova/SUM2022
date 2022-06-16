/* FILE NAME  : rndres.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 16.06.2022
 * PURPOSE    : 3D animation project.
 *              Render subsystem implementation module.
 *              Render system resourses handle module.
 */

#include "rndres.h"

/* Resourses system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndResInit( VOID )
{
  PL6_RndShdInit();
  PL6_RndTexInit();
  PL6_RndMtlInit();
} /* End of 'PL6_RndResInit' function */

/* Resourses system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndResClose( VOID )
{
  PL6_RndMtlClose();
  PL6_RndTexClose();
  PL6_RndShdClose();
} /* End of 'PL6_RndResClose' function */

/* END OF 'rndres.c' FILE */


