#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "tinyxml2.h"
#include "myvectors.h"
#include "ray.h"
typedef unsigned char RGB[3];

int main(int argc, char* argv[])
{

    // this is just for making the user to provide a parser_scene file.
    if (argc < 2) {
    std::cout << "please provide parser_scene file" << std::endl;
    return 1;
    }
    
    // Sample usage for reading an XML scene file
    parser::Scene parser_scene;




    parser_scene.loadFromXml(argv[1]);
    // buraya bi parse edilen scene ile alakali var olan objelerin bi hit table'i eklensin
    // ayni sekilde bence cameralari da bi kamera objesine cekelim

    for(int camno = 0; camno < parser_scene.cameras.size(); camno++) // for every camera we have to generate a pic.
    {
        // buraya bence bi camera modulu eklenip operasyonlar o kamera objesi uzerinden dondurulsun. 
        // ray_color , render vb vb
        
        const char* ppm_filename = parser_scene.cameras[camno].image_name.c_str(); // get the name for outfile.

        //resolution of the image
        int width = parser_scene.cameras[camno].image_width;
        int height = parser_scene.cameras[camno].image_height;

        //this is the image data array for every rgb for every pixel on the image. basically pixel numer * 3.
        unsigned char* image = new unsigned char [width * height * 3];


        //for camera and image plane calculations (acoording to slide 02)
        Vec3f w_dir = parser_scene.cameras[camno].gaze * -1;
        Vec3f u_dir = w_dir.CrossProduct(parser_scene.cameras[camno].up).Normalize();
        Vec3f& v_dir = parser_scene.cameras[camno].up;
        Vec3f& e = parser_scene.cameras[camno].position;

        // image plane
        float l = parser_scene.cameras[camno].near_plane.x;
        float r = parser_scene.cameras[camno].near_plane.y;
        float b = parser_scene.cameras[camno].near_plane.z;
        float t = parser_scene.cameras[camno].near_plane.w;

        Vec3f center = e + parser_scene.cameras[camno].gaze * parser_scene.cameras[camno].near_distance;
        Vec3f lefttopcorner = center + v_dir * t + u_dir * l;
        
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
                temp_t = max
                temp_obj;
                for(int size = 0; size < parser.scene.meshes.length() -1 ; size++ ){
                    if intersectmesh(t) 
                        if temp_t > t
                            temp_t = t
                            temp_obj = hit_record
                }
                for(int size = 0; size < parser.scene.triangle.length() -1 ; size++ ){
                    if intersecttriangle(t)
                        if temp_t > t
                            temp_t = t 
                            temp_obj = hit_record
                    }
                for(int size = 0; size < parser.scene.spheres.length() -1 ; size++ ){
                    if intersectspheres(t)
                        if temp_t > t
                            temp_t = t 
                            temp_obj = hit_record
                }

                

    
            }
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
