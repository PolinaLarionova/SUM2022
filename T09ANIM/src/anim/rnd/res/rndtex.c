 /* FILE NAME  : rndtex.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 16.06.2022
 * PURPOSE    : 3D animation project.
 *              Render subsystem implementation module.
 *              Rendering resourse handle implementation module.
 *              Textures handle functions.
 */

#include <stdio.h>
#include <string.h>

#include "../rnd.h"

/***
 * Texture stock handle
 ***/

/* Textures stock */
pl6TEXTURE PL6_RndTextures[PL6_MAX_TEXTURES]; /* Array of textures */
INT PL6_RndTexturesSize;                      /* Textures array store size */

/* Texture store system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTexInit( VOID )
{
  PL6_RndTexturesSize = 0;
} /* End of 'PL6_RndTexInit' function */

/* Texture store system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < PL6_RndTexturesSize; i++)
    glDeleteTextures(1, &PL6_RndTextures[i].TexId);
  PL6_RndTexturesSize = 0;
} /* End of 'PL6_RndTexClose' function */

/* Add new texture to texture store system function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size:
 *       INT W, H;
 *   - number of terxture pixels color components:
 *       INT C;
 *   - pointer to pixels color data:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) added texture table number.
 */
INT PL6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT mips;

  /* Check for free space */
  if (PL6_RndTexturesSize >= PL6_MAX_TEXTURES)
    return -1;

  strncpy(PL6_RndTextures[PL6_RndTexturesSize].Name, Name, PL6_STR_MAX - 1);
  PL6_RndTextures[PL6_RndTexturesSize].W = W;
  PL6_RndTextures[PL6_RndTexturesSize].H = H;
  glGenTextures(1, &PL6_RndTextures[PL6_RndTexturesSize].TexId);

  glBindTexture(GL_TEXTURE_2D, PL6_RndTextures[PL6_RndTexturesSize].TexId);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  mips = W > H ? H : W;
  mips = log(mips) / log(2);
  if (mips < 1)
    mips = 1;

  glTexStorage2D(GL_TEXTURE_2D, mips, C == 3 ? GL_RGB8 : C == 4 ? GL_RGBA8 : GL_R8, W, H);

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
    C == 3 ? GL_BGR : C == 4 ? GL_BGRA : GL_LUMINANCE, GL_UNSIGNED_BYTE, Bits);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return PL6_RndTexturesSize++;
} /* End of 'PL6_RndTexAddImg' function */

/* Add texture image from file function.
 * ARGUMENTS:
 *   - texture image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) texture table index or -1 if error is occured.
 */
INT PL6_RndTexAddFromFile( CHAR *FileName )
{
  INT w, h, x, y, n = -1, flen, bpp;
  HBITMAP hBm;
  BITMAP bm;
  FILE *F;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if (bm.bmBitsPixel == 24)
    {
      BYTE *Bits = bm.bmBits;
      BYTE *mem;

      if ((mem = malloc(w * h * 3)) != NULL)
      {
        for (y = 0; y < h; y++)
          for (x = 0; x < w; x++)
          {
            BYTE *ptr = ((BYTE *)bm.bmBits + y * bm.bmWidthBytes + x * 3);

            mem[(y * w + x) * 3 + 0] = *ptr++;
            mem[(y * w + x) * 3 + 1] = *ptr++;
            mem[(y * w + x) * 3 + 2] = *ptr++;
          }
        n = PL6_RndTexAddImg(FileName, w, h, 3, mem);
        free(mem);
      }
    }
    else if (bm.bmBitsPixel == 8)
    {
      BYTE *Bits = bm.bmBits;
      BYTE *mem;

      if ((mem = malloc(w * h * 1)) != NULL)
      {
        for (y = 0; y < h; y++)
          for (x = 0; x < w; x++)
          {
            BYTE *ptr = ((BYTE *)bm.bmBits + y * bm.bmWidthBytes + x);

            mem[y * w + x] = *ptr++;
          }
        n = PL6_RndTexAddImg(FileName, w, h, 1, mem);
        free(mem);
      }
    }
    DeleteObject(hBm);
  }
  else if ((F = fopen(FileName, "rb")) != NULL)
  {
    DWORD *mem;

    w = 0;
    h = 0;
    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    fseek(F, 0, SEEK_END);
    flen = ftell(F);
    fseek(F, 4, SEEK_SET);
    if (flen == 4 + w * h * 4)
      bpp = 4;
    else if (flen == 4 + w * h * 3)
      bpp = 3;
    else if (flen == 4 + w * h)
      bpp = 1;

    if ((mem = malloc(w * h * 4)) != NULL)
    {
      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          DWORD Color = 0xFF000000;

          fread(&Color, bpp, 1, F);
          if (bpp == 1)
            Color |= ((Color & 0xFF) << 8) | ((Color & 0xFF) << 16);
          mem[(h - 1 - y) * w + x] = Color;
        }
      n = PL6_RndTexAddImg(FileName, w, h, 4, mem);
      free(mem);
    }
    fclose(F);
  }
  return n;
} /* End of 'PL6_RndTexAddFromFile' function */

/* END OF 'rndtex.c' FILE */
