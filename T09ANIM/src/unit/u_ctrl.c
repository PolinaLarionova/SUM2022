/* FILE       : u_ctrl.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */
#include "pl6.h"

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  VEC CamLoc, CamDir;
  DBL Speed;
  pl6PRIM Pr;
} pl6UNIT_CONTROL;

/* Unit_Control initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_CONTROL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT_CONTROL *Uni, pl6ANIM *Ani )
{
  pl6VERTEX v[] =
  {
    {{0, 0, 0}, {0}, {0}, {0, 0.7, 0, 1}},
    {{1, 0, 0}, {0}, {0}, {0, 0.4, 0, 1}},
    {{1, 0, 1}, {0}, {0}, {0, 0.7, 0, 1}},
    {{0, 0, 1}, {0}, {0}, {0, 0.4, 0, 1}},
  };
  INT ind[] = {0, 1, 2, 0, 2, 3};


  PL6_RndPrimCreate(&Uni->Pr, PL6_RND_PRIM_TRIMESH, v, 4, ind, 6);
  Uni->CamLoc = VecSet(0, 0, 6);
  Uni->CamDir = VecNormalize(VecSet(0, 0, -1));
  Uni->Speed = 1;
}/* End of 'PL6_UnitInit' function */

/* Unit_Control inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_CONTROL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitResponse( pl6UNIT_CONTROL *Uni, pl6ANIM *Ani )
{
  if (Ani->KeysClick['W'] && Ani->Keys[VK_SHIFT])
  {
    INT mode[2];

    glGetIntegerv(GL_POLYGON_MODE, mode);
    glPolygonMode(GL_FRONT_AND_BACK, mode[0] == GL_FILL ? GL_LINE : GL_FILL);
  }

  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  if (Ani->KeysClick['V'])
    PL6_AnimFlipFullScreen();
 /*
  if (Ani->KeysClick[VK_CONTROL])
  {
    FLT Dist, cosT, sinT, plen, cosP, sinP, Azimuth, Elevator, 

    Dist = VecLen(VecSubVec(VG4_RndCamAt, VG4_RndCamLoc));

    cosT = (VG4_RndCamLoc.Y - VG4_RndCamAt.Y) / Dist;
    sinT = sqrt(1 – cosT * cosT);

    plen = Dist * sinT;
    cosP = (VG4_RndCamLoc.Z - VG4_RndCamAt.Z) / plen;
    sinP = (VG4_RndCamLoc.X - VG4_RndCamAt.X) / plen;

    Azimuth = R2D(atan2(sinP, cosP));
    Elevator = R2D(atan2(sinT, cosT));

    Azimuth += Ani->GlobalDeltaTime *
      (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdx +
       47 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));

    Elevator += Ani->GlobalDeltaTime *
      (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 
       47 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

    Dist += Ani->GlobalDeltaTime *
      (1 * Ani->Mdz +
      (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));

    if (Elevator < 0.01)
      Elevator = 0.01
    else if (Elevator > 179.99)
      Elevator = 179.99;

    PointTransform(VecSet(0, Dist, 0),
               MatrMulMatr(MatrRotateX(Elevator),
                           MatrRotateY(Azimuth)));
  }*/

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
        Ani->GlobalDeltaTime * 45 * Ani->Mdx));

  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateX(Ani->Keys[VK_LBUTTON] *
       Ani->GlobalDeltaTime * 45 * Ani->Mdy));

    Uni->CamLoc = 
      VecAddVec(Uni->CamLoc,
        VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * Ani->JX));

    Uni->CamLoc = 
      VecAddVec(Uni->CamLoc,
        VecMulNum(VecNormalize(VecSet(0, 1, 0)), Ani->GlobalDeltaTime * Uni->Speed * Ani->JR));

   PL6_RndCamSet(Uni->CamLoc, VecSet(0, 0, 0), VecSet(0, 1, 0)); 
}/* End of 'PL6_UnitResponse' function */

/* Unit_Control render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_CONTROL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT_CONTROL *Uni, pl6ANIM *Ani )
{
  static CHAR Buf[102];
  //INT i, j;

  sprintf(Buf, "%lf (%f %f %f %f ) %i", Ani->FPS, Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JPov);
  SetWindowText(Ani->hWnd, Buf);

  /*for(i = -15; i < 15; i++)
    for(j = -15; j < 15; j++)
      PL6_RndPrimDraw(&Uni->Pr, MatrTranslate(VecSet(i, 0, j))); */

}/* End of 'PL6_UnitRender' function */

/* Unit_Control deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_CONTROL *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT_CONTROL *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimFree(&Uni->Pr);
}/* End of 'PL6_UnitClose' function */

/* Unit_Control create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateControl( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_CONTROL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Response = (VOID *)PL6_UnitResponse;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
} /* End of 'PL6_UnitCreateControl' function */

/* END OF 'u_ctrl.c' FILE */