#include "intersections.h"

float mini(float a, float b)
{
    return a < b ? a : b;
}

float discriminant(float a, float b, float c)
{
    return b*b - 4*a*c;
}

bool raySphereIntersect(const Vec3f &center, float radius, const Ray &ray, float &t)
{
    //(x – cx)^2 + (y – cy)^2 + (z – cz)^2 – R^2 = 0


    float a = ray.direction * ray.direction; //dot product
    float b = -2 * (ray.direction * (ray.origin - center));
    float c = (ray.origin - center) * (ray.origin - center) - radius * radius;

    //we will compute t for shading etc
    float D = discriminant(a, b, c);
    if (D < 0)
    {
        return false;
    }      
    float t0 = (-b - sqrt(D)) / (2*a);
    float t1 = (-b + sqrt(D)) / (2*a);

     //t has to be pozitive and the smallest one.

    if (t0 > 0 && t1 > 0) {
        t = mini(t0, t1); 
        return true;
    } 
    else if (t0 > 0) {
        t = t0; 
        return true;
    } 
    else if (t1 > 0) {
        t = t1;
        return true; 
    }
    else {
        return false;
    }

}

