/* FILE       : rndtrgt.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 20.06.2022
 * PURPOSE    : 3D animation project.
 */

#include <string.h>

#include "pl6.h"

/***
 * Render target variables
 ***/

/* FBO number */
static UINT PL6_RndTargetFBO;

/* Color textures */
#define PL6_RND_TARGETS 2
static INT PL6_RndTargetTex[PL6_RND_TARGETS];

/* Render buffer */
static INT PL6_RndTargetRndBuf;

/* Target shaders */
static INT PL6_RndTargetShd;

/* Target primitive */
static pl6PRIM PL6_RndTargetPrim;

/***
 * Render target handle functions
 ***/

/* Initialize render target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTargetInit( VOID )
{
  pl6VERTEX
    V[] =
  {
    {(-1, 1, 0, 1), (1, 0)},
    {(-1, -1, 0, 1), (0, 0)},
    {(1, 1, 0, 1), (0, 1)},
    {(1, -1, 0, 1), (1, 1)}
  };
  pl6MATERIAL mtl;
  INT i;

  for (i = 0; i < PL6_RND_TARGETS; i++)
    PL6_RndTargetTex[i] = -1;

  PL6_RndTargetShd = PL6_RndShdAdd("target0");

  PL6_RndPrimCreate(&PL6_RndTargetPrim, PL6_RND_PRIM_TRISTRIP, V, 4, NULL, 0);
  mtl = PL6_RndMtlGetDef();
  strcpy(mtl.Name, "Target Material");
  mtl.ShdNo = PL6_RndTargetShd;
  PL6_RndTargetPrim.MtlNo = PL6_RndMtlAdd(&mtl); 

  PL6_RndTargetCreate(PL6_RndFrameW, PL6_RndFrameH);
} /* End of 'PL6_RndTargetInit' function */

/* Deinitialize render target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTargetClose( VOID )
{
  PL6_RndTargetFree();
} /* End of 'PL6_RndTargetClose' function */

/* Create render target function.
 * ARGUMENTS:
 *   - target frame buffer size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID PL6_RndTargetCreate( INT W, INT H )
{
  INT i, DrawBuffer[PL6_RND_TARGETS], status;

  /* Create frame buffer */
  glGenFramebuffers(1, &PL6_RndTargetFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, PL6_RndTargetFBO);

  /* Create attachment textures */
  for (i = 0; i < PL6_RND_TARGETS; i++)
  {
    DrawBuffer[i] = GL_COLOR_ATTACHMENT0 + i;

    if (PL6_RndTargetTex[i] == -1)
    {
      pl6MATERIAL *mtl = PL6_RndMtlGet(PL6_RndTargetPrim.MtlNo);

      PL6_RndTargetTex[i] = PL6_RndTexAddFmt("TARGET", W, H, GL_RGBA32F);
      mtl->Tex[i] = PL6_RndTargetTex[i];
    }
    else
    {
      glDeleteTextures(PL6_RND_TARGETS, &PL6_RndTextures[PL6_RndTargetTex[i]].TexId);
      glGenTextures(PL6_RND_TARGETS, &PL6_RndTextures[PL6_RndTargetTex[i]].TexId);
      glBindTexture(GL_TEXTURE_2D, PL6_RndTextures[PL6_RndTargetTex[i]].TexId);
      glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, W, H);
      glBindTexture(GL_TEXTURE_2D, 0);
      PL6_RndTextures[PL6_RndTargetTex[i]].H = H;
      PL6_RndTextures[PL6_RndTargetTex[i]].W = W;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, PL6_RndTextures[PL6_RndTargetTex[i]].TexId, 0);
  }
  glDrawBuffers(PL6_RND_TARGETS, DrawBuffer);

  /* Create depth bvuffer as render buffer */
  glGenRenderbuffers(1, &PL6_RndTargetRndBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, PL6_RndTargetRndBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, W, H);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, PL6_RndTargetRndBuf);

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    OutputDebugString("Error create FBO\n");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
} /* End of 'PL6_RndTargetCreate' function */

/* Free render target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTargetFree( VOID )
{
  INT i;

  glBindFramebuffer(GL_FRAMEBUFFER, PL6_RndTargetFBO);
  for (i = 0; i < PL6_RND_TARGETS; i++)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
  /* glDeleteTextures(PL6_RND_TARGETS, PL6_RndTargetTex); */
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
  glDeleteRenderbuffers(1, &PL6_RndTargetRndBuf);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &PL6_RndTargetFBO);
} /* End of 'PL6_RndTargetFree' function */

/* Start frame through target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTargetStart( VOID )
{
  INT i;
  FLT col[PL6_RND_TARGETS][4] = {{0, 0.3, 0.6, 1}}, depth = 1;

  PL6_RndLightShadow();
  glPolygonOffset(4, 2);

  glBindFramebuffer(GL_FRAMEBUFFER, PL6_RndTargetFBO);
  for (i = 0; i < PL6_RND_TARGETS; i++)
    glClearBufferfv(GL_COLOR, i, col[i]);
  glClearBufferfv(GL_DEPTH, 0, &depth);
  glViewport(0, 0, PL6_RndFrameW, PL6_RndFrameH);
} /* End of 'PL6_RndTargetStart' function */

/* Finalize frame through target system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndTargetEnd( VOID )
{
  FLT col[4] = {0, 0.3, 0.6, 1}, depth = 1;

  glFinish();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, PL6_RndFrameW, PL6_RndFrameH);
  glClearBufferfv(GL_COLOR, 0, col);
  glClearBufferfv(GL_DEPTH, 0, &depth);

  /*for (i = 0; i < PL6_RND_TARGETS; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, PL6_RndTargetTex[i]);
  }*/

  /*glDisable(GL_DEPTH_TEST); 
  glUseProgram(PL6_RndShaders[PL6_RndTargetShd].ProgId);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glUseProgram(0);*/
  PL6_RndPrimDraw(&PL6_RndTargetPrim, MatrIdentity());
  glFinish();
} /* End of 'PL6_RndTargetEnd' function */

/* Resize render target frame buffer function.
 * ARGUMENTS:
 *   - target frame buffer size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID PL6_RndTargetResize( INT W, INT H )
{
  PL6_RndTargetFree();
  PL6_RndTargetCreate(W, H);
} /* End of 'PL6_RndTargetResize' function */

/* END OF 'rndtrgt.c' FILE */