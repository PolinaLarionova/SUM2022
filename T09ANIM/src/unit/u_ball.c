/* FILE       : u_ball.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 18.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */

#include "pl6.h"

#define GRID_H 10
#define GRID_W 20

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  pl6PRIM Ball;
} pl6UNIT_BALL;

/* Unit_Ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_BALL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
  pl6MATERIAL mtl;

  Uni->Pos = VecSet(0, 0, 0);
  PL6_RndPrimCreateSphere(&Uni->Ball, Uni->Pos, 50, GRID_W, GRID_H);

  mtl = PL6_RndMtlGetDef();
  strcpy(mtl.Name, "Sphere Material");
  mtl.ShdNo = PL6_RndShdAdd("sky");
  mtl.Tex[0] = PL6_RndTexAddFromFile("bin/textures/sky2.g24");
  Uni->Ball.MtlNo = PL6_RndMtlAdd(&mtl);
} /* End of 'PL6_UnitInit' function */

/* Unit_Ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_BALL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitResponse( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
} /* End of 'PL6_UnitResponse' function */

/* Unit_Ball render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_BALL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
  if (PL6_RndShadowPassFlag)
    return;

  PL6_RndPrimDraw(&Uni->Ball, MatrIdentity());
} /* End of 'PL6_UnitRender' function */

/* Unit_Ball deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_BALL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimFree(&Uni->Ball);
} /* End of 'PL6_UnitClose' function */

/* Unit_Ball create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateBall( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_BALL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Response = (VOID *)PL6_UnitResponse;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
} /* End of 'PL6_UnitCreateBall' function */

/* END OF 'u_ball.c' FILE */