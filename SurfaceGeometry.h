/*
 * Your contact info here
 *
 * $Id: SurfaceGeometry.h 3305 2011-11-16 17:58:30Z mshafae $
 *
 */


#ifndef _SURFACEGEOMETRY_H_
#define _SURFACEGEOMETRY_H_

#include "SurfaceGeometryFaceList.h"

#define kSurfaces 6
#define kColorSchemes 21

enum {
	kCube = 0,
	kTranguloidTrefoil,
	kTriaxialTritorus,
	kStilettoSurface,
	kSlippersSurface,
	kMaedersOwl
};

void GetStrings( unsigned int surface, char ** strName, char ** strAuthor,
                 char ** strX, char ** strY, char ** strZ, char ** strRange );

SurfaceGeometryFaceList* BuildGeometry( unsigned int surface,
                                        unsigned int colorScheme,
												                unsigned int subdivisions,
												                unsigned int xyRatio );



#endif
