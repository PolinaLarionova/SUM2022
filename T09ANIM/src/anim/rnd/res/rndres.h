/* FILE NAME  : rndres.h
 * PROGRAMMER : PL6
 * LAST UPDATE: 14.06.2022
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



#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */
