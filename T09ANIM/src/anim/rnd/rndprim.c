/* FILE       : rndprim.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 09.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */

#include "pl6.h"

/* Primitive creation function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 *   - primitive type:
 *       pl6PRIM_TYPE Type;
 *   - array of pl6VERTEX:
 *       pl6VERTEX *V;
 *   - number of vertexes:
 *       INT NoofV
 *   - array of indexes:
 *       INT *I
 *   - number of indexes:
 *       INT NoofI
 * RETURNS: 
 *     (BOOL) TRUE if success, FALSE otherwise
 */
BOOL PL6_RndPrimCreate( pl6PRIM *Pr, pl6PRIM_TYPE Type, pl6VERTEX *V, INT NoofV, INT *I, INT NoofI )
{
  INT i;

  memset(Pr, 0, sizeof(pl6PRIM));

  if (NoofV > 0 && V != NULL)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);
    /* make the vertex array active */
    glBindVertexArray(Pr->VA);
    /* make buffer active */
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    /* merge data (NumOfV - number of vertices, V - array of vertices) */
    glBufferData(GL_ARRAY_BUFFER, sizeof(pl6VERTEX) * NoofV, V, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(pl6VERTEX),
                        (VOID *)0); /* position P */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(pl6VERTEX),
                        (VOID *)sizeof(VEC)); /* texture coordinates T */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(pl6VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal N */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(pl6VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color C */

    /* enable required attributes (layout) */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    /* turned off the vertex array */
    glBindVertexArray(0);

    Pr->MinBB = Pr->MaxBB = V[0].P;
    for (i = 1; i < Pr->NumOfV; i++)
    {
      Pr->MinBB = VecMinVec(Pr->MinBB, V[i].P);
      Pr->MaxBB = VecMaxVec(Pr->MaxBB, V[i].P);
    }
  }

  Pr->NumOfI = NoofI;
  Pr->NumOfV = NoofV;

  if (NoofI > 0 && I != NULL)
  {
    glGenBuffers(1, &Pr->IBuf);
    /* make buffer active */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    /* merge data (NumOfI - number of indices, I - array of indices) */ 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, I, GL_STATIC_DRAW);
    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;

  Pr->Type = Type;
  Pr->Trans = MatrIdentity();
  return TRUE;
} /* End of 'PL6_RndPrimCreate' function */


/* Primitive free function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 * RETURNS: None.
 */
VOID PL6_RndPrimFree( pl6PRIM *Pr )
{
  /* make the vertex array active */
  glBindVertexArray(Pr->VA);
  /* "unhook" buffer */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  /* make the array of vertices inactive */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);

} /* End of 'PL6_RndPrimFree' function */

/* Primitive drawing function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 *   - addtional transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID PL6_RndPrimDraw( pl6PRIM *Pr, MATR World )
{
  INT loc;
  INT RndProgId;
  INT gl_prim_type = Pr->Type == PL6_RND_PRIM_LINES ? GL_LINES :
                     Pr->Type == PL6_RND_PRIM_TRIMESH ? GL_TRIANGLES :
                     Pr->Type == PL6_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP : GL_POINTS;
  MATR
    w = MatrMulMatr(Pr->Trans, World),
    winv = MatrTranspose(MatrInverse(w)),
    wvp = MatrMulMatr(w, PL6_RndMatrVP);
  glLoadMatrixf(wvp.A[0]);

  RndProgId = PL6_RndMtlApply(Pr->MtlNo);

  glUseProgram(RndProgId);

    /* Pass render uniforms */
  if ((loc = glGetUniformLocation(RndProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(RndProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, w.A[0]);
  if ((loc = glGetUniformLocation(RndProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);
  if ((loc = glGetUniformLocation(RndProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &PL6_RndCamLoc.X);
  if ((loc = glGetUniformLocation(RndProgId, "Time")) != -1)
    glUniform1f(loc, PL6_Anim.Time);

  /* make the vertex array active */
  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
  else
  {
    /* make the index array active */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    /* rendering */
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  /* turned off the vertex array */
  glBindVertexArray(0);
  glUseProgram(0);
} /* End of 'PL6_RndPrimDraw' function */

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       pl6PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *       (BOOL) TRUE if succes, FALSE otherwise.
 */
BOOL PL6_RndPrimLoad( pl6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT noofv = 0, noofi = 0, size;
  static CHAR Buf[1000];
  pl6VERTEX *V;
  INT *Ind;

  memset(Pr, 0, sizeof(pl6PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      noofv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0, i;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
          n++;
      noofi += (n - 2) * 3;
    }

  size = sizeof(pl6VERTEX) * noofv + sizeof(INT) * noofi;

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + noofv);

  rewind(F);
  noofv = noofi = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[noofv].P = VecSet(x, y, z);
      V[noofv++].T = Vec2Set(x, z);
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
            Ind[noofi++] = n0 - 1;
            Ind[noofi++] = n1 - 1;
            Ind[noofi++] = nc - 1;
            n1 = nc;
          }
          n++;
        }
    }

  PL6_RndPrimTriMeshEvaNormals(V, noofv, Ind, noofi);
  PL6_RndPrimCreate(Pr, PL6_RND_PRIM_TRIMESH, V, noofv, Ind, noofi);
  free(V);
  fclose(F);
  return TRUE;
} /* End of 'PL6_RndPrimLoad' function */


/* Create grid function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 *   - grid split values:
 *       INT SplitW, INT SplitH;
 *   - agrid vertex:
 *       pl6VERTEX *V;
 * RETURNS: 
 *     (BOOL) TRUE if success, FALSE otherwise
 */
BOOL PL6_RndPrimCreateGrid( pl6PRIM *Pr, INT SplitW, INT SplitH, pl6VERTEX *V )
{
  INT i, j, k, num_of_indices;
  INT *Ind;

  memset(Pr, 0, sizeof(pl6PRIM));
  num_of_indices = (SplitH - 1) * (SplitW * 2 + 1) - 1;
  if((Ind = malloc(sizeof(INT) * num_of_indices)) == NULL)
    return FALSE;

  for(k = 0, i = 0; i < SplitH - 1; i++)
  {
    for(j = 0; j < SplitW; j++)
    {
      Ind[k++] = (i + 1) * SplitW + j;
      Ind[k++] = (i + 0) * SplitW + j;
    }
    if (i != SplitH - 2)
      Ind[k++] = -1;
  }
  PL6_RndPrimCreate(Pr, PL6_RND_PRIM_TRISTRIP, V, SplitW * SplitH, Ind, num_of_indices);
  return TRUE;
} /* End of 'PL6_RndPrimCreateGrid' function */

/* Tri-mesh geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - vertex array:
 *       pl6VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *Ind;
 *   - index array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID PL6_RndPrimTriMeshEvaNormals( pl6VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i;

  for (i = 0; i < NumOfV; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < NumOfI; i += 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p1 = V[Ind[i + 1]].P,
      p2 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

    V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
    V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
    V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }

  for (i = 0; i < NumOfV; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'PL6_RndPrimTriMeshEvaNormals' function */

/* Grid geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - grid split values:
 *       INT SplitW, SplitH;
 *   - vertex array:
 *       pl6VERTEX *V;
 * RETURNS: None.
 */
VOID PL6_RndPrimGridEvaNormals( INT SplitW, INT SplitH, pl6VERTEX *V )
{
  INT i, j;

  for (i = 0; i < SplitW * SplitH; i++)
    V[i].N = VecSet(0, 0, 0);

  for(i = 0; i < SplitH - 1; i++)
    for(j = 0; j < SplitW - 1; j++)
    {
      pl6VERTEX
        *P00 = &V[i * SplitW + j],
        *P01 = &V[i * SplitW + j + 1],
        *P10 = &V[(i + 1) * SplitW + j],
        *P11 = &V[(i + 1) * SplitW + j + 1];
      VEC N;

      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }
} /* End of 'PL6_RndPrimTriMeshEvaNormals' function */

/* Create sphere function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 *   - sphere center point:
 *       VEC C;
 *   - sphere radius:
 *       DBL R;
 *   - sphere grid split values:
 *       INT SplitW, SplitH;
 * RETURNS: 
 *     (BOOL) TRUE if success, FALSE otherwise
 */
BOOL PL6_RndPrimCreateSphere( pl6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j;
  DBL theta, phi;
  pl6VERTEX *V;
  BOOL k;

  if ((V = malloc(sizeof(pl6VERTEX) * SplitW * SplitH)) == NULL)
    return FALSE;
  memset(V, 0, sizeof(pl6VERTEX) * SplitW * SplitH);
  /* Set vertexes */
  for (theta = 0, i = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (phi = 0, j = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
    {
      V[(SplitH - i - 1) * SplitW + j].N = VecSet(sin(theta) * sin(phi), 
                                                  cos(theta), 
                                                  sin(theta) * cos(phi));
      V[(SplitH - i - 1) * SplitW + j].P = VecSet(C.X + R * sin(theta) * sin(phi), 
                                                  C.Y + R * cos(theta), 
                                                  C.Z + R * sin(theta) * cos(phi));
      V[i * SplitW + j].T = Vec2Set(j / (SplitW - 1.0), i / (SplitH - 1.0));
    }
  k = PL6_RndPrimCreateGrid(Pr, SplitW, SplitH, V);
  free(V);
  return k;
} /* End of 'PL6_RndPrimCreateSphere' function */

/* END OF 'rndprim.c' FILE */
