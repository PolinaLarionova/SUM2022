/* FILE       : rndprim.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 09.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */

#include "rnd.h"

BOOL PL6_RndPrimCreate( pl6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(pl6PRIM));
  size = sizeof(pl6VERTEX) * NoofV + sizeof(INT) * NoofI;
  Pr->V = malloc(size);
  if (Pr->V == NULL)
    return FALSE;
  memset(Pr->V, 0, size);
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  return TRUE;
}

VOID PL6_RndPrimFree( pl6PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(pl6PRIM));
}

VOID PL6_RndPrimDraw( pl6PRIM *Pr, MATR World )
{
  INT i;
  POINT *pnts;
  MATR M = MatrMulMatr3(Pr->Trans, World, PL6_RndMatrVP);
  if((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, M);

    pnts[i].x = (INT)((p.X + 1) * PL6_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * PL6_RndFrameH / 2);
  }

  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(PL6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(PL6_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(PL6_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(PL6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}

BOOL PL6_RndPrimLoad( pl6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nind = 0;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(pl6PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;


  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0, i;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
          n++;
      nind += (n - 2) * 3;
    }

 
  if (!PL6_RndPrimCreate(Pr, nv, nind))
  {
    fclose(F);
    return FALSE;
  }


  rewind(F);
  nv = nind = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0, i, nc, n0, n1;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) &&!isspace((UCHAR)Buf[i]))
        {
          sscanf(Buf + i, "%d", &nc);
          if (n == 0)
            n0 = nc;
          else if (n == 1)
            n1 = nc;
          else
          {
            Pr->I[nind++] = n0 - 1;
            Pr->I[nind++] = n1 - 1;
            Pr->I[nind++] = nc - 1;
            n1 = nc;
          }
          n++;
        }
    }
  fclose(F);
  return TRUE;
} /* End of 'PL6_RndPrimLoad' function */





