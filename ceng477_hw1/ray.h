#ifndef __RAY_H__
#define __RAY_H__

#include "myvectors.h"

struct Ray
{
    Vec3f origin;
    Vec3f direction;

    Ray() {}
    Ray(Vec3f origin, Vec3f direction)
    {
        this->origin = origin;
        this->direction = direction;
    }
    const Vec3f& getOrigin() const
    {
        return origin;
    }
    const Vec3f& getDirection() const
    {
        return direction;
    }
    Ray operator=(const Ray &rhs)
    {
        this->origin = rhs.origin;
        this->direction = rhs.direction;
        return *this;
    }
    Vec3f pointIsAt(float t) const
    {
        return origin + direction * t;
    }
    
   
    

};




#endif