/* FILE       : unit.h
 * PROGRAMMER : PL6
 * LAST UPDATE: 22.06.2022
 * PURPOSE    : 3D animation project.
 *              
 */
#ifndef __unit_h_
#define __unit_h_

#include "anim/anim.h"

/* Unit_Cow create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateCow( VOID );

/* Unit_Ball create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateBall( VOID );

/* Unit_Control create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateControl( VOID );

/* Unit_Land create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateLand( VOID );

/* Unit_G3dm create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateG3dm( VOID );

/* Unit_Rain create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateRain( VOID );

/* Unit_Goose create function.
 * ARGUMENTS:
 *   - none;
 * RETURNS: (pl6UNIT) unit.
 */
pl6UNIT * PL6_UnitCreateGoose( VOID );

#endif /* __unit_h_ */

/* END OF 'unit.h' FILE */