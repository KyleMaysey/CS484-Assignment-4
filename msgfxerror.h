/*
 * Copyright (c) 2005-2011 Michael Shafae.
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: msgfxerror.h 3303 2011-11-16 17:56:59Z mshafae $
 *
 * Error reporting routines and macros
 *
 */

#ifndef _MSGFX_ERROR_H_
#define _MSGFX_ERROR_H_

#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
/* MS Windows, Unix, Linux, and BSD */
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <cstdio>


#ifndef NDEBUG
#define msglError( ) msgfx::_msglError( stderr, __FILE__, __LINE__ )
#define msglAssert( ) assert( glGetError( ) == GL_NO_ERROR )
#else
#define msglError( ) true
#define msglAssert( )
#endif

namespace msgfx{
  bool _msglError( FILE *out, const char *filename, int line );
};

#endif