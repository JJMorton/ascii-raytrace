#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "logger.h"
#include "triangle.h"

#define NO_INTERSECTION -1.0

// The characters used as different light levels in rendering
const size_t numshades = 13;
const char shades[] = { '.', ',', '-', '~', ':', ';', '!', '>', '?', '=', '*', '#', '$', '@' };

Triangle triangle_create(Vec3 p1, Vec3 p2, Vec3 p3)
{
	Triangle t =
	{
		.origin = p1,
		.v1 = vec3_sub(p2, p1),
		.v2 = vec3_sub(p3, p1)
	};
	return t;
}

Triangle triangle_rotate(Triangle t, Vec3 pivot, double thetax, double thetay, double thetaz)
{
	// Rotate the triangle's origin point around the pivot
	Vec3 delta = vec3_sub(t.origin, pivot);
	t.origin = vec3_add(pivot, vec3_rotate(delta, thetax, thetay, thetaz));

	// Rotate the triangle's other two points around its origin point
	t.v1 = vec3_rotate(t.v1, thetax, thetay, thetaz);
	t.v2 = vec3_rotate(t.v2, thetax, thetay, thetaz);

	return t;
}

double triangle_intersectionWithRay(Triangle t, Vec3 origin, Vec3 direction, Vec3 *intersection)
{
	// Vector normal to the surface of the triangle (not normalised)
	Vec3 normal = vec3_cross(t.v1, t.v2);
	double normalDot = vec3_dot(normal, direction);
	// If the line is parallel to the triangle's plane, they don't intersect
	if (normalDot == 0) return NO_INTERSECTION;

	// The coefficient of `direction` required to meet the triangle's plane exactly
	// This comes from plugging the line into the equation for the triangle's plane
	double coeff = vec3_dot(normal, vec3_sub(t.origin, origin)) / normalDot;
	// The line points away from the triangle's plane, does not intersect with it
	if (coeff <= 0) return NO_INTERSECTION;

	// Now impose the bounds of the triangle itself
	Vec3 intersect = vec3_add(origin, vec3_scale(direction, coeff));
	Vec3 p1 = vec3_add(t.origin, t.v1);
	Vec3 p2 = vec3_add(t.origin, t.v2);
	// The intersect point is inside the triangle if the signed area (determinant) of every
	// new triangle formed with the intersect and two of the triangle's points
	// has the same sign
	// This is because the sign of the determinant corresponds with the direction taken
	// around the triangle
	bool b1 = vec3_determinant(intersect, t.origin, p1) >= 0;
	bool b2 = vec3_determinant(intersect, p1, p2) >= 0;
	bool b3 = vec3_determinant(intersect, p2, t.origin) >= 0;
	if (b1 == b2 && b2 == b3)
	{
		*intersection = intersect;
		return coeff;
	}
	else
	{
		return NO_INTERSECTION;
	}
}

void triangle_renderAll(size_t trianglesc, Triangle triangles[], int screensize, double fov, double cameradistance)
{
	double scale = tan(fov * 0.5);
	Vec3 light = { 1, 0, 0 };
	Vec3 ray_origin = { 0, -cameradistance, 0 };
	for (int z = 0; z < screensize; z++)
	{
		for (int x = 0; x < screensize; x++)
		{
			// Doesn't matter that the ray isn't normalised as long at it's the same for every triangle
			Vec3 ray_direction =
			{
				(2 * (double) x / screensize - 1) * scale,
				1,
				(1 - 2 * (double) z / screensize) * scale
			};

			double minCoeff = DBL_MAX;
			char c = ' ';

			for (int i = 0; i < trianglesc; i++)
			{
				// Find if the ray intersects with the triangle
				Vec3 intersection;
				double coeff = triangle_intersectionWithRay(triangles[i], ray_origin, ray_direction, &intersection);
				if (coeff == NO_INTERSECTION || coeff >= minCoeff) continue;
				minCoeff = coeff;

				// The ray hit the triangle, now calculate the lighting
				Vec3 delta = vec3_normalise(vec3_sub(intersection, light));
				Vec3 normal = vec3_normalise(vec3_cross(triangles[i].v1, triangles[i].v2));
				double intensity = fabs(vec3_dot(delta, normal));
				c = shades[(int) (intensity * numshades)];
			}

			putchar(c); putchar(' ');
		}
		putchar('\n');
	}
}

