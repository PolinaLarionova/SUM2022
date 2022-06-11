/* FILE       : rnd.h
 * PROGRAMMER : PL6
 * LAST UPDATE: 09.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */
#ifndef __rnd_h_
#define __rnd_h_
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "def.h"

extern HWND PL6_hRndWnd;        /* Work window handle */
extern HDC PL6_hRndDCFrame;     /* Work window memory device context  */
extern HBITMAP PL6_hRndBmFrame; /* Work window background bitmap handle */
extern INT PL6_RndFrameW, PL6_RndFrameH; /* Work window size */

extern POINT *PL6_RndProjPoints; /* Buffer for store projection points for primitive rendering */
extern INT PL6_RndProjPointsSize; /* Projection storage buffer size */

extern DBL
  PL6_RndProjSize,  /* Project plane fit square */
  PL6_RndProjDist,  /* Distance to project plane from viewer (near) */
  PL6_RndProjFarClip;  /* Distance to project far clip plane (far) */


extern MATR
  PL6_RndMatrView, /* View coordinate system matrix */
  PL6_RndMatrProj, /* Projection coordinate system matrix */
  PL6_RndMatrVP;   /* Stored (View * Proj) matrix */

typedef struct tagpl6VERTEX
{
  VEC P;
} pl6VERTEX;

typedef struct tagpl6PRIM
{
  pl6VERTEX *V;
  INT NumOfV;
  INT *I;
  INT NumOfI;
  MATR Trans;
} pl6PRIM;

VOID PL6_RndInit( HWND hWnd );
VOID PL6_RndClose( VOID );
VOID PL6_RndResize( INT W, INT H );
VOID PL6_RndProjSet( VOID );
VOID PL6_RndCopyFrame( HDC hDC );
VOID PL6_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID PL6_RndStart( VOID );
VOID PL6_RndEnd( VOID );
VOID PL6_RndPrimDraw( pl6PRIM *Pr, MATR World );
VOID PL6_RndPrimFree( pl6PRIM *Pr );
BOOL PL6_RndPrimCreate( pl6PRIM *Pr, INT NoofV, INT NoofI );
BOOL PL6_RndPrimLoad( pl6PRIM *Pr, CHAR *FileName );


#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */
