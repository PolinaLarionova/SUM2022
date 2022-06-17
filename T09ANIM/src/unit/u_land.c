/* FILE       : u_land.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 17.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */

#include "pl6.h"

/* Animation unit reprentation type */
typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  pl6PRIM Land;
} pl6UNIT_LAND;


/* Load picture from '*.BMP' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       pl6PRIM *Land;
 * RETURNS:
 *       (BOOL) TRUE if succes, FALSE otherwise.
 */
static VOID PL6_LandLoad( pl6UNIT_LAND *Uni, pl6PRIM *Land )
{
  HBITMAP hBm;
  BITMAP bm;
  pl6VERTEX *V;
  INT w, h, x, y;
  pl6MATERIAL mtl = PL6_RndMtlGetDef();

  if ((hBm = LoadImage(NULL, "bin/heights/hf.bmp", IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if ((bm.bmBitsPixel == 8 || bm.bmBitsPixel == 24) &&
        (V = malloc(w * h * sizeof(pl6VERTEX))) != NULL)
    {
      BYTE *Bits = bm.bmBits;

      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x * (bm.bmBitsPixel / 8)];

          V[y * w + x].P = VecSet(x / (w - 1.0),
                                  hgt / 255.0,
                                  1 - y / (h - 1.0));
          V[y * w + x].T = Vec2Set(x / (w - 1.0), 1 - y / (h - 1.0));
        }
      PL6_RndPrimGridEvaNormals(w, h, V);
      PL6_RndPrimCreateGrid(Land, w, h, V);


      strcpy(mtl.Name, "Landscape Material");
      //mtl.ShdNo = PL6_RndShdAdd("landscape");
      mtl.Tex[0] = PL6_RndTexAddFromFile("bin/textures/hftex.bmp");
      Uni->Land.MtlNo = PL6_RndMtlAdd(&mtl);
      free(V);
    }
    DeleteObject(hBm);
  }

}

/* Unit_Land initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_LAND *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitInit( pl6UNIT_LAND *Uni, pl6ANIM *Ani )
{
  Uni->Pos = VecSet(-0.5, -0.05, -0.5);
  PL6_LandLoad(Uni, &Uni->Land);
} /* End of 'PL6_UnitInit' function */

/* Unit_Land render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_LAND *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitRender( pl6UNIT_LAND *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimDraw(&Uni->Land, MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(100, 10, 100))));
} /* End of 'PL6_UnitRender' function */

/* Unit_Land deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       pl6UNIT_LAND *Uni;
 *   - animation context:
 *       pl6ANIM *Ani;
 * RETURNS: None.
 */
static VOID PL6_UnitClose( pl6UNIT_LAND *Uni, pl6ANIM *Ani )
{
  PL6_RndPrimFree(&Uni->Land);
} /* End of 'PL6_UnitClose' function */

/* Unit_Land create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateLand( VOID )
{
  pl6UNIT *Uni;

  if ((Uni = PL6_AnimUnitCreate(sizeof(pl6UNIT_LAND))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PL6_UnitInit;
  Uni->Close = (VOID *)PL6_UnitClose;
  Uni->Render = (VOID *)PL6_UnitRender;
  return Uni;
} /* End of 'PL6_UnitCreateCow' function */

/* END OF 'u_land.c' FILE */