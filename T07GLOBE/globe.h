/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: PL6
 * DATE: 06.06.2020
 * PURPOSE: globe
 */
#include "mth.h"
#include "timer.h"

#define GRID_H 50
#define GRID_W 26
VOID GlobeSet( DBL R );
VOID GlobeDraw( HDC hDC, INT w, INT h );
VEC RotateZ( VEC P, DBL angle );
VEC RotateX( VEC P, DBL angle );
VEC RotateY( VEC P, DBL angle );