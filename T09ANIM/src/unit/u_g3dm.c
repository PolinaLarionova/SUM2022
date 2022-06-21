/* FILE       : u_g3dm.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 21.06.2022
 * PURPOSE    : 3D animation project.
 */

#include "pl6.h"

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  INT NumOfpl6Prims;
  pl6PRIMS G3dm[300];
} pl6UNIT_G3DM;

/* Unit_G3dm initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_G3DM *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT_G3DM *Uni, pl6ANIM *Ani )
{
  INT i;
  pl6MATERIAL mtl;

  Uni->NumOfpl6Prims = 2;
  PL6_RndPrimsLoad(&Uni->G3dm[0], "bin/models/Flower.g3dm");
  Uni->G3dm[0].Trans = MatrMulMatr(MatrScale(VecSet1(0.2)), MatrTranslate(VecSet(-2, 0, 2)));

  PL6_RndPrimsLoad(&Uni->G3dm[1], "bin/models/sova30.g3dm");
  Uni->G3dm[1].Trans = MatrTranslate(VecSet(0, 4, 0));
  mtl = PL6_RndMtlGetDef();
  strcpy(mtl.Name, "Sova Material");
  mtl.ShdNo = PL6_RndShdAdd("sova");
  //mtl.Tex[0] = PL6_RndTexAddFromFile("bin/textures/sky2.g24");
  for (i = 0; i < Uni->G3dm[1].NumOfPrims; i++)
    Uni->G3dm[1].Prims[i].MtlNo = PL6_RndMtlAdd(&mtl);
} /* End of 'PL6_UnitInit' function */

/* Unit_G3dm inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_G3DM *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitResponse( pl6UNIT_G3DM *Uni, pl6ANIM *Ani )
{
  //Uni->G3dm[1].Trans = MatrMulMatr(Uni->G3dm[1].Trans, MatrRotateY(180 * sin(Ani->DeltaTime / 10)));
} /* End of 'PL6_UnitResponse' function */

/* Unit_G3dm render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_G3DM *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT_G3DM *Uni, pl6ANIM *Ani )
{
  INT i;

  for (i = 0; i < Uni->NumOfpl6Prims; i++)
    PL6_RndPrimsDraw(&Uni->G3dm[i], Uni->G3dm[i].Trans);
} /* End of 'PL6_UnitRender' function */

/* Unit_G3dm deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_G3DM *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT_G3DM *Uni, pl6ANIM *Ani )
{
  INT i;

  for (i = 0; i < Uni->NumOfpl6Prims; i++)
    PL6_RndPrimsFree(&Uni->G3dm[i]);
} /* End of 'PL6_UnitClose' function */

/* Unit_G3dm create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateG3dm( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_G3DM))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Response = (VOID *)PL6_UnitResponse;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
} /* End of 'PL6_UnitCreateG3dm' function */

/* END OF 'u_g3dm.c' FILE */