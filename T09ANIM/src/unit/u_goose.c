/* FILE       : u_goose.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 22.06.2022
 * PURPOSE    : 3D animation project.
 */

#include "pl6.h"

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos, V;
  DBL Rot;
  pl6PRIMS Goose;
} pl6UNIT_GOOSE;

/* Unit_Goose initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_GOOSE *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT_GOOSE *Uni, pl6ANIM *Ani )
{
  Uni->V = VecSet(0, 0, -1);
  Uni->Pos = VecSet(0, 0, 0);
  PL6_RndNumberOfShader = PL6_RndShdAdd("goose");
  PL6_RndPrinsLoadTransform = MatrMulMatr(MatrScale(VecSet1(0.03)), MatrRotateX(-90));
  PL6_RndPrimsLoad(&Uni->Goose, "bin/models/bird2.g3dm");
  Uni->Goose.Trans = MatrTranslate(Uni->Pos);
} /* End of 'PL6_UnitInit' function */

/* Unit_Goose inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_GOOSE *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitResponse( pl6UNIT_GOOSE *Uni, pl6ANIM *Ani )
{
  static DBL R = 0;

  Uni->Rot = 20 * -Ani->JX * Ani->DeltaTime;
  R += Uni->Rot;
  Uni->V = VectorTransform(Uni->V, MatrRotateY(Uni->Rot));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Uni->V, Ani->JY * Ani->DeltaTime));
  Uni->Goose.Trans = MatrMulMatr(MatrRotateY(R), MatrTranslate(Uni->Pos));
} /* End of 'PL6_UnitResponse' function */

/* Unit_Goose render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_GOOSE *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT_GOOSE *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimsDraw(&Uni->Goose, Uni->Goose.Trans);
  //PL6_RndCamSet(VecAddVec(Uni->Pos, VecSet(Uni->V.X, 1, Uni->V.Z * 3)), Uni->Pos, VecSet(0, 1, 0));
} /* End of 'PL6_UnitRender' function */

/* Unit_Goose deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_GOOSE *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT_GOOSE *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimsFree(&Uni->Goose);
} /* End of 'PL6_UnitClose' function */

/* Unit_Goose create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateGoose( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_GOOSE))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Response = (VOID *)PL6_UnitResponse;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
} /* End of 'PL6_UnitCreateGoose' function */

/* END OF 'u_goose.c' FILE */