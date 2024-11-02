#include "intersections.h"

constexpr float kEpsilon = 1e-2;

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
    // p = o + dt
    // (p-c)^2 - R^2
    // ((o + dt) - c)^2 - R^2 
    // (o^2 + 2.dt.o + dt^2) - 2((o+dt).c) + c^2 - R^2  
    //  o^2 + 2.dt.o + dt^2 - 2.o.c - 2.dt.c + c^2 - R^2 
    //  dt^2 + 2.dt.(o - c) + o^2 - 2.o.c + c^2 - R^2 


    float a = ray.direction * ray.direction; // dt^2 (dt . dt) , dot product
    // edd float b = - 2 * (ray.direction * (ray.origin - center));
    float b = 2 * (ray.direction * (ray.origin - center)); // 2.dt.(o - c)
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



// bool rayTriangleIntersect(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Ray &ray, float &t){
//     // 𝒏=(𝒄 −𝒃)  x (𝐚 −𝐛)
//     // 𝑓(𝒑)=(𝒑−𝒂).𝒏=0
//     // p = ray(o + td)
//     // ab
//     // bc
//     // ca
//     Vec3f edge_ab = b - a;
//     Vec3f edge_bc = c - b;
//     Vec3f edge_ac = c - a ;
//     Vec3f triangleNormal = edge_ab.CrossProduct(edge_ac);
//     float triangleNormal_dot_raydir = triangleNormal * ray.direction;

//     if (triangleNormal_dot_raydir < kEpsilon)  // if det is smaller than 0 then it is back facing
//         return false;

//     if(fabs(triangleNormal_dot_raydir) < kEpsilon ) // if 
//         return false;

//     float d = edge_ab.Negate() * triangleNormal;

//     t = -(triangleNormal * ray.origin + d) / triangleNormal_dot_raydir;

//     if (t < 0) 
//         return false;

//     Vec3f rayPoint = ray.pointIsAt(t);

//         //if 0, it is on the triangle plane, means it intersects with the
//         // TRIANGLE'S PLANE, not means it intersects with the triangle, additional check below.
//     Vec3f edge_abp = rayPoint - edge_ab;
//     Vec3f edge_bcp = rayPoint - edge_bc;
//     Vec3f edge_ca = c - a;
//     Vec3f edge_cap = rayPoint - edge_ca;
//     if (triangleNormal*(edge_ab.CrossProduct(edge_abp)) > 0 &&
//         triangleNormal*(edge_bc.CrossProduct(edge_bcp)) > 0 &&
//         triangleNormal*(edge_ca.CrossProduct(edge_cap)) > 0 ){
//             return true;
//         }
//     return false;
//     /*Check for the direction of the plane and the normal.*/



// }
bool rayTriangleIntersect(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Ray &ray, float &t) {
    Vec3f edge_ab = b - a;
    Vec3f edge_ac = c - a;
    
    // Calculate the normal of the triangle plane
    Vec3f triangleNormal = edge_ab.CrossProduct(edge_ac);
    
    // Calculate dot product of triangle normal and ray direction
    float triangleNormal_dot_raydir = triangleNormal * ray.direction;
    
    // Check if ray is parallel to the triangle's plane
    if (std::fabs(triangleNormal_dot_raydir) < kEpsilon) 
        return false; // No intersection if parallel

    // Compute the plane offset `d`
    float d = -(triangleNormal * a); 

    // Calculate the intersection distance `t`
    t = -(triangleNormal * ray.origin + d) / triangleNormal_dot_raydir;

    // If `t` is negative, the triangle is behind the ray origin
    if (t < 0) 
        return false;

    // Compute intersection point on the plane
    Vec3f rayPoint = ray.pointIsAt(t);

    // Inside-Outside Test with relaxed precision for edge cases
    Vec3f edge_abp = rayPoint - a;
    if ((triangleNormal * edge_ab.CrossProduct(edge_abp)) < -kEpsilon)
        return false;

    Vec3f edge_bc = c - b;
    Vec3f edge_bcp = rayPoint - b;
    if ((triangleNormal * edge_bc.CrossProduct(edge_bcp)) < -kEpsilon)
        return false;

    Vec3f edge_ca = a - c;
    Vec3f edge_cap = rayPoint - c;
    if ((triangleNormal * edge_ca.CrossProduct(edge_cap)) < -kEpsilon)
        return false;

    // If all tests pass, we have a valid intersection inside or on the triangle boundary
    return true;
}