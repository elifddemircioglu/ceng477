#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "tinyxml2.h"
#include "myvectors.h"
#include "ray.h"
#include "intersections.h"

typedef unsigned char RGB[3];

int main(int argc, char* argv[])
{

    // this is just for making the user to provide a scene file.
    if (argc < 2) {
    std::cout << "please provide scene file" << std::endl;
    return 1;
    }
    
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    for(int camno = 0; camno < scene.cameras.size(); camno++) // for every camera we have to generate a pic.
    {
        const char* ppm_filename = scene.cameras[camno].image_name.c_str(); // get the name for outfile.

        //resolution of the image
        int width = scene.cameras[camno].image_width;
        int height = scene.cameras[camno].image_height;

        //this is the image data array for every rgb for every pixel on the image. basically pixel numer * 3.
        unsigned char* image = new unsigned char [width * height * 3];


        //for camera and image plane calculations (acoording to slide 02)
        Vec3f w_dir = scene.cameras[camno].gaze * -1;
        Vec3f u_dir = scene.cameras[camno].up.CrossProduct(w_dir).Normalize();
        Vec3f& v_dir = scene.cameras[camno].up;
        Vec3f& e = scene.cameras[camno].position;
        float l = scene.cameras[camno].near_plane.x;
        float r = scene.cameras[camno].near_plane.y;
        float b = scene.cameras[camno].near_plane.z;
        float t = scene.cameras[camno].near_plane.w;

        Vec3f center = e + scene.cameras[camno].gaze * scene.cameras[camno].near_distance;
        Vec3f lefttopcorner = center + v_dir * t + u_dir * l;
        int k = 0;



        for (int row = 0; row < height; ++row)
        {
            for (int column = 0; column < width; ++column)
            {
                float dist_u = ((r-l)/width)*(column+0.5);
                float dist_v = ((t-b)/height)*(row+0.5);

                Vec3f pixel = lefttopcorner + u_dir * dist_u - v_dir * dist_v;
                Vec3f direction = pixel - e;
                Vec3f direction_normalized = direction.Normalize();
                Ray r(e, direction_normalized); // bu şu an istediğimiz pixelden geçen ray eq. olmuş oluyor.

                //for every object in the scene we have to check if the ray intersects with the object.
                // triangle with barycentric coordinates ile yapmayı deneyelim 
                // sanırım şey gibi olcak, if mesh obbjesi varsa triangleintersect, if sphere varsa sphereintersect gibi. 
                //sonra da intersection pointlere shading yapıcaz. en son da color valuesini image a atayacağız.   


                float t1 = INFINITY;
                float t2 = INFINITY;
                float t3 = INFINITY;
                float minT = INFINITY;
                int index = 0;

            bool isAnyIntersection = anyIntersection(t1, t2, t3, minT, scene, r, index, scene.shadow_ray_epsilon);
                
            if(!isAnyIntersection){
                image[k] = scene.background_color.x;
                image[k+1] = scene.background_color.y;
                image[k+2] = scene.background_color.z;
            }
            else{
               // burada materialı doğru ver
               Vec3f color;
                    if(minT == t1){color = scene.ambient_light.PointWise(scene.materials[scene.triangles[index].material_id-1].ambient);} //triangle
                    else if(minT == t2){color = scene.ambient_light.PointWise(scene.materials[scene.spheres[index].material_id-1].ambient);} //sphere
                    else if(minT == t3){color = scene.ambient_light.PointWise(scene.materials[scene.meshes[index].material_id-1].ambient);} //mesh
                        
                        

                        //her point lightin renge sağladığı değerleri ayrı ayrı ekleyeceğiz.
                        for(int i = 0; i< scene.point_lights.size() ;i++ ){

                             //shading calculations

                            Vec3f lightPosition = scene.point_lights[i].position;
                            Vec3f lightIntensity = scene.point_lights[i].intensity;

                            //eğer o point lighta göre gölgedeyse sadece ambient değeri alınacak.
                            

                            if(minT == t1){
                                //bu demektir ki bi triangle ile intersect ettik en yakında.
                                //triangle normal calculate
                                Vec3f triangleNormal1 = triangleNormal(scene.vertex_data[scene.triangles[index].indices.v0_id-1],scene.vertex_data[scene.triangles[index].indices.v1_id-1],scene.vertex_data[scene.triangles[index].indices.v2_id-1]);
                                Vec3f shadowPoint = r.pointIsAt(minT) + (triangleNormal1*scene.shadow_ray_epsilon);
                                bool isInShadow = isThereObjectInBetween(lightPosition, r.pointIsAt(minT), shadowPoint, scene,scene.shadow_ray_epsilon);
                                //belki burda yön sabittir shadow pointten light deil de orijinal pointten giden yönü alabiliriz. isinsgadow için
                                if(!isInShadow){
                                    Vec3f lightDir = lightPosition - r.pointIsAt(minT);
                                    Vec3f radiance = findRadiance(scene.point_lights[i], r.pointIsAt(minT));
                                    //color += calculateDiffuse(triangleNormal1, lightDir, lightIntensity, scene.materials[scene.triangles[index].material_id-1].diffuse, (lightPosition - r.pointIsAt(minT)).Length());
                                    //color += calculateSpecular(triangleNormal1, lightDir,r.direction , lightIntensity, scene.materials[scene.triangles[index].material_id-1].specular, scene.materials[scene.triangles[index].material_id-1].phong_exponent);
                                    color += findDiffuse(triangleNormal1, lightDir, scene.materials[scene.triangles[index].material_id-1].diffuse, radiance);
                                    color += findSpecular(triangleNormal1, lightDir, r.direction, scene.materials[scene.triangles[index].material_id-1].specular, radiance, scene.materials[scene.triangles[index].material_id-1].phong_exponent);
                                }
                            }

                            else if(minT == t2){
                                //bu demektir ki bi sphere ile intersect ettik en yakında.
                                //sphere normal calculate

                                Vec3f sphereNormal1 = sphereNormal(r.pointIsAt(minT), scene.vertex_data[scene.spheres[index].center_vertex_id-1],scene.shadow_ray_epsilon);
                                Vec3f shadowPoint = r.pointIsAt(minT) + (sphereNormal1*scene.shadow_ray_epsilon);
                                bool isInShadow = isThereObjectInBetween(lightPosition, r.pointIsAt(minT),shadowPoint, scene,scene.shadow_ray_epsilon);
                                if(!isInShadow){
                                    Vec3f lightDir = lightPosition - r.pointIsAt(minT);
                                    Vec3f radiance = findRadiance(scene.point_lights[i], r.pointIsAt(minT));
                                    
                                    //color += calculateDiffuse(sphereNormal1, lightDir, lightIntensity, scene.materials[scene.spheres[index].material_id-1].diffuse, (lightPosition - r.pointIsAt(minT)).Length());
                                    //color += calculateSpecular(sphereNormal1, lightDir,r.direction , lightIntensity, scene.materials[scene.spheres[index].material_id-1].specular, scene.materials[scene.spheres[index].material_id-1].phong_exponent);
                                    color += findDiffuse(sphereNormal1, lightDir, scene.materials[scene.spheres[index].material_id-1].diffuse, radiance);
                                    color += findSpecular(sphereNormal1, lightDir, r.direction, scene.materials[scene.spheres[index].material_id-1].specular, radiance, scene.materials[scene.spheres[index].material_id-1].phong_exponent);
                                }
                            }

                            else if(minT == t3){
                                //mesh normal calculate
                                for(int face = 0; face < scene.meshes[index].faces.size(); face ++){
                                    Vec3f triangleNormal1 = triangleNormal(scene.vertex_data[scene.meshes[index].faces[face].v0_id-1],scene.vertex_data[scene.meshes[index].faces[face].v1_id-1],scene.vertex_data[scene.meshes[index].faces[face].v2_id-1]);
                                    Vec3f shadowPoint = r.pointIsAt(minT) + (triangleNormal1*scene.shadow_ray_epsilon);
                                    bool isInShadow = isThereObjectInBetween(lightPosition, r.pointIsAt(minT), shadowPoint, scene,scene.shadow_ray_epsilon);
                                    if(!isInShadow){
                                        Vec3f lightDir = lightPosition - r.pointIsAt(minT);
                                        Vec3f radiance = findRadiance(scene.point_lights[i], r.pointIsAt(minT));
                                        //color += calculateDiffuse(triangleNormal1, lightDir, lightIntensity, scene.materials[scene.meshes[index].material_id-1].diffuse, (lightPosition - r.pointIsAt(minT)).Length());
                                        //color += calculateSpecular(triangleNormal1, lightDir,r.direction , lightIntensity, scene.materials[scene.meshes[index].material_id-1].specular, scene.materials[scene.meshes[index].material_id-1].phong_exponent);
                                        color += findDiffuse(triangleNormal1, lightDir, scene.materials[scene.meshes[index].material_id-1].diffuse, radiance);
                                        color += findSpecular(triangleNormal1, lightDir, r.direction, scene.materials[scene.meshes[index].material_id-1].specular, radiance, scene.materials[scene.meshes[index].material_id-1].phong_exponent);
                                    }

                                }
                            }
                        } 
                        if(color.x > 255){
                            color.x = 255;
                        }
                        if(color.y > 255){
                            color.y = 255;
                        }
                        if(color.z > 255){
                            color.z = 255;
                        }
                        image[k] = color.x;
                        image[k+1] = color.y;
                        image[k+2] = color.z;
                    }

            /*else{
                for(int i = 0; i < scene.point_lights.size(); i++){
                    Vec3f lightPosition = scene.point_lights[i].position;
                    Vec3f lightIntensity = scene.point_lights[i].intensity;
                    Vec3f intersection = r.pointIsAt(minT);
                    if(isThereObjectInBetween(lightPosition, lightIntensity, intersection, scene)){
                        image[k] = scene.background_color.x;
                        image[k+1] = scene.background_color.y;
                        image[k+2] = scene.background_color.z;
                    }
                    else{
                        image[k] = 255;
                        image[k+1] = 255;
                        image[k+2] = 255;
                        //shading calculations
                    }

                }
            }*/

            k += 3;

        }

       
        

        write_ppm(ppm_filename, image, width, height); 

    }
    
    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.

    /*int width = 640, height = 480;

    unsigned char* image = new unsigned char [width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            
            auto r = double(x) / (width-1);
            auto g = double(y) / (height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            image[i++] = ir;
            image[i++] = ig;
            image[i++] = ib;
        }
    }

    



    const char* xml_filename = "eddtest1.xml";
    // burda sadece çıktı dosyasının ismini ayarlıyoruz. üst kod ise argümansız denemek için bir line kod.
    char ppm_filename[256];
    //const char* xml_filename = argv[1];
    int len = strlen(xml_filename);
    for(int i = 0 ; i  < len-4; i++) 
    {
        ppm_filename[i] = xml_filename[i];
    }
    ppm_filename[len - 4] = '\0';
    strcat(ppm_filename, ".ppm");
*/

    

}
}
