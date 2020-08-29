#ifndef RENDERER_H
#define RENDERER_H

#include "vector.h"

typedef struct
{
	Vec3 origin, v1, v2;
} Triangle;

Triangle triangle_create(Vec3 p1, Vec3 p2, Vec3 p3);
Triangle triangle_rotate(Triangle t, Vec3 pivot, double thetax, double thetay, double thetaz);
double   triangle_intersectionWithRay(Triangle t, Vec3 origin, Vec3 direction, Vec3 *intersection);
void     triangle_renderAll(size_t trianglesc, Triangle triangles[], int screensize, double cameradistance);

#endif // RENDERER_H

