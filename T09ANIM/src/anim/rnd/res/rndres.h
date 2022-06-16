/* FILE NAME  : rndres.h
 * PROGRAMMER : PL6
 * LAST UPDATE: 16.06.2022
 * PURPOSE    : 3D animation project.
 *              Render subsystem implementation module.
 *              Rendering resourse handle declaration module.
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

/* Maximum resourse string size */
#define PL6_STR_MAX 300

/* Resourses system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndResInit( VOID );

/* Resourses system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndResClose( VOID );

/***
 * Shaders support
 ***/

/* Shader program store type */
typedef struct tagpl6SHADER
{
  CHAR Name[PL6_STR_MAX]; /* Shader filename prefix */
  UINT ProgId;            /* Shader program Id */
} pl6SHADER;

/* Shaders stock maximum size */
#define PL6_MAX_SHADERS 30

/* Array of shaders */
extern pl6SHADER PL6_RndShaders[PL6_MAX_SHADERS];
/* Shadres array store size */
extern INT PL6_RndShadersSize;

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndShdInit( VOID );

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndShdClose( VOID );

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndShdUpdate( VOID );

/***
 * Texture handle function group
 ***/

/* Textures stock maximum size */
#define PL6_MAX_TEXTURES 3000

/* Texture store representation type */
typedef struct tagpl6TEXTURE
{
  CHAR Name[PL6_STR_MAX]; /* Image file name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */
} pl6TEXTURE;

/* Texture resource table and size */
extern pl6TEXTURE PL6_RndTextures[PL6_MAX_TEXTURES];
extern INT PL6_RndTexturesSize;

/* Texture store system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTexInit( VOID );

/* Texture store system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTexClose( VOID );

/* Add texture image from file function.
 * ARGUMENTS:
 *   - texture image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) texture table index or -1 if error is occured.
 */
INT PL6_RndTexAddFromFile( CHAR *FileName );

/* Add texture image by image data function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture image size:
 *       INT W, H;
 *   - number of terxture pixels color components:
 *       INT C;
 *   - texture image pixel color data (DWORD colors):
 *       VOID *ImageData;
 * RETURNS:
 *   (INT) texture table index or -1 if error is occured.
 */
INT PL6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *ImageData );

/***
 * Material handle function group
 ***/

/* Materials stock maximum size */
#define PL6_MAX_MATERIALS 5000

/* Material store representation type */
typedef struct tagpl6MATERIAL
{
  CHAR Name[PL6_STR_MAX]; /* Material file name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;    /* Ambient, diffuse, specular coefficients */
  FLT Ph;            /* Phong power coefficient */
  FLT Trans;         /* Transparency factor */

  INT Tex[8];        /* Texture references from texture table */
  INT ShdNo;         /* Shader reference in shader table */
} pl6MATERIAL;

/* Material resource table and size */
extern pl6MATERIAL PL6_RndMaterials[PL6_MAX_MATERIALS];
extern INT PL6_RndMaterialsSize;

/* Material store system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndMtlInit( VOID );

/* Material store system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndMtlClose( VOID );

/* Add material function.
 * ARGUMENTS:
 *   - material data structure pointer:
 *       pl6MATERIAL *Mtl;
 * RETURNS:
 *   (INT) material table index or -1 if error is occured.
 */
INT PL6_RndMtlAdd( pl6MATERIAL *Mtl );

/* Material apply function.
 * ARGUMENTS:
 *   - material table index:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) apply result shader program identifier.
 */
INT PL6_RndMtlApply( INT MtlNo );

/* Get default material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (pl6MATERIAL) default material.
 */
pl6MATERIAL PL6_RndMtlGetDef( VOID );

/* Get material pointer by materials table number function.
 * ARGUMENTS:
 *   - number of masterial at materials table:
 *       INT MtlNo;
 * RETURNS:
 *   (pl6MATERIAL *) material pointer.
 */
pl6MATERIAL * PL6_RndMtlGet( INT MtlNo );

/* Get material by name function.
 * ARGUMENTS:
 *   - material name to find:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) found material table number.
 */
INT PL6_RndMtlGetByName( CHAR *Name );

#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */