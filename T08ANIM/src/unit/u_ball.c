/* FILE       : u_ball.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */

#include "pl6.h"

#define GRID_H 30
#define GRID_W 60

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  pl6PRIM Ball;
} pl6UNIT_BALL;

static VOID PL6_UnitInit( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
  Uni->Pos = VecSet(3, 0, 0);
  PL6_RndPrimCreate(&Uni->Ball, GRID_W * GRID_H, GRID_W * GRID_H * 3);
}

static VOID PL6_UnitResponse( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
  //Uni->Pos += Ani->DeltaTime * 2.4;
  Uni->Ball.Trans = MatrScale(VecSet1(0.1)); 
}

static VOID PL6_UnitRender( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimDraw(&Uni->Ball, /*MatrMulMatr(MatrRotateY(30 * Ani->Time), */ MatrTranslate(Uni->Pos));
}

static VOID PL6_UnitClose( pl6UNIT_BALL *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimFree(&Uni->Ball);
}

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
}