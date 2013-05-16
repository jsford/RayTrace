/*
    This file belongs to the Ray tracing tutorial of http://www.codermind.com/
    It is free to use for educational purpose and cannot be redistributed
    outside of the tutorial pages.
    Any further inquiry :
    mailto:info@codermind.com
 */

#ifndef __RAYTRACE_H
#define __RAYTRACE_H

#include "Def.h"

struct material {
    color diffuse;
    float reflection;
    color specular;
    float power;
};

struct sphere {
	point pos;
	float size;
	int materialId;
};

struct light {
	point pos;
	color intensity;
};

struct ray {
	point start;
	vecteur dir;
};


#define invsqrtf(x) (1.0f / sqrtf(x))

#endif // __RAYTRACE_H
