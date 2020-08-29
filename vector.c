#include <math.h>

#include "vector.h"

Vec3 vec3_rotateX(Vec3 v, double theta)
{
	double c = cos(theta);
	double s = sin(theta);
	Vec3 out =
	{
		v.x,
		(c + s) * v.y,
		(c - s) * v.z
	};
	return out;
}

Vec3 vec3_rotateY(Vec3 v, double theta)
{
	double c = cos(theta);
	double s = sin(theta);
	Vec3 out =
	{
		(c - s) * v.x,
		v.y,
		(s + c) * v.z
	};
	return out;
}

Vec3 vec3_rotateZ(Vec3 v, double theta)
{
	double c = cos(theta);
	double s = sin(theta);
	Vec3 out =
	{
		(c + s) * v.x,
		(c - s) * v.y,
		v.z
	};
	return out;
}

Vec3 vec3_rotate(Vec3 v, double thetax, double thetay, double thetaz)
{

	// https://en.wikipedia.org/wiki/Rotation_matrix#General_rotations
	double cx = cos(thetax);
	double sx = sin(thetax);
	double cy = cos(thetay);
	double sy = sin(thetay);
	double cz = cos(thetaz);
	double sz = sin(thetaz);
	Vec3 out =
	{
		(cz*cy) * v.x + (cz*sy*sx - sz*cy) * v.y + (cz*sy*cx + sz*sx) * v.z,
		(sz*cy) * v.x + (sz*sy*sx + cz*cx) * v.y + (sz*sy*cx - cz*sx) * v.z,
		-sy     * v.x + cy*sx              * v.y + cy*cx              * v.z
	};
	return out;

}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
	Vec3 out = { a.x + b.x, a.y + b.y, a.z + b.z };
	return out;
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
	Vec3 out = { a.x - b.x, a.y - b.y, a.z - b.z };
	return out;
}

Vec3 vec3_scale(Vec3 a, double factor)
{
	Vec3 out = { a.x * factor, a.y * factor, a.z * factor };
	return out;
}

double vec3_magnitude(Vec3 v)
{
	return sqrt(vec3_dot(v, v));
}

double vec3_dot(Vec3 a, Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_cross(Vec3 a, Vec3 b)
{
	Vec3 out = {
		a.y * b.z - a.z * b.y,
		-(a.x * b.z - a.z * b.x),
		a.x * b.y - a.y * b.x
	};
	return out;
}

double vec3_determinant(Vec3 a, Vec3 b, Vec3 c)
{
	return vec3_dot(a, vec3_cross(b, c));
}

Vec3 vec3_normalise(Vec3 v)
{
	double mag = vec3_magnitude(v);
	/* if (mag == 0) return vec3_scale(v, 0); */
	return vec3_scale(v, 1.0 / mag);
}

