/*
 *
 * Your contact info here
 *
 * $Id: misc.h 3305 2011-11-16 17:58:30Z mshafae $
 *
 * Utility functions
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifndef _MISC_H_
#define _MISC_H_

void vecDifference( float a[3], float b[3], float c[3] );

void crossProduct( float a[3], float b[3], float c[3] );

void vecNormalize( float n[3] );

void material( int mode );

#endif