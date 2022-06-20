/* FILE       : u_ctrl.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 18.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */
#include "pl6.h"

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  DBL Speed;
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
  PL6_RndCamSet(VecSet(2, 2, 2), VecSet(1, 1, 1), VecSet(0, 1, 0));
  Uni->Speed = 1;
  PL6_RndFntInit();
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

  if (Ani->Keys[VK_CONTROL])
  {
    FLT Dist, cosT, sinT, plen, cosP, sinP, Azimuth, Elevator, Hp, Wp, sx, sy;
    VEC dv;

    Dist = VecLen(VecSubVec(PL6_RndCamAt, PL6_RndCamLoc));

    if (Dist < 0.0001)
      Dist = 0.0001;

    cosT = (PL6_RndCamLoc.Y - PL6_RndCamAt.Y) / Dist;
    sinT = sqrt(1 - cosT * cosT);

    plen = Dist * sinT;
    cosP = (PL6_RndCamLoc.Z - PL6_RndCamAt.Z) / plen;
    sinP = (PL6_RndCamLoc.X - PL6_RndCamAt.X) / plen;

    Azimuth = R2D(atan2(sinP, cosP));
    Elevator = R2D(atan2(sinT, cosT));

    Azimuth += Ani->GlobalDeltaTime *
      (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdx +
       47 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));

    Elevator += Ani->GlobalDeltaTime *
      (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 
       47 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

    Dist += Ani->GlobalDeltaTime *
      (1 * -Ani->Mdz +
      (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));

    if (Elevator < 0.1)
      Elevator = 0.1;
    else if (Elevator > 179)
      Elevator = 179;

    Wp = PL6_RndProjSize;
    Hp = PL6_RndProjSize;

    if (Ani->W > Ani->H)
      Wp *= (FLT)Ani->W / Ani->H;
    else
      Hp *= (FLT)Ani->H / Ani->W;

    sx = Ani->Keys[VK_RBUTTON] * -Ani->Mdx * Wp / Ani->W * Dist / PL6_RndProjDist;
    sy = Ani->Keys[VK_RBUTTON] * Ani->Mdy * Hp / Ani->H * Dist / PL6_RndProjDist;

    dv = VecAddVec(VecMulNum(PL6_RndCamRight, sx),
                   VecMulNum(PL6_RndCamUp, sy));

    PL6_RndCamAt = VecAddVec(PL6_RndCamAt, dv);
    PL6_RndCamLoc = VecAddVec(PL6_RndCamLoc, dv);

    PL6_RndCamSet(PointTransform(VecSet(0, Dist, 0),
                                 MatrMulMatr3(MatrRotateX(Elevator),
                                            MatrRotateY(Azimuth),
                                            MatrTranslate(PL6_RndCamAt))),
                                 PL6_RndCamAt,
                                 VecSet(0, 1, 0));
  }
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

  sprintf(Buf, "%lf (%f %f %f %f ) %i", Ani->FPS, Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JPov);
  //SetWindowText(Ani->hWnd, Buf);
  PL6_RndFntDraw(Buf, VecSet(2, 2, 2), 1);
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
  PL6_RndFntClose("bin/fonts/arial.g3df");
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