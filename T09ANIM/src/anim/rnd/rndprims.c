/* FILE NAME: rndprims.c
 * PROGRAMMER: PL6
 * DATE: 17.06.2022
 * PURPOSE: 3D animation primitive collection handle module.
 */

#include <stdio.h>
#include <string.h>

#include "pl6.h"

/* Create array of primitives function.
* ARGUMENTS:
 *   - pointer to primitives structure:
 *       pl6PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL PL6_RndPrimsCreate( pl6PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(pl6PRIMS));
  if ((Prs->Prims = malloc(sizeof(pl6PRIM) * NumOfPrims)) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(pl6PRIM) * NumOfPrims);
  Prs->NumOfPrims = NumOfPrims;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'PL6_RndPrimsCreate' function */

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       pl6PRIMS *Prs;
 * RETURNS: None.
 */
VOID PL6_RndPrimsFree( pl6PRIMS *Prs )
{
  INT i;

  if (Prs->Prims != NULL)
  {
    for (i = 0; i < Prs->NumOfPrims; i++)
      PL6_RndPrimFree(&Prs->Prims[i]);
    free(Prs->Prims);
  }
  memset(Prs, 0, sizeof(pl6PRIMS));
} /* End of 'PL6_RndPrimsFree' function */

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       pl6PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID PL6_RndPrimsDraw( pl6PRIMS *Prs, MATR World )
{
  INT i;
  MATR m = MatrMulMatr(Prs->Trans, World);

  PL6_RndShadersAddonI[0] = Prs->NumOfPrims;

  /* Draw all nontransparent primitives */ 
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (PL6_RndMtlGet(Prs->Prims[i].MtlNo)->Trans == 1)
      PL6_RndShadersAddonI[1] = i, PL6_RndPrimDraw(&Prs->Prims[i], m);

  /* Draw all transparent primitives with front face culling */ 
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (PL6_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      PL6_RndShadersAddonI[1] = i, PL6_RndPrimDraw(&Prs->Prims[i], m);

  /* Draw all transparent primitives with back face culling */ 
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (PL6_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      PL6_RndShadersAddonI[1] = i, PL6_RndPrimDraw(&Prs->Prims[i], m);
  glDisable(GL_CULL_FACE);
} /* End of 'PL6_RndPrimsDraw' function */

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       pl6PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL PL6_RndPrimsLoad( pl6PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen;
  BYTE *mem, *ptr;
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  DWORD p, m, t;

  memset(Prs, 0, sizeof(pl6PRIMS));
  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Calculate file size */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  fseek(F, 0, SEEK_SET);

  /* Load whole file into memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(mem, 1, flen, F);

  /* Go through all data */
  ptr = mem;

  /* Read header */
  Sign = *(DWORD *)ptr, ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    fclose(F);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr, ptr += 4;
  NumOfMaterials = *(DWORD *)ptr, ptr += 4;
  NumOfTextures = *(DWORD *)ptr, ptr += 4;

  /* Create primitive collection */
  if (!PL6_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    fclose(F);
    return FALSE;
  }
  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;
    DWORD MtlNo;
    pl6VERTEX *V;
    INT *Ind;

    NumOfVertexes = *(DWORD *)ptr, ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr, ptr += 4;
    MtlNo = *(DWORD *)ptr, ptr += 4;
    V = (pl6VERTEX *)ptr, ptr += sizeof(pl6VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr, ptr += sizeof(INT) * NumOfFacetIndexes;
    PL6_RndPrimCreate(&Prs->Prims[p], PL6_RND_PRIM_TRIMESH, V, NumOfVertexes, Ind, NumOfFacetIndexes);
    Prs->Prims[p].MtlNo = PL6_RndMaterialsSize + MtlNo;
  }
  /* Load materials */
  for (m = 0; m < NumOfMaterials; m++)
  {
    struct
    {
      CHAR Name[300]; /* Material name */
      /* Illumination coefficients */
      VEC Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
      FLT Ph;             /* Phong power coefficient – shininess */
      FLT Trans;          /* Transparency factor */
      DWORD Tex[8];       /* Texture references 
                           * (8 time: texture number in G3DM file, -1 if no texture) */
      /* Shader information */
      CHAR ShaderString[300]; /* Additional shader information */
      DWORD Shader;       /* Shader number (uses after load into memory) */
    } *fmat;
    pl6MATERIAL mtl = PL6_RndMtlGetDef();

    fmat = (VOID *)ptr, ptr += sizeof(*fmat);
    strncpy(mtl.Name, fmat->Name, PL6_STR_MAX);
    mtl.Ka = VecMinVec(fmat->Ka, VecSet1(0.1));
    mtl.Kd = fmat->Kd;
    mtl.Ks = fmat->Ks;
    mtl.Ph = fmat->Ph;
    mtl.Trans = fmat->Trans;
    for (t = 0; t < 8; t++)
      mtl.Tex[t] = fmat->Tex[t] == -1 ? -1 : PL6_RndTexturesSize + fmat->Tex[t];
    PL6_RndMtlAdd(&mtl);
  }

  /* Load textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    CHAR *Name;
    DWORD W, H, C;

    Name = (CHAR *)ptr, ptr += 300;
    W = *(DWORD *)ptr, ptr += 4;
    H = *(DWORD *)ptr, ptr += 4;
    C = *(DWORD *)ptr, ptr += 4;
    PL6_RndTexAddImg(Name, W, H, C, ptr);
    ptr += W * H * C;
  }

  free(mem);
  fclose(F);
  return TRUE;
} /* End of 'PL6_RndPrimsDraw' function */

/* END OF 'rndprims.c' FILE */
