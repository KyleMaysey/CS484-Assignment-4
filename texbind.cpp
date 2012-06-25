/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*  texbind.c
 *  This program demonstrates using glBindTexture() by 
 *  creating and managing two textures.
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "Texture.h"
#include "GLSLShader.h"
#include "SurfaceGeometryFaceList.h"
#include "SurfaceGeometry.h"
//using namespace msgfx;
 
#define ISOTROPIC_TEXTURE "melon_skin_64.png"
#define ANISOTROPIC_TEXTURE "letter_A_64.tif"
msgfx::Texture* gImg[2];
GLUquadric *gQuadric;
SurfaceGeometryFaceList *gModel[5];
size_t gNumModels;

unsigned int gCurrentModel;
float xangle;
float yangle;
char gMode;


void init(void){
  // OpenGL Environment
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);

  // quadric for drawing a sphere
  gQuadric = gluNewQuadric();
  gluQuadricDrawStyle(gQuadric, GLU_FILL);
  gluQuadricNormals(gQuadric, GL_SMOOTH);
  gluQuadricTexture(gQuadric, GL_TRUE);

  // Load images, create textures
  gImg[0] = new msgfx::Texture( ISOTROPIC_TEXTURE );
  gImg[1] = new msgfx::Texture( ANISOTROPIC_TEXTURE );
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for(int i = 0; i < 2; i++ ){
   gImg[i]->assignToTextureUnit( i );
   gImg[i]->parameters( GL_TEXTURE_WRAP_S, GL_REPEAT );
   gImg[i]->parameters( GL_TEXTURE_WRAP_T, GL_REPEAT );
   gImg[i]->parameters( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   gImg[i]->parameters( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   gImg[i]->load( );
  }

  // Load shaders, build shading program
  msgfx::FragmentShader fs("simpletexture.frag");
  msgfx::VertexShader vs("simpletexture.vert");
  
  msgfx::GLSLProgram renderer;
  if( ! renderer.attach( vs ) ){
    fprintf( stderr, "Couldn't attach vertex shader." );
    exit(1);
  }
  if( ! renderer.attach( fs ) ){
    fprintf( stderr, "Couldn't attach fragment shader." );
    exit(1);
  }
  if( ! renderer.link( ) ){
    fprintf( stderr, "Couldn't attach link shaders." );
    exit(1);
  }
  if( ! renderer.activate( ) ){
    fprintf( stderr, "Unable to activate the shader.\n" );
    exit(1);
  }
  
  // Set up uniform variables
  renderer.setUniformLocation( "LightPosition",  2.0, 2.0, -2.0 );
  renderer.setUniformLocation( "IsotropicTexture", 0 );
  renderer.setUniformLocation( "AnisotropicTexture", 1 );

  gCurrentModel = 0;
  gMode = 0;
  gNumModels = 5;
  xangle = 0.0;
  yangle = 0.0;
  
  // Build geometry
  for( unsigned int i = 1; i <= gNumModels; i++ ){
    gModel[i-1] = BuildGeometry( i, 16, 32, 12 );
    assert(gModel[i-1]);
  }
  
}

void drawSurface( SurfaceGeometryFaceList *f ){
  int i = 0;
  int j = 0;
  int index;
  for( i = 0; i < (f->maxI); i++ ){
    glBegin( GL_TRIANGLE_STRIP );
    for( j = 0; j <= (f->maxJ); j++ ){
      index = (i % (f->maxI)) * (f->maxJ) + (j % (f->maxJ));
      glColor3fv( f->colors[index] );
      glNormal3fv( f->v_normals[index] );
      glTexCoord2fv( f->texcoords[index] );
      glVertex3fv( f->vertices[index] );
      
      index = ((i + 1) % f->maxI) * f->maxJ + (j % f->maxJ);
      glColor3fv( f->colors[index] );
      glNormal3fv( f->v_normals[index] );
      glTexCoord2fv( f->texcoords[index] );
      glVertex3fv( f->vertices[index] );
    }
    glEnd( );
  }
}

void display(void){  
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity( );
  
   gluLookAt( 0.0, 0.0, -4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
   glRotatef( xangle, 1.0, 1.0, 1.0 );
   glRotatef( yangle, 0.0, 1.0, 1.0 );

   switch(gMode){
     case 0:
     gluSphere( gQuadric, 1.25, 60, 60 );
     break;
     case 1:
     glBegin(GL_QUADS);
     glNormal3f( 0.0, 0.0, 1.0); glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
     glNormal3f( 0.0, 0.0, 1.0); glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
      glNormal3f( 0.0, 0.0, 1.0); glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
      glNormal3f( 0.0, 0.0, 1.0); glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
     glEnd();
     glBegin(GL_QUADS);
     glNormal3f( 0.70711, 0.00000, 0.70711); glTexCoord2f(1.0, 0.0);  glVertex3f(2.41421, -1.0, -1.41421);
    glNormal3f( 0.70711, 0.00000, 0.70711); glTexCoord2f(1.0, 1.0);  glVertex3f(2.41421, 1.0, -1.41421);
    glNormal3f( 0.70711, 0.00000, 0.70711); glTexCoord2f(0.0, 1.0);  glVertex3f(1.0, 1.0, 0.0);
    glNormal3f( 0.70711, 0.00000, 0.70711); glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
     glEnd();
     break;
     case 2:
      drawSurface( gModel[gCurrentModel] );
     break;
 }
   glFlush();
   glutSwapBuffers( );
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
    exit(0);
    break;
    case 'c':
    gCurrentModel = (gCurrentModel + 1) % gNumModels;
    break;
    case 'h':
    yangle += 1.0;
    break;
    case 'l':
    yangle -= 1.0;
    break;
    case 'j':
    xangle -= 1.0;
    break;
    case 'k':
    xangle += 1.0;
    break;
    case 'm':
    gMode = (gMode + 1) % 3;
    break;
  }
  glutPostRedisplay();
}

bool supportsOpenGLVersion( int atLeastMajor, int atLeastMinor ){
  const char *version;
  int major, minor;
  bool ret = false;
  version = (const char *) glGetString(GL_VERSION);
  if (sscanf(version, "%d.%d", &major, &minor) == 2){
    //fprintf( stderr, "GL Version: major: %d, minor: %d\n", major, minor );
    if(( major < atLeastMajor ) ||  
       ( major == atLeastMajor && minor >= atLeastMinor ) )
     {
       ret = true;
     }
  }
  return( ret );
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   Magick::InitializeMagick(*argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   if( supportsOpenGLVersion( 2, 0 ) ){
     fprintf( stderr, "Congrats! OpenGL Shading Language is supported.\n" );
   }else{
     fprintf( stderr, "OpenGL Shading Language not supported. Sorry.\n" );
     exit(1);
   }
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0; 
}
