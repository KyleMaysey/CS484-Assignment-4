/*
 * Copyright (c) 2005-2011 Michael Shafae
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: Texture.cpp 3303 2011-11-16 17:56:59Z mshafae $
 *
 *
 * Reading image files using GraphicsMagick into an object for texturing
 *
 */

#include "Texture.h"
#include "msgfxerror.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

namespace Magick{
  std::ostream& operator <<( std::ostream& out, const Magick::ColorRGB& c ){
    out << "RGB (" << c.red( ) << ", " << c.green( ) << ", " << c.blue( ) << ")";
    return(out);
  }  
};

namespace msgfx{
  /*Texture::Texture( ){
    _image = NULL;
    _rows = -1;
    _cols = -1;
    _format = GL_RGB;
    _type = GL_UNSIGNED_BYTE;
    _target = GL_TEXTURE_2D;
  }
  Texture::Texture( size_t rows, size_t cols ){
    _image = new GLubyte[3 * rows * cols];
    _rows = _rows;
    _cols = _cols;
    _format = GL_RGB;
    _type = GL_UNSIGNED_BYTE;
    _target = GL_TEXTURE_2D;
  }*/
  Texture::Texture( const char *filename ){
    Magick::Image *img;    
    img = new Magick::Image( filename );
    // ick- just index correctly.
    img->flip();
    _rows = img->baseRows( );
    _cols = img->baseColumns( );
    _format = GL_RGB;
    _type = GL_UNSIGNED_BYTE;
    _target = GL_TEXTURE_2D;
    _image = new GLubyte[3 * _rows * _cols];
    // GraphicsMagick is column major order
    // OpenGL is column major order too?
    // C is row major order
    for( size_t i = 0; i < _cols; i++ ){
      for( size_t j = 0; j < _rows; j++ ){
        size_t k = 0;
        Magick::ColorRGB c = img->pixelColor(i, j);
        _image[3*(_rows*j+i)+k] = lrint(c.red( ) * 255.0);
        k++;
        _image[3*(_rows*j+i)+k] = lrint(c.green( ) * 255.0);
        k++;
        _image[3*(_rows*j+i)+k] = lrint(c.blue( ) * 255.0);
      }
    }
    delete img;
    glGenTextures( 1, &_name );
    msglError( );
  }
  Texture::~Texture( ){
    if( _image != NULL ){
      delete _image;
    }
    glDeleteTextures(1, &_name);
  }
  bool Texture::bind( ){
    glBindTexture( _target, _name );
    return( msglError( ) );
  }
  bool Texture::assignToTextureUnit( GLuint unit ){
    glActiveTexture(GL_TEXTURE0 + unit);
    return( bind( ) );
  }
  bool Texture::parameters( GLenum pname, GLfloat param ){
    glTexParameteri( _target, pname, param );
    return( msglError( ) );
  }
  bool Texture::parameters( GLenum pname, GLint param ){
    glTexParameterf( _target, pname, param );
    return( msglError( ) );
  }
  bool Texture::environmentParameters( GLint param ){
    // Specifies  a  single  symbolic  constant,  one  of GL_MODULATE, GL_DECAL, GL_BLEND, or GL_REPLACE.
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param );
    return( msglError( ) );
  }
  bool Texture::environmentParameters( GLfloat param ){
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param );
    return( msglError( ) );
  }
  bool Texture::load( ){
    glTexImage2D( _target, 0, _format, _rows, _cols, 0, _format, _type, _image);
    return( msglError( ) );
  }
};