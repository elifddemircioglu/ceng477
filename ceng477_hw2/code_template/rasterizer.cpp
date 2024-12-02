#include "Helpers.h"
#include "Triangle.h"
#include "Scene.h"
#include <iostream>


int main(int argc, char* argv[])
{

    if (argc < 2) {
    std::cout << "please provide scene file" << std::endl;
    return 1;
    }


    Scene *scene = new Scene("scene.xml");


    scene->initializeImage(scene->cameras[0]);
    scene->forwardRenderingPipeline(scene->cameras[0]);
    scene->convertPPMToPNG(scene->cameras[0]->outputFilename, 1);

    return 0;

}