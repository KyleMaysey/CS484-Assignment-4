/*
 * Copyright (c) 2005-2011 Michael Shafae
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: Texture.h 3303 2011-11-16 17:56:59Z mshafae $
 *
 *
 * Reading image files using GraphicsMagick into an object for texturing
 *
 */

#ifndef _MSGFX_TEXTURE_H_
#define _MSGFX_TEXTURE_H_

#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>
#include <Magick++.h>


namespace Magick{
  std::ostream& operator <<( std::ostream& out, const Magick::ColorRGB& c );
};
/*
 * Current it's only one format RGB and one type GLubyte. Maybe more later.
 */
namespace msgfx{
  class Texture{
  public:
    explicit Texture( );
    explicit Texture( size_t rows, size_t cols );
    explicit Texture( const char *filename );
    ~Texture( );
    bool bind( );
    bool assignToTextureUnit( GLuint unit );
    bool parameters( GLenum pname, GLfloat param );    
    bool parameters( GLenum pname, GLint param );
    bool environmentParameters( GLint param );
    bool environmentParameters( GLfloat param );
    bool load( );
    GLubyte *_image;
    GLuint _name;
    GLenum _target;
    size_t _rows;
    size_t _cols;
    // For the future...
    GLenum _format;
    GLenum _type;
  };
};

#endif

