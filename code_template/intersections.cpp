#include "intersections.h"
#include "parser.h"

float kEpsilon = 1e-5;

float mini(float a, float b)
{
    return a < b ? a : b;
}

float minOfThree(float a, float b, float c) {
    return std::min(a, std::min(b, c));
}

float discriminant(float a, float b, float c)
{
    return b*b - 4*a*c;
}

bool raySphereIntersect(const Vec3f &center, float radius, const Ray &ray, float &t)
{
    //(x – cx)^2 + (y – cy)^2 + (z – cz)^2 – R^2 = 0


    float a = ray.direction * ray.direction; //dot product
    float b = -2 * (ray.direction * (center  - ray.origin));
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

Ray sphereNormal(Vec3f intersectionPoint, Vec3f sphereCenter){
    return Ray(intersectionPoint, (intersectionPoint - sphereCenter).Normalize());
}

Ray triangleNormal(Vec3f a, Vec3f b, Vec3f c){
    return Ray(a, (b - a).CrossProduct(c - a).Normalize());
}


bool anyIntersection(bool &flag1, bool &flag2, bool &flag3, float &t1, float &t2, float &t3,float &mint, parser::Scene scene, const Ray &r) {


                for(int i = 0; i < scene.triangles.size() ; i++ ){
                    
                    parser::Face f;
                    f = scene.triangles[i].indices;
                    int index_a = f.v0_id;
                    int index_b = f.v1_id;
                    int index_c = f.v2_id;
                    //printf("a:%d,b:%d,c:%d\n",index_a,index_b,index_c);
                    Vec3f& a = scene.vertex_data[index_a-1];
                    Vec3f& b =scene.vertex_data[index_b-1];
                    Vec3f& c = scene.vertex_data[index_c-1];
                    if(rayTriangleIntersect(a,b,c,r,t1)){
                        flag1 = true;
                    }     
                }
                for(int i = 0; i < scene.spheres.size() ; i++ ){
                    
                    parser::Sphere f;
                    f = scene.spheres[i];
                    int material_id = f.material_id;
                    int center_vertex_id = f.center_vertex_id;
                    float radius = f.radius;
                    //printf("a:%d,b:%d,c:%d\n",index_a,index_b,index_c);
                    Vec3f& center = scene.vertex_data[center_vertex_id-1];
                    
                    if(raySphereIntersect(center,radius,r,t2)){
                        flag2 = true;
                        
                    }     
                }
                for(int i = 0; i < scene.meshes.size() ; i++ ){
                    
                    parser::Mesh f;
                    f = scene.meshes[i];
                    int material_id = f.material_id;
                    for(int boncuk = 0 ; boncuk < f.faces.size(); boncuk++)
                    {

                        parser::Face tri;
                        tri = f.faces[boncuk];
                        int index_a = tri.v0_id;
                        int index_b = tri.v1_id;
                        int index_c = tri.v2_id;
      
                        Vec3f& a = scene.vertex_data[index_a-1];
                        Vec3f& b = scene.vertex_data[index_b-1];
                        Vec3f& c = scene.vertex_data[index_c-1];
                        if(rayTriangleIntersect(a,b,c,r,t3)){
                            flag3 = true;
                        }     
                    }
                }
                if(flag1 || flag2 || flag3 == false){
                        return false;
                }

    return true;

}


bool isThereObjectInBetween(Vec3f lightPosition, Vec3f lightIntensity , Vec3f  intersection, parser::Scene scene) {

    Ray lightRay(lightPosition, (intersection - lightPosition).Normalize());
    float maxDistance = (intersection - lightPosition).Length();

                bool flag1 = false;
                bool flag2 = false;
                bool flag3 = false;
                float t1 = INFINITY;
                float t2 = INFINITY;
                float t3 = INFINITY;
                float minT = INFINITY;

                bool intersectionExists = anyIntersection(flag1, flag2, flag3, t1, t2, t3, minT, scene, lightRay);

                if(intersectionExists){
                    if(lightRay.pointIsAt(minT).Length() < maxDistance){
                        return true;
                    }
                }
 return false;
}
