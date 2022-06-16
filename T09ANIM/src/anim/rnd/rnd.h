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

#define GLEW_STATIC
#include <glew.h>

#include "def.h"
#include "res/rndres.h"

extern HWND PL6_hRndWnd;                 /* Work window handle */
extern HDC PL6_hRndDC;                   /* Work window memory device context  */
extern HGLRC PL6_hRndGLRC;               /* Work OpenGL rendering context */
extern INT PL6_RndFrameW, PL6_RndFrameH; /* Work window size */

extern DBL
  PL6_RndProjSize,     /* Project plane fit square */
  PL6_RndProjDist,     /* Distance to project plane from viewer (near) */
  PL6_RndProjFarClip;  /* Distance to project far clip plane (far) */

extern VEC
  PL6_RndCamLoc,    /* Camera location */
  PL6_RndCamAt;     /* Camera 'look at' point */
  PL6_RndCamRight,  /* Camera right direction */
  PL6_RndCamUp,     /* Camera up direction */
  PL6_RndCamDir;    /* Camera forward direction */

extern MATR
  PL6_RndMatrView, /* View coordinate system matrix */
  PL6_RndMatrProj, /* Projection coordinate system matrix */
  PL6_RndMatrVP;   /* Stored (View * Proj) matrix */

typedef struct tagpl6VERTEX
{
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
} pl6VERTEX;

/* Primitive type */
typedef enum tagpl6PRIM_TYPE
{
  PL6_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles */
  PL6_RND_PRIM_TRISTRIP, /* Triangle strip - array of stripped triangles */
  PL6_RND_PRIM_LINES,    /* Line segments (by 2 points) */
  PL6_RND_PRIM_POINTS,   /* Arrauy of points */
} pl6PRIM_TYPE;


typedef struct tagpl6PRIM
{
  pl6PRIM_TYPE Type; /* Primitive type */
  INT
    VA,             /* Vertex array OpenGL id */
    VBuf,           /* Vertex buffer OpenGL id */
    IBuf,           /* Index buffer OpenGL id */
    NumOfV,         /* Number of vertexes */
    NumOfI,         /* Number of indexes */
    NumOfElements;  /* Number of draw points */
  MATR Trans;       /* Transformation matrix */
  VEC MinBB, MaxBB; /* Minimum and maximum bound box cooerdinates */
  INT MtlNo;        /* Material number in material array */
} pl6PRIM;

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndInit( HWND hWnd );

/* Rendering system deinitialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID PL6_RndClose( VOID );

/* Rendering system frame resize handle function.
 * ARGUMENTS: 
 *     - new frame size:
 *          INT W, INT H;
 * RETURNS: None.
 */
VOID PL6_RndResize( INT W, INT H );

/* Setup rendering projection function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_RndProjSet( VOID );

/* Rendering system copy frame to DC function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndCopyFrame( VOID );

/* Rendering system viewer/camera function.
 * ARGUMENTS: 
 *     - camera position:
 *         VEC Loc;
 *     - camera oint of interest ('look at' point):
 *         VEC At;
 *     - camera upproximatly up direction:
 *         VEC Up;
 * RETURNS: None.
 */
VOID PL6_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Rendering system start frame drawing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndStart( VOID );

/* Rendering system end frame drawing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndEnd( VOID );

/* Primitive drawing function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 *   - addtional transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID PL6_RndPrimDraw( pl6PRIM *Pr, MATR World );

/* Primitive free function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 * RETURNS: None.
 */
VOID PL6_RndPrimFree( pl6PRIM *Pr );

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
BOOL PL6_RndPrimCreate( pl6PRIM *Pr, pl6PRIM_TYPE Type, pl6VERTEX *V, INT NoofV, INT *I, INT NoofI );

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       pl6PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *       (BOOL) TRUE if succes, FALSE otherwise.
 */
BOOL PL6_RndPrimLoad( pl6PRIM *Pr, CHAR *FileName );

/* Create grid function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       pl6PRIM *Pr;
 *   - grid split values:
 *       INT SplitW, SplitH;
 *   - agrid vertex:
 *       pl6VERTEX *V;
 * RETURNS: 
 *     (BOOL) TRUE if success, FALSE otherwise
 */
BOOL PL6_RndPrimCreateGrid( pl6PRIM *Pr, INT SplitW, INT SplitH, pl6VERTEX *V );

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
VOID PL6_RndPrimTriMeshEvaNormals( pl6VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI );

/* Grid geometry autonormal evaluation function.
 * ARGUMENTS:
 *   - grid split values:
 *       INT SplitW, INT SplitH;
 *   - vertex array:
 *       pl6VERTEX *V;
 * RETURNS: None.
 */
VOID PL6_RndPrimGridEvaNormals( INT SplitW, INT SplitH, pl6VERTEX *V );

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
BOOL PL6_RndPrimCreateSphere( pl6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */
