/*
 * Your contact info here
 *
 * $Id: misc.cpp 3305 2011-11-16 17:58:30Z mshafae $
 *
 * Utility functions
 *
 */

#include <cmath>
#include "misc.h"


// c = a - b
void vecDifference( float a[3], float b[3], float c[3] ){
  for( int i = 0; i < 3; i++ ){
    c[i] = a[i] - b[i];
  }
}

// c = a X b
void crossProduct( float a[3], float b[3], float c[3] ){
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];  
}

void vecNormalize( float n[3] ){
  float norm;
  norm = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
  if( norm != 0 ){
    n[0] = n[0] / norm;
    n[1] = n[1] / norm;
    n[2] = n[2] / norm;
  }
}

void material( int mode ){
  // See
  //<http://www.opengl.org/resources/code/samples/sig99/advanced99/notes/node153.html>
  // and
  // <http://devernay.free.fr/cours/opengl/materials.html>
  float ambient[3];
  float diffuse[3];
  float specular[3];
  float shininess[1];
  switch( mode ){
    case 0:
    // ruby
    ambient[0] = 0.1745;
    ambient[1] = 0.01175;
    ambient[2] = 0.01175;
    diffuse[0] = 0.61424;
    diffuse[1] = 0.04136;
    diffuse[2] = 0.04136;
    specular[0] = 0.727811;
    specular[1] = 0.626959;
    specular[2] = 0.626959;
    shininess[0] = 0.6;
    glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, shininess );
    break;
    case 1:
    // chrome
    ambient[0] = 0.25;
    ambient[1] = 0.25;
    ambient[2] = 0.25;
    diffuse[0] = 0.4;
    diffuse[1] = 0.4;
    diffuse[2] = 0.4;
    specular[0] = 0.774597;
    specular[1] = 0.774597;
    specular[2] = 0.774597;
    shininess[0] = 0.6;
    glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, shininess );
    break;
    case 2:
    // green plastic
    ambient[0] = 0.0;
    ambient[1] = 0.0;
    ambient[2] = 0.0;
    diffuse[0] = 0.1;
    diffuse[1] = 0.35;
    diffuse[2] = 0.1;
    specular[0] = 0.45;
    specular[1] = 0.55;
    specular[2] = 0.45;
    shininess[0] = 0.25;
    glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, shininess );
    break;
    default:
    fprintf( stderr, "Unknown mode. Exiting\n" );
    break;
  }
}
