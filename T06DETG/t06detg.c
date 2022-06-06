/* FILE NAME: T06DETG.C
 * PROGRAMMER: PL6
 * DATE: 04.06.2020
 * PURPOSE: 
 */
#include <stdio.h>
#include <windows.h>

/* Helpful float point double precision type */
typedef DOUBLE DBL;
 
/* Atrray max size */
#define MAX 100

/* Source matrix */
DBL A[MAX][MAX];

/* Source matrix size */
INT N;
 
/* Load matrix function.
 * ARGUMENTS:
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;
 
  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
}/* End of 'LoadMatrix' function */

/* Swap function 
 * ARGUMENTS:
 *         INT A, INT B
*/
VOID Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

DBL FindDet ( void )
{
  INT i, max_row, max_col, y, x, k;
  DBL det, coef;

  det = 1;
  for (i = 0; i < N; i++)
  {
    /* look for maximum matrix element */
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_row = y, max_col = x;
    if (A[max_row][max_col] == 0)
    {
      det = 0;
      break;
    }
    if (max_row != i)
    {
      /* Swap max_row and i row (elements/columns: [i..N-1]) */
      for (x = i; x < N; x++)
        Swap(&A[max_row][x], &A[i][x]);
      det = -det;
    }
    if (max_col != i)
    {
      /* Swap max_col and i column (elements/columns: [0..N-1]) */
      for (x = i; x < N; x++)
        Swap(&A[x][max_col], &A[x][i]);
      det = -det;
    }
    /* Subtrack from every row k:[i+1..N-1] row [i] multipled by (A[k][i] / A[i][i]) */
    for (k = i + 1; k < N; k++)
    {
      coef = A[k][i] / A[i][i];
      A[k][i] = 0;
      for (x = i + 1; x < N; x++)
      A[k][x] -= A[i][x] * coef;
    }
    
    det *= A[i][i];
  }
  return det;
}/* End of 'FindDet' function */

/*main function*/
void main( VOID )
{
  if (LoadMatrix("matrix1.txt"))
    printf("%f", FindDet());
  else
    printf("Error");
  getchar();
  getchar();
}/* End of t06detg.c */