/* FILE NAME: rndfnt.c
 * PROGRAMMER: PL6
 * DATE: 17.06.2022
 * PURPOSE: 3D animation primitive collection handle module.
 */
#include "pl6.h"

/* Font description structure */
typedef struct tagpl6FONT
{
  DWORD LineH, BaseH; /* Font line height and base line height in pixels */
  FLT AdvanceX[256];  /* Every letter shift right value (0 if no letter present) */
} pl6FONT;

/* Current font description */

/* Font data */
static pl6FONT PL6_RndFntFont;

/* Font characters primitives */
static pl6PRIM PL6_RndFntChars[256];

/* Font material */
static INT PL6_RndFntMtlNo;


/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL PL6_RndFntLoad( CHAR *FileName )
{
  FILE *F;
  DWORD Sign, W, H;
  pl6VERTEX Chars[256][4];
  DWORD *tex;
  pl6MATERIAL mtl;
  INT i;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }
  fread(&PL6_RndFntFont, sizeof(pl6FONT), 1, F);
  fread(Chars, sizeof(pl6VERTEX) * 4, 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
  if ((tex = malloc(W * H * 4)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(tex, 4, W * H, F);
  fclose(F);

  mtl = PL6_RndMtlGetDef();
  strncpy(mtl.Name, FileName, PL6_STR_MAX - 1);
  mtl.Tex[0] = PL6_RndTexAddImg(FileName, W, H, 4, tex);
  free(tex);
  mtl.ShdNo = PL6_RndShdAdd("font");
  PL6_RndFntMtlNo = PL6_RndMtlAdd(&mtl);

  for (i = 0; i < 256; i++)
  {
    PL6_RndPrimFree(&PL6_RndFntChars[i]);
    PL6_RndPrimCreate(&PL6_RndFntChars[i], PL6_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    PL6_RndFntChars[i].MtlNo = PL6_RndFntMtlNo;
  }

  return TRUE;
} /* End of 'PL6_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndFntInit( VOID )
{
  PL6_RndFntLoad("bin/fonts/arial.g3df");
} /* End of 'PL6_RndFntInit' function */

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndFntClose( CHAR *FileName )
{
  INT i;

  for (i = 0; i < 256; i++)
    PL6_RndPrimFree(&PL6_RndFntChars[i]);
  memset(PL6_RndFntChars, 0, sizeof(PL6_RndFntChars));
} /* End of 'PL6_RndFntClose' function */

/* Draw string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   = font size:
 *      FLT Size;
 * RETURNS: None.
 */
VOID PL6_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  VEC p = Pos;

  while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = p.X, Pos.Y -= Size;
    else
      if (PL6_RndFntFont.AdvanceX[(UCHAR)*Str] != 0)
      {
        PL6_RndPrimDraw(&PL6_RndFntChars[(UCHAR)*Str],
          MatrMulMatr(MatrScale(VecSet(Size, Size, 1)), MatrTranslate(Pos)));
        Pos.X += PL6_RndFntFont.AdvanceX[(UCHAR)*Str] * Size;
      }
    Str++;
  }
} /* End of 'PL6_RndFntDraw' function */

/* END OF 'rndfnt.h' FILE */