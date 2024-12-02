#ifndef __INTERSECTIONS__
#define __INTERSECTIONS__

#include "myvectors.h"
#include "ray.h"
#include "parser.h"

float minOfThree(float a, float b, float c);

float mini(float a, float b);

float discriminant(float a, float b, float c);

bool raySphereIntersect(const Vec3f &center, float radius, const Ray &ray, float &t);

bool rayTriangleIntersect(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Ray &ray, float &t);

bool anyIntersection(float &t1, float &t2, float &t3,float &mint, parser::Scene scene, const Ray &r, int &index, float epsilon);

bool isThereObjectInBetween(Vec3f lightPosition,Vec3f intersection, Vec3f  shadowPoint, parser::Scene scene, float epsilon);

Vec3f sphereNormal(Vec3f intersectionPoint, Vec3f sphereCenter, float epsilon);

Vec3f triangleNormal(Vec3f a, Vec3f b, Vec3f c);

Vec3f findRadiance( parser::PointLight pointLight,  Vec3f intersectionPoint);

Vec3f findDiffuse( Vec3f N,  Vec3f Wi, Vec3f diffuseReflectance, Vec3f radiance);

Vec3f findSpecular( Vec3f N,  Vec3f Wi, Vec3f R, Vec3f specularReflectance, Vec3f radience, float phongExponent);

#endif