/* FILE       : u_rain.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 22.06.2022
 * PURPOSE    : 3D animation project.
 */

#include "pl6.h"

#define PL6_NUM_OF_DROPS 40

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos[PL6_NUM_OF_DROPS];
  pl6PRIM Rain;
} pl6UNIT_RAIN;

/* Unit_Rain initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_RAIN *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT_RAIN *Uni, pl6ANIM *Ani )
{
  INT i;
  pl6VERTEX 
    V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 1, 0}, {1, 1, 1, 1}},
  };
  pl6MATERIAL mtl;

  for (i = 0; i < PL6_NUM_OF_DROPS; i++)
    Uni->Pos[i] = VecSet(Rnd1() * 10, Rnd0() * 10, Rnd1() * 10);

  PL6_RndPrimCreate(&Uni->Rain, PL6_RND_PRIM_POINTS, V, 1, NULL, 0);
  mtl = PL6_RndMtlGetDef();
  strcpy(mtl.Name, "Rain Material");
  mtl.ShdNo = PL6_RndShdAdd("rain");
  mtl.Tex[0] = PL6_RndTexAddFromFile("bin/textures/drop2.g32");
  Uni->Rain.MtlNo = PL6_RndMtlAdd(&mtl);
} /* End of 'PL6_UnitInit' function */

/* Unit_Rain inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_RAIN *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitResponse( pl6UNIT_RAIN *Uni, pl6ANIM *Ani )
{
  INT i;

  for (i = 0; i < PL6_NUM_OF_DROPS; i++)
    if((Uni->Pos[i].Y -= Ani->DeltaTime * 1) <= 0)
      Uni->Pos[i].Y = 10;
} /* End of 'PL6_UnitResponse' function */

/* Unit_Rain render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_RAIN *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT_RAIN *Uni, pl6ANIM *Ani )
{
  INT i;

  for (i = 0; i < PL6_NUM_OF_DROPS; i++)
    PL6_RndPrimDraw(&Uni->Rain, MatrTranslate(Uni->Pos[i]));
} /* End of 'PL6_UnitRender' function */

/* Unit_Rain deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_RAIN *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT_RAIN *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimFree(&Uni->Rain);
} /* End of 'PL6_UnitClose' function */

/* Unit_Rain create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateRain( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_RAIN))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Response = (VOID *)PL6_UnitResponse;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
} /* End of 'PL6_UnitCreateRain' function */

/* END OF 'u_rain.c' FILE */