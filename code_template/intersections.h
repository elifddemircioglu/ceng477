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

bool anyIntersection(float &t1, float &t2, float &t3,float &mint, parser::Scene scene, const Ray &r, int &index);

bool isThereObjectInBetween(Vec3f lightPosition, Vec3f  shadowPoint, parser::Scene scene);

Vec3f pointShade(Vec3f intersectionPoint, Vec3f normal, Vec3f viewDirection, parser::Material material, parser::Scene scene, bool inShadow);


Vec3f sphereNormal(Vec3f intersectionPoint, Vec3f sphereCenter);

Vec3f triangleNormal(Vec3f a, Vec3f b, Vec3f c);



Vec3f calculateDiffuse( Vec3f N,  Vec3f L,  Vec3f lightIntensity,  Vec3f surfaceColor, float distance);

Vec3f calculateSpecular(Vec3f N,  Vec3f L, Vec3f V, 
                        Vec3f lightIntensity, Vec3f specularReflectance, 
                        float phongExponent);
#endif