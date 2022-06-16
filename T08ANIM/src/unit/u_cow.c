/* FILE       : u_cow.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */

#include "pl6.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  pl6PRIM Cow;
} pl6UNIT_COW;

static VOID PL6_UnitInit( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
  Uni->Pos = VecSet(0, 0, 0);
  PL6_RndPrimLoad(&Uni->Cow, "bmp/models/cow.obj");
}

static VOID PL6_UnitResponse( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
  //Uni->Pos.Z += Ani->DeltaTime * 2.4;
  Uni->Cow.Trans = MatrScale(VecSet1(0.1)); 
}

static VOID PL6_UnitRender( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimDraw(&Uni->Cow, /*MatrMulMatr(MatrRotateY(30 * Ani->Time), */ MatrTranslate(Uni->Pos));
}

static VOID PL6_UnitClose( pl6UNIT_COW *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimFree(&Uni->Cow);
}

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
}