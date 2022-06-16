/* FILE       : u_ctrl.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */
#include "pl6.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC CamLoc, CamDir;
  DBL Speed;
} pl6UNIT_CONTROL;

static VOID PL6_UnitInit( pl6UNIT_CONTROL *Uni, pl6ANIM *Ani )
{
  PL6_RndCamSet(VecSet(0, 0, 6), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Uni->CamLoc = VecSet(0, 0, 6);
  Uni->CamDir = VecNormalize(VecSet(0, 0, -1));
  Uni->Speed = 1;
}

static VOID PL6_UnitResponse( pl6UNIT_CONTROL *Uni, pl6ANIM *Ani )
{
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed *
        (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));

  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * Ani->Mdz));

  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
        Ani->GlobalDeltaTime * Uni->Speed * Ani->JX));

  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateX(Ani->Keys[VK_LBUTTON] *
       Ani->GlobalDeltaTime * Uni->Speed * Ani->Mdy));

    //Uni->CamLoc = 
      VecAddVec(Uni->CamLoc,
        VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * Ani->JX));

    Uni->CamLoc = 
      VecAddVec(Uni->CamLoc,
        VecMulNum(VecNormalize(VecSet(0, 1, 0)), Ani->GlobalDeltaTime * Uni->Speed * Ani->JR));


  PL6_RndCamSet(Uni->CamLoc, VecSet(0, 0, 0), VecSet(0, 1, 0));
}

static VOID PL6_UnitRender( pl6UNIT_CONTROL *Uni, pl6ANIM *Ani )
{
  CHAR Buf[102];

  SetBkMode(PL6_Anim.hDC, TRANSPARENT);
  TextOut(PL6_Anim.hDC, 100, 100, Buf, sprintf(Buf, "%lf (%f %f %f %f ) %i",
    Ani->FPS, Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JPov));
}

pl6UNIT * PL6_UnitCreateControl( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_CONTROL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  //Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Response = (VOID *)PL6_UnitResponse;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
}
/* END OF 'u_ctrl.c' FILE */