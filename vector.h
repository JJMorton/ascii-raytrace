#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
	double x, y, z;
} Vec3;

double vec3_magnitude(Vec3 v);
double vec3_dot(Vec3 a, Vec3 b);
double vec3_determinant(Vec3 a, Vec3 b, Vec3 c);
Vec3   vec3_rotateX(Vec3 v, double theta);
Vec3   vec3_rotateY(Vec3 v, double theta);
Vec3   vec3_rotateZ(Vec3 v, double theta);
Vec3   vec3_rotate(Vec3 v, double thetax, double thetay, double thetaz);
Vec3   vec3_add(Vec3 a, Vec3 b);
Vec3   vec3_sub(Vec3 a, Vec3 b);
Vec3   vec3_scale(Vec3 a, double factor);
Vec3   vec3_cross(Vec3 a, Vec3 b);
Vec3   vec3_normalise(Vec3 v);

#endif // VECTOR_H

