#include "intersections.h"

constexpr float kEpsilon = 1e-5;

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

    // (o^2 + 2.dt.o + (dt)^2) - 2((o+dt).c) + c^2 - R^2  
    //  o^2 + 2.dt.o + (dt)^2 - 2.o.c - 2.dt.c + c^2 - R^2 
    //  (dt)^2 + 2.dt.(o - c) + o^2 - 2.o.c + c^2 - R^2 
    // 
    //  Ax^2 + Bx + C  = f(x)
    // At^2 + Bt + C = f(t)

    float a = ray.direction * ray.direction; // (dt)^2 (dt . dt) , dot product
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


// //more simple approach
// bool rayTriangleIntersect(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Ray &ray, float &t){
//     // 𝒏=(𝒄 −𝒃)  x (𝐚 −𝐛)
//     // 𝑓(𝒑)=(𝒑−𝒂).𝒏=0
//     // p = ray(o + td)
//     // ab
//     // bc
//     // ca
//     Vec3f edge_ab = b - a; //v0v1
//     Vec3f edge_ac = c - a; //v0v2

//     Vec3f triangleNormal = edge_ab.CrossProduct(edge_ac);
    
//     float triangleNormal_dot_raydir = triangleNormal * ray.direction;

//     if (std::fabs(triangleNormal_dot_raydir)< kEpsilon)  // if det is smaller than 0 then it is back facing
//         return false;

//     float d = -(triangleNormal * a);

//     t = -(triangleNormal * ray.origin + d); //   / triangleNormal_dot_raydir;

//     if (t < 0) 
//         return false;

//     Vec3f rayPoint = ray.pointIsAt(t);

//         //if 0, it is on the triaedge_abngle plane, means it intersects with the
//         // TRIANGLE'S PLANE, not means it intersects with the triangle, additional check below.
    
//     Vec3f Ne;
//     Vec3f edge_ap = rayPoint - a; //v0p
//     Ne = edge_ab.CrossProduct(edge_ap);
//     if (triangleNormal*(Ne) < 0)
//         return false;

//     Vec3f edge_bc = c - b; // v2v1
//     Vec3f edge_bp = rayPoint - b; //v1p
//     Ne = edge_bc.CrossProduct(edge_bp);
//     if (triangleNormal*(Ne) < 0)
//         return false;
    
//     Vec3f edge_ca = a - c;        //v2v0
//     Vec3f edge_cp = rayPoint - c; //v2p
//     Ne = edge_ca.CrossProduct(edge_cp);
//     if (triangleNormal*(Ne) < 0)
//         return false;
        
//     return true;
//     /*Check for the direction of the plane and the normal.*/
// }




 //baricentric
 
bool rayTriangleIntersect(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Ray &ray, float &t) {
    Vec3f edge_ab = b - a;
    Vec3f edge_ac = c - a;
    
    // Compute the triangle's normal
    Vec3f triangleNormal = edge_ab.CrossProduct(edge_ac).Normalize();

    // Check if ray is parallel to the triangle's plane
    float triangleNormal_dot_raydir = triangleNormal * ray.direction;

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

    // Calculate vectors for barycentric coordinates
    Vec3f edge_bc = c - b;
    Vec3f edge_ca = a - c;
    Vec3f edge_ap = rayPoint - a;
    
    Vec3f edge_bp = rayPoint - b;
    Vec3f edge_cp = rayPoint - c;

    // Compute barycentric coordinates
    float area_abc = (triangleNormal * edge_ab.CrossProduct(edge_ac)); // Full triangle area
    float u = triangleNormal * edge_bc.CrossProduct(edge_bp) / area_abc; // PBC area / ABC area
    float v = triangleNormal * edge_ca.CrossProduct(edge_cp) / area_abc; // PCA area / ABC area
    float w = 1.0f - u - v; // Remaining coordinate

    // If all barycentric coordinates are within [0, 1], the point is inside or on the triangle
    return (u >= -kEpsilon) && (v >= -kEpsilon) && (w >= -kEpsilon);
}