/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: PL6
 * DATE: 06.06.2020
 * PURPOSE: globe
 */
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "globe.h"

static VEC Geom[GRID_H][GRID_W];

VOID GlobeSet( DBL R )
{
  INT i, j;

  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      DBL theta = i * PI / (GRID_H - 1), phi = j * 2 * PI / (GRID_W - 1);

      Geom[i][j].X = R * sin(theta) * sin(phi);
      Geom[i][j].Y = 0.5 * R * cos(theta);
      Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
}

VEC RotateZ( VEC P, DBL angle )
{
  VEC NewP;
  DBL a = angle * PI / 180, si = sin(a), co = cos(a);

  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
}

VEC RotateX( VEC P, DBL angle )
{
  VEC NewP;
  DBL a = angle * PI / 180, si = sin(a), co = cos(a);

  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Y * si + P.Z * co;
  NewP.X = P.X;
  return NewP;
}

VEC RotateY( VEC P, DBL angle )
{
  VEC NewP;
  DBL a = angle * PI / 180, si = sin(a), co = cos(a);

  NewP.Z = P.Z * co - P.X * si;
  NewP.X = P.Z * si + P.X * co;
  NewP.Y = P.Y;
  return NewP;
}

VOID GlobeDraw( HDC hDC, INT w, INT h )
{
  static POINT pnts[GRID_H][GRID_W];
  INT i, j;
  DBL r, Wp, Hp, Size, ProjDist, t = clock() / (DBL)CLOCKS_PER_SEC;
  HPEN hPen;
  MATR m;

  r = (w < h ? w : h) / 2;

  ProjDist = Size = 1;
  Wp = Hp = Size;
  if (w > h)
    Wp *= (DBL)w / h;
  else
    Hp *= (DBL)h / w;

  m = MatrMulMatr(MatrView(VecSet(2, 9, 6), VecSet(0, 0, 0), VecSet(0, -1, 0)), MatrFrustum(-Wp / 2, Wp / 2, -Hp / 2, Hp /2, 3, 300));
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC p = VecMulMatr(Geom[i][j], m);

      //p = RotateZ(p, t * 17);
      //p = RotateY(p, t * 12);
      //p = RotateX(p, t * 10);
      pnts[i][j].x = (INT)((p.X + 1) * w / 2);
      pnts[i][j].y = (INT)((-p.Y + 1) * h / 2);
    }
  hPen = CreatePen(PS_SOLID, 1, RGB(4, 51, 119));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(30, 144, 255));
  SelectObject(hDC, hPen);
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      POINT pnts1[4];
      pnts1[0] = pnts[i][j];
      pnts1[1] = pnts[i][j + 1];
      pnts1[2] = pnts[i + 1][j + 1];
      pnts1[3] = pnts[i + 1][j];

      if ((pnts1[0].x - pnts1[1].x) * (pnts1[0].y + pnts1[1].y) +
          (pnts1[1].x - pnts1[2].x) * (pnts1[1].y + pnts1[2].y) +
          (pnts1[2].x - pnts1[3].x) * (pnts1[2].y + pnts1[3].y) +
          (pnts1[3].x - pnts1[0].x) * (pnts1[3].y + pnts1[0].y) > 0)
          Polygon(hDC, pnts1, 4);
    }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      POINT pnts1[4];
      pnts1[0] = pnts[i][j];
      pnts1[1] = pnts[i][j + 1];
      pnts1[2] = pnts[i + 1][j + 1];
      pnts1[3] = pnts[i + 1][j];

      if ((pnts1[0].x - pnts1[1].x) * (pnts1[0].y + pnts1[1].y) +
          (pnts1[1].x - pnts1[2].x) * (pnts1[1].y + pnts1[2].y) +
          (pnts1[2].x - pnts1[3].x) * (pnts1[2].y + pnts1[3].y) +
          (pnts1[3].x - pnts1[0].x) * (pnts1[3].y + pnts1[0].y) <= 0)
          Polygon(hDC, pnts1, 4);
    }
  DeleteObject(hPen);
}