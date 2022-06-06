/* FILE NAME: T05DET.C
 * PROGRAMMER: PL6
 * DATE: 04.06.2020
 * PURPOSE: 
 */
#include <stdio.h>
#include <windows.h>

/* Helpful float point double precision type */
typedef DOUBLE DBL;
 
/* Atrray max size */
#define MAX 10

/* Source matrix */
DBL A[MAX][MAX], Det;

/* Source matrix size *//* Permutation table */
INT N, P[MAX];

/* Current permutation parity flag */ 
BOOL IsParity;
 
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
VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

/* Go function (count Det)
 * ARGUMENTS:
 *         INT Pos
*/
VOID Go( INT Pos )
{
  INT i, x;
  DBL prod;

  if (Pos == N)
  {
    for (i = 0, prod = 1; i < N; i++)
      prod *= A[i][P[i]];
    Det += prod * (IsParity ? 1 : -1);
  }
  else
  {
    BOOL save_parity = IsParity;

    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      IsParity = !IsParity;
      Go(Pos + 1);
    }
    IsParity = save_parity;
    x = P[Pos];
    for (i = Pos + 1; i < N; i++)
      P[i - 1] = P[i];
      P[N - 1] = x;
  }
}/* End of 'Go' function */

/*main function*/
void main( void )
{
  INT i;

  if (LoadMatrix("matrix.txt"))
  {  
    for (i = 0; i < N; i++)
      P[i] = i;
    IsParity = TRUE;
    Go(0);
    printf("%f", Det);
  }
  else
    printf("Error");
  getchar();
  getchar();
}/* End of t05det.c */