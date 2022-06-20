/* FILE       : rndlight.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 20.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */

#include "pl6.h"

/***
 * Light system global data
 ***/

/* Shadow frame buffer identifier */
INT PL6_RndShadowFBO;

/* Shadow map texture identifier */
INT PL6_RndShadowTexId;

/* Shadow map light source matrix */
MATR PL6_RndShadowMatr;

/* Flag for shadow drawing rendering pass */
BOOL PL6_RndShadowPassFlag;

/* Light source direction */
VEC PL6_RndLightDir;

/* Light source color */
VEC PL6_RndLightColor;

/* Light system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndLightInit( VOID )
{
  INT tex;

  /* Set default light sourse */
  PL6_RndLightDir = VecNormalize(VecSet(10, 10, 10));
  PL6_RndLightColor = VecSet(1, 1, 1);

  /* Create shadow FBO */
  glGenFramebuffers(1, &PL6_RndShadowFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, PL6_RndShadowFBO);

  /* Create shadow map texture */
  tex = PL6_RndTexAddFmt("ShadowMap", PL6_RND_SHADOW_MAP_SIZE, PL6_RND_SHADOW_MAP_SIZE, GL_DEPTH_COMPONENT32);
  PL6_RndShadowTexId = PL6_RndTextures[tex].TexId;

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, PL6_RndShadowTexId, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
} /* End of 'PL6_RndLightInit' function */

/* Light system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndLightClose( VOID )
{
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
  glDeleteFramebuffers(1, &PL6_RndShadowFBO);
} /* End of 'PL6_RndLightClose' function */

/* Draw shadow map function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndLightShadow( VOID )
{
  INT i;
  FLT depth = 1, size = 30;
  MATR SaveRndMatrView, SaveRndMatrProj;
  VEC
    SaveRndCamLoc,
    SaveRndCamAt,
    SaveRndCamRight,
    SaveRndCamUp,
    SaveRndCamDir;

  glBindFramebuffer(GL_FRAMEBUFFER, PL6_RndShadowFBO);
  glClearBufferfv(GL_DEPTH, 0, &depth);

  /* Save camera parameters */
  SaveRndMatrView = PL6_RndMatrView;
  SaveRndMatrProj = PL6_RndMatrProj;
  SaveRndCamLoc = PL6_RndCamLoc;
  SaveRndCamAt = PL6_RndCamAt;
  SaveRndCamRight = PL6_RndCamRight;
  SaveRndCamUp = PL6_RndCamUp;
  SaveRndCamDir = PL6_RndCamDir;

  PL6_RndMatrView = MatrView(VecAddVec(PL6_RndCamAt, VecMulNum(PL6_RndLightDir, 30)), PL6_RndCamAt, VecSet(0, 1, 0));
  PL6_RndMatrProj = MatrOrtho(-size, size, -size, size, 0, 100);
  PL6_RndMatrVP = MatrMulMatr(PL6_RndMatrView, PL6_RndMatrProj);
  PL6_RndShadowMatr = PL6_RndMatrProj;

  PL6_RndShadowPassFlag = TRUE;
  /* Render all units */
  for (i = 0; i < PL6_Anim.NumOfUnits; i++)
    PL6_Anim.Units[i]->Render(PL6_Anim.Units[i], &PL6_Anim);
  PL6_RndShadowPassFlag = FALSE;

  /* Restore camera parameters */
  PL6_RndMatrView = SaveRndMatrView;
  PL6_RndMatrProj = SaveRndMatrProj;
  PL6_RndCamLoc = SaveRndCamLoc;
  PL6_RndCamAt = SaveRndCamAt;
  PL6_RndCamRight = SaveRndCamRight;
  PL6_RndCamUp = SaveRndCamUp;
  PL6_RndCamDir = SaveRndCamDir;
  PL6_RndMatrVP = MatrMulMatr(PL6_RndMatrView, PL6_RndMatrProj);

  glFinish();
} /* End of 'PL6_RndLightShadow' function */

/* END OF 'rndlight.c' FILE */