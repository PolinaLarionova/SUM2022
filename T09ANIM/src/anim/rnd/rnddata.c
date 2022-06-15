/* FILE       : rnddata.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 09.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */

#include "rnd.h"

HWND PL6_hRndWnd;        /* Work window handle */
HDC PL6_hRndDC;     /* Work window memory device context  */
HGLRC PL6_hRndGLRC;               /* Work OpenGL rendering context */
INT PL6_RndFrameW, PL6_RndFrameH; /* Work window size */

DBL
  PL6_RndProjSize = 0.1,  /* Project plane fit square */
  PL6_RndProjDist = 0.1,  /* Distance to project plane from viewer (near) */
  PL6_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */


MATR
  PL6_RndMatrView, /* View coordinate system matrix */
  PL6_RndMatrProj, /* Projection coordinate system matrix */
  PL6_RndMatrVP;   /* Stored (View * Proj) matrix */

/* END OF 'rnddata.c' FILE */