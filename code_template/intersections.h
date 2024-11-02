#ifndef __INTERSECTIONS__
#define __INTERSECTIONS__

#include "myvectors.h"
#include "ray.h"


float mini(float a, float b);
float discriminant(float a, float b, float c);
//bool rayTriangleIntersect(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Ray &ray, float &t, float &u, float &v);
bool raySphereIntersect(const Vec3f &center, float radius, const Ray &ray, float &t);
bool rayTriangleIntersect(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Ray &ray, float &t);



#endif