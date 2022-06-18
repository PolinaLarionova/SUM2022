/* FILE       : unit.h
 * PROGRAMMER : PL6
 * LAST UPDATE: 10.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */
#ifndef __unit_h_
#define __unit_h_

#include "anim/anim.h"
pl6UNIT * PL6_UnitCreateCow( VOID );
pl6UNIT * PL6_UnitCreateBall( VOID );
pl6UNIT * PL6_UnitCreateControl( VOID );
pl6UNIT * PL6_UnitCreateLand( VOID );

/* Unit_G3dm create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateG3dm( VOID );

#endif /* __unit_h_ */

/* END OF 'unit.h' FILE */