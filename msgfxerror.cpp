/*
 * Copyright (c) 2005-2011 Michael Shafae.
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: msgfxerror.cpp 3303 2011-11-16 17:56:59Z mshafae $
 *
 * Error reporting routines and macros
 *
 */

#include "msgfxerror.h"

namespace msgfx{
  bool _msglError( FILE *out, const char *filename, int line ){
    bool ret = false;
    GLenum err = glGetError( );
    while( err != GL_NO_ERROR ) {
      ret = true;
      fprintf( out, "GL ERROR:%s:%d: %s\n", filename, line, (char*)gluErrorString( err ) );
        err = glGetError( );
    }
    return( ret );
  }
};