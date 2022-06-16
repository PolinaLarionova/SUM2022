/* FILE       : rndbase.c
 * PROGRAMMER : PL6
 * LAST UPDATE: 09.06.2022
 * PURPOSE    : 3D animation project.
 *              Startup module.
 */
#include "pl6.h"
#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndInit( HWND hWnd )
{
  INT i, nums;
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;

  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };

  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };

  PL6_hRndWnd = hWnd;
  PL6_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(PL6_hRndDC, &pfd);
  DescribePixelFormat(PL6_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(PL6_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  PL6_hRndGLRC = wglCreateContext(PL6_hRndDC);
  wglMakeCurrent(PL6_hRndDC, PL6_hRndGLRC);

  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(PL6_hRndWnd, "Error extensions initializing", "Error",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(PL6_hRndWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }

  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(PL6_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(PL6_hRndDC, NULL, ContextAttribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(PL6_hRndGLRC);

  PL6_hRndGLRC = hRC;
  wglMakeCurrent(PL6_hRndDC, PL6_hRndGLRC);
  glClearColor(0, 0.3, 0.6, 1);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);
  PL6_RndResInit();
}/* End of 'PL6_RndInit' function */

/* Rendering system deinitialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID PL6_RndClose( VOID )
{
  PL6_RndResClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(PL6_hRndGLRC);
  ReleaseDC(PL6_hRndWnd, PL6_hRndDC);
}/* End of 'PL6_RndClose' function */

/* Rendering system frame resize handle function.
 * ARGUMENTS: 
 *     - new frame size:
 *          INT W, INT H;
 * RETURNS: None.
 */
VOID PL6_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);
  /* setup */
  PL6_RndFrameW = W;
  PL6_RndFrameH = H;
  PL6_RndProjSet();
}/* End of 'PL6_RndResize' function */

/* Setup rendering projection function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID PL6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = PL6_RndProjSize;
  /* Correct aspect ratio */
  if (PL6_RndFrameW > PL6_RndFrameH)
    rx *= (DBL)PL6_RndFrameW / PL6_RndFrameH;
  else
    ry *= (DBL)PL6_RndFrameH / PL6_RndFrameW;

  PL6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2, PL6_RndProjDist, PL6_RndProjFarClip);
  PL6_RndMatrVP = MatrMulMatr(PL6_RndMatrView, PL6_RndMatrProj);
} /* End of 'PL6_RndProjSet' function */

/* Rendering system copy frame to DC function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndCopyFrame( VOID )
{
  SwapBuffers(PL6_hRndDC);
} /* End of 'PL6_RndCopyFrame' function */


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
VOID PL6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  PL6_RndMatrView = MatrView(Loc, At, Up);
  PL6_RndCamLoc = Loc;
  PL6_RndCamAt = At;
  PL6_RndCamRight = VecSet(PL6_RndMatrView.A[0][0], PL6_RndMatrView.A[1][0], PL6_RndMatrView.A[2][0]);
  PL6_RndCamUp = VecSet(PL6_RndMatrView.A[0][1], PL6_RndMatrView.A[1][1], PL6_RndMatrView.A[2][1]);
  PL6_RndCamDir = VecSet(-PL6_RndMatrView.A[0][2], -PL6_RndMatrView.A[1][2], -PL6_RndMatrView.A[2][2]);
  PL6_RndMatrVP = MatrMulMatr(PL6_RndMatrView, PL6_RndMatrProj);
} /* End of 'PL6_RndCamSet' function */

/* Rendering system start frame drawing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndStart( VOID )
{
  static DBL UpdateTime = 0;

  if(PL6_Anim.GlobalTime - UpdateTime > 2)
  {
    UpdateTime = PL6_Anim.GlobalTime;
    PL6_RndShdUpdate();
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} /* End of 'PL6_RndStart' function */

/* Rendering system end frame drawing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID PL6_RndEnd( VOID )
{
  glFinish();
} /* End of 'PL6_RndEnd' function */

/* END OF 'rndbase.c' FILE */