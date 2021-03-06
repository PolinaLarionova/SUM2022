/* FILE       : u_cow.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 22.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */

#include "pl6.h"

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  pl6PRIM Cow;
} pl6UNIT_COW;

/* Unit_Cow initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_COW *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
  pl6MATERIAL mtl;

  PL6_RndPrimLoad(&Uni->Cow, "bin/models/cow.obj");

  mtl = PL6_RndMtlGetDef();
  strcpy(mtl.Name, "Cow Material");
  //mtl.ShdNo = PL6_RndShdAdd("cow");
  mtl.Tex[0] = PL6_RndTexAddFromFile("bin/textures/cow3.g24");
  Uni->Cow.MtlNo = PL6_RndMtlAdd(&mtl);
} /* End of 'PL6_UnitInit' function */

/* Unit_Cow inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_COW *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitResponse( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
} /* End of 'PL6_UnitResponse' function */

/* Unit_Cow render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_COW *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimDraw(&Uni->Cow, MatrIdentity());
}/* End of 'PL6_UnitRender' function */

/* Unit_Cow deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_COW *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimFree(&Uni->Cow);
} /* End of 'PL6_UnitClose' function */

/* Unit_Cow create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateCow( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_COW))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Response = (VOID *)PL6_UnitResponse;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
} /* End of 'PL6_UnitCreateCow' function */

/* END OF 'u_cow.c' FILE */