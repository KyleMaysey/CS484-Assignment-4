/*
 * Copyright (c) 2005-2008 Michael Shafae.
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: GLSLShader.h 3305 2011-11-16 17:58:30Z mshafae $
 *
 * Utility functions for loading a shader
 *
 */

#ifndef _GLSLSHADER_H_
#define _GLSLSHADER_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
#include "msgfxerror.h"

char* file2strings( const char *filename ){
  long fileLength, numberOfCharsRead;
  char *strings;
  FILE *fhandle = fopen( filename, "r" );
  if( !fhandle ){
    fprintf( stderr, "%s: Can't open file %s for reading\n", __FUNCTION__, filename );
    return NULL;
  }
  fseek( fhandle, 0, SEEK_END );
  fileLength = ftell( fhandle );

  fseek( fhandle, 0, SEEK_SET );

  if( !(strings = (char*)calloc( (fileLength+1), sizeof(char) )) ){
    fprintf( stderr, "%s: Can't malloc space for string buffer %s\n", __FUNCTION__, filename );
    return NULL;
  }

  numberOfCharsRead = (long)fread( strings, sizeof(char), fileLength, fhandle );

  strings[numberOfCharsRead] = '\0';

  fclose( fhandle );

  return( strings );
}

namespace msgfx{
class Shader{
public:
  GLuint _object;
  
  Shader( ){ };

  ~Shader( ){
    glDeleteShader( _object );
    msglError( );
  }

  bool compileShader( const GLchar *src ){
    GLint compiled_ok;
    char *msg;
    GLint length = (GLint)strlen(src);
    glShaderSource( _object, 1, &src, &length );
    glCompileShader( _object );
    msglError( );
    glGetShaderiv( _object, GL_COMPILE_STATUS, &compiled_ok );
    msglError( );
    if( !compiled_ok ){
      fprintf( stderr, "Compilation failed for shader \"%s\" of length %d\n", src, length );
      msg = getInfoLog( );
      fprintf( stderr, "Info Log:\n%s\n", msg );
      free( msg );
    }
    return( (bool)compiled_ok );
  }
  
  char* getInfoLog( ){
    GLint info_log_length;
    glGetShaderiv( _object, GL_INFO_LOG_LENGTH, &info_log_length );
    char *info_log = (char*)malloc( info_log_length );
    glGetShaderInfoLog( _object, info_log_length, NULL, info_log);
    return( info_log );
  }
};

class VertexShader : public Shader{

public:
  VertexShader( const char *srcFileName ){
    char *src = file2strings( srcFileName );
    if( src != NULL ){ // should throw an exception
      if( (Shader::_object = glCreateShader( GL_VERTEX_SHADER )) == 0 ){
        fprintf( stderr, "Can't generate vertex shader name\n" );
      }
      msglError( );
      compileShader( src );
      msglError( );
      free( src );
    }
  }

  GLuint object( ){
    return Shader::_object;
  }

};


class FragmentShader : public Shader{

public:
  FragmentShader( const char *srcFileName ){
    char *src = file2strings( srcFileName );
    if( src != NULL ){
      if( (Shader::_object = glCreateShader( GL_FRAGMENT_SHADER )) == 0 ){
        fprintf( stderr, "Can't generate fragment shader name\n" );
        exit(1);
      }
      msglError( );
      compileShader( src );
      msglError( );
      free( src );
    }
  }

    GLuint object( ){
      return Shader::_object;
    }
};


class GLSLProgram{

public:
  GLuint _object;

  GLSLProgram( ){
    if( (_object = glCreateProgram( )) == 0 ){
      fprintf( stderr, "Can't generate program name\n" );
      exit(1);
    }
  }

  ~GLSLProgram( ){
    detachAll( );
    glDeleteProgram( _object );
  }

  bool attach( FragmentShader &fs ){
    glAttachShader( _object, fs.object( ) );
    return( !msglError( ) );
  }

  bool attach( VertexShader &vs ){
    glAttachShader( _object, vs.object( ) );
    return( !msglError( ) );
  }

  bool detachAll( ){
    bool ret = false;
    GLsizei const maxCount = 32;
    GLsizei count;
    GLuint shaders[32];
    glGetAttachedShaders( _object, maxCount, &count, shaders );
    for(int i = 0; i < count; i++ ){
      if( !(detach( shaders[i] )) ){
        ret = true;
      }
    }
    return( ret );
  }

  bool detach( GLuint shaderName ){
    glDetachShader( _object, shaderName );
    return( !msglError( ) );
  }

  bool detach( FragmentShader &fs ){
    glDetachShader( _object, fs.object( ) );
    return( !msglError( ) );
  }

  bool detach( VertexShader &vs ){
    glDetachShader( _object, vs.object( ) );
    return( !msglError( ) );
  }

  bool link( ){
    GLint linked_ok;
    char *msg;
    bool ret = true;
  
    glLinkProgram( _object );

    glGetProgramiv( _object, GL_LINK_STATUS, &linked_ok );
    if( !linked_ok ){
      ret = false;
      fprintf( stderr, "Linking failed.\n" );
      msg = getInfoLog( );
      fprintf( stderr, "%s\n", msg );
      free(msg );
    }
    return ret;
  }
  
  char* getInfoLog( ){
    GLint info_log_length;
    glGetProgramiv( _object, GL_INFO_LOG_LENGTH, &info_log_length );
    char *info_log = (char*)malloc( info_log_length );
    glGetProgramInfoLog( _object, info_log_length, NULL, info_log);
    return( info_log );
  }
  
  bool activate( ){
    glUseProgram( _object );
    return( !msglError( ) );
  }
  
  int getUniformLocation( const char* uniformName ){
    int uniformLocation = glGetUniformLocation(_object, uniformName);
    if( uniformLocation == -1 ){
      fprintf( stderr, "Warning: No such uniform named \"%s\"\n", uniformName );
    }
    return( uniformLocation );
  }

  bool setUniformLocation( const char* uniformName, float v0, float v1, float v2, float v3 ){
    int uniformLocation = glGetUniformLocation(_object, uniformName);
    if( uniformLocation == -1 ){
      fprintf( stderr, "Warning: No such uniform named \"%s\"\n", uniformName );
    }
    glUniform4f( uniformLocation, v0, v1, v2, v3 );
    return( !msglError( ) );
  }

  bool setUniformLocation( const char* uniformName, float v0, float v1, float v2 ){
    int uniformLocation = glGetUniformLocation(_object, uniformName);
    if( uniformLocation == -1 ){
      fprintf( stderr, "Warning: No such uniform named \"%s\"\n", uniformName );
    }
    glUniform3f( uniformLocation, v0, v1, v2 );
    return( !msglError( ) );
  }
  
  bool setUniformLocation( const char* uniformName, int v0 ){
    int uniformLocation = glGetUniformLocation(_object, uniformName);
    if( uniformLocation == -1 ){
      fprintf( stderr, "Warning: No such uniform named \"%s\"\n", uniformName );
    }
    glUniform1i( uniformLocation, v0 );
    return( !msglError( ) );
  }
  

  bool isHardwareAccelerated( ){
    bool ret = true;
     #if defined(__APPLE__)
     /*
      * This checks to make sure that the shaders are going to
      * be hardware accelerated.
      * If they aren't, a message is printed.
      *
      * Something similar can be done on Unix as well as MS Windows but
      * it is left up to the student to look up the neccessary API calls.
      */
    GLint hardwareAccelerated;

    CGLGetParameter( CGLGetCurrentContext( ), kCGLCPGPUVertexProcessing, &hardwareAccelerated );
    if( !hardwareAccelerated ){
      ret = false;
      fprintf( stderr,
        "Warning: Vertex shader is NOT being hardware-accelerated\n" );
    }
    CGLGetParameter( CGLGetCurrentContext( ), kCGLCPGPUFragmentProcessing, &hardwareAccelerated );
    if( !hardwareAccelerated ){
      ret = false;
      fprintf( stderr,
        "Warning: Fragment shader is NOT being hardware-accelerated\n" );
    }
     #endif
    return ret;
  }
};
};

#endif
