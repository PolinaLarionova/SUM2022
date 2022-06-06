/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: PL6
 * DATE: 06.06.2020
 * PURPOSE: globe
 */
typedef DOUBLE DBL;
typedef struct
{
  DBL X, Y, Z;
} VEC;
#define GRID_H 50
#define GRID_W 26
#define pi 3.14159265358979323846
VOID GlobeSet( DBL R );
VOID GlobeDraw( HDC hDC, INT w, INT h);
VEC RotateZ( VEC P, DBL angle );
VEC RotateX( VEC P, DBL angle );
VEC RotateY( VEC P, DBL angle );