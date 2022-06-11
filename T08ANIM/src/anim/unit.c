/* FILE       : unit.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */
#include "unit/units.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT *Uni, pl6ANIM *Ani )
{
} /* End of 'PL6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT *Uni, pl6ANIM *Ani )
{
} /* End of 'PL6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitResponse( pl6UNIT *Uni, pl6ANIM *Ani )
{
} /* End of 'PL6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT *Uni, pl6ANIM *Ani )
{
} /* End of 'PL6_UnitRender' function */

pl6UNIT * PL6_AnimUnitCreate( INT Size )
{
  pl6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(pl6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = PL6_UnitInit;
  Uni->Close = PL6_UnitClose;
  Uni->Response = PL6_UnitResponse;
  Uni->Render = PL6_UnitRender;

  return Uni;
} /* End of 'PL6_AnimUnitCreate' function */

/* END OF 'unit.c' FILE */