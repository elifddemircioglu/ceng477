#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include "tinyxml2.h"
#include "Triangle.h"
#include "Helpers.h"
#include "Scene.h"

#include <unordered_set>

using namespace tinyxml2;
using namespace std;

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *xmlElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *rootNode = xmlDoc.FirstChild();

	// read background color
	xmlElement = rootNode->FirstChildElement("BackgroundColor");
	str = xmlElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	xmlElement = rootNode->FirstChildElement("Culling");
	if (xmlElement != NULL)
	{
		str = xmlElement->GetText();

		if (strcmp(str, "enabled") == 0)
		{
			this->cullingEnabled = true;
		}
		else
		{
			this->cullingEnabled = false;
		}
	}

	// read cameras
	xmlElement = rootNode->FirstChildElement("Cameras");
	XMLElement *camElement = xmlElement->FirstChildElement("Camera");
	XMLElement *camFieldElement;
	while (camElement != NULL)
	{
		Camera *camera = new Camera();

		camElement->QueryIntAttribute("id", &camera->cameraId);

		// read projection type
		str = camElement->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
		{
			camera->projectionType = ORTOGRAPHIC_PROJECTION;
		}
		else
		{
			camera->projectionType = PERSPECTIVE_PROJECTION;
		}

		camFieldElement = camElement->FirstChildElement("Position");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->position.x, &camera->position.y, &camera->position.z);

		camFieldElement = camElement->FirstChildElement("Gaze");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->gaze.x, &camera->gaze.y, &camera->gaze.z);

		camFieldElement = camElement->FirstChildElement("Up");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->v.x, &camera->v.y, &camera->v.z);

		camera->gaze = normalizeVec3(camera->gaze);
		camera->u = crossProductVec3(camera->gaze, camera->v);
		camera->u = normalizeVec3(camera->u);

		camera->w = inverseVec3(camera->gaze);
		camera->v = crossProductVec3(camera->u, camera->gaze);
		camera->v = normalizeVec3(camera->v);

		camFieldElement = camElement->FirstChildElement("ImagePlane");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &camera->left, &camera->right, &camera->bottom, &camera->top,
			   &camera->near, &camera->far, &camera->horRes, &camera->verRes);

		camFieldElement = camElement->FirstChildElement("OutputName");
		str = camFieldElement->GetText();
		camera->outputFilename = string(str);

		this->cameras.push_back(camera);

		camElement = camElement->NextSiblingElement("Camera");
	}

	// read vertices
	xmlElement = rootNode->FirstChildElement("Vertices");
	XMLElement *vertexElement = xmlElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (vertexElement != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = vertexElement->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = vertexElement->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		this->vertices.push_back(vertex);
		this->colorsOfVertices.push_back(color);

		vertexElement = vertexElement->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	xmlElement = rootNode->FirstChildElement("Translations");
	XMLElement *translationElement = xmlElement->FirstChildElement("Translation");
	while (translationElement != NULL)
	{
		Translation *translation = new Translation();

		translationElement->QueryIntAttribute("id", &translation->translationId);

		str = translationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		this->translations.push_back(translation);

		translationElement = translationElement->NextSiblingElement("Translation");
	}

	// read scalings
	xmlElement = rootNode->FirstChildElement("Scalings");
	XMLElement *scalingElement = xmlElement->FirstChildElement("Scaling");
	while (scalingElement != NULL)
	{
		Scaling *scaling = new Scaling();

		scalingElement->QueryIntAttribute("id", &scaling->scalingId);
		str = scalingElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		this->scalings.push_back(scaling);

		scalingElement = scalingElement->NextSiblingElement("Scaling");
	}

	// read rotations
	xmlElement = rootNode->FirstChildElement("Rotations");
	XMLElement *rotationElement = xmlElement->FirstChildElement("Rotation");
	while (rotationElement != NULL)
	{
		Rotation *rotation = new Rotation();

		rotationElement->QueryIntAttribute("id", &rotation->rotationId);
		str = rotationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		this->rotations.push_back(rotation);

		rotationElement = rotationElement->NextSiblingElement("Rotation");
	}

	// read meshes
	xmlElement = rootNode->FirstChildElement("Meshes");

	XMLElement *meshElement = xmlElement->FirstChildElement("Mesh");
	while (meshElement != NULL)
	{
		Mesh *mesh = new Mesh();

		meshElement->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = meshElement->Attribute("type");

		if (strcmp(str, "wireframe") == 0)
		{
			mesh->type = WIREFRAME_MESH;
		}
		else
		{
			mesh->type = SOLID_MESH;
		}

		// read mesh transformations
		XMLElement *meshTransformationsElement = meshElement->FirstChildElement("Transformations");
		XMLElement *meshTransformationElement = meshTransformationsElement->FirstChildElement("Transformation");

		while (meshTransformationElement != NULL)
		{
			char transformationType;
			int transformationId;

			str = meshTransformationElement->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			meshTransformationElement = meshTransformationElement->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *cloneStr;
		int v1, v2, v3;
		XMLElement *meshFacesElement = meshElement->FirstChildElement("Faces");
		str = meshFacesElement->GetText();
		cloneStr = strdup(str);

		row = strtok(cloneStr, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

			if (result != EOF)
			{
				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		this->meshes.push_back(mesh);

		meshElement = meshElement->NextSiblingElement("Mesh");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}


/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFilename.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFilename << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "./magick " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}


/*
	Transformations, clipping, culling, rasterization are done here.
*/


bool liangBarsky_3D(float &x1, float &y1, float &z1, float &x2, float &y2, float &z2, float w1, float w2) {
    // Step 1: Compute deltas (line direction)
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;

    // Step 2: Define clipping boundaries
    // The p[] array contains direction factors for clipping planes
    float p[6] = {-dx, dx, -dy, dy, -dz, dz};
    
    // The q[] array contains distances from the segment's start point to the clipping planes
    float q[6] = {x1 + w1, w1 - x1, y1 + w1, w1 - y1, z1 + w1, w1 - z1};

    // Step 3: Initialize parametric clipping values
    float t_min = 0.0f;  // Start of the clipped segment
    float t_max = 1.0f;  // End of the clipped segment

    // Step 4: Iterate over all 6 planes
    for (int i = 0; i < 6; i++) {
        if (p[i] == 0) {  // Line is parallel to this clipping plane
            if (q[i] < 0) return false;  // Line is completely outside the plane
        } else {
            float t = q[i] / p[i];  // Compute the parametric intersection point
            if (p[i] < 0) {
                // Line enters the clipping region
                t_min = std::max(t_min, t);
            } else {
                // Line leaves the clipping region
                t_max = std::min(t_max, t);
            }
        }
    }

    // Step 5: Check if the segment is completely outside
    if (t_min > t_max) return false;

    // Step 6: Compute clipped coordinates
    float newX1 = x1 + t_min * dx;
    float newY1 = y1 + t_min * dy;
    float newZ1 = z1 + t_min * dz;

    float newX2 = x1 + t_max * dx;
    float newY2 = y1 + t_max * dy;
    float newZ2 = z1 + t_max * dz;

    // Update the input parameters with the clipped coordinates
    x1 = newX1;
    y1 = newY1;
    z1 = newZ1;

    x2 = newX2;
    y2 = newY2;
    z2 = newZ2;

    // The line was successfully clipped
    return true;
}



 void Scene::makeTransformations(std::vector<std::unordered_map<int, Vec3>> &allTransformedVertices){

	

	for(size_t k = 0; k < meshes.size(); k++){

		Mesh *mesh = meshes[k];

		std::unordered_set<int> hashVertexIds;
		std::vector<int> distinctVertexIds;
		std::unordered_map<int, Vec3> transformedVerticesMap;

		//after this loop, we gave distinct vertices of the mesh
		for(size_t j = 0; j < mesh->triangles.size(); j++){

			Triangle currentTriangle = mesh->triangles[j];

			for (int v = 0; v < 3; v++) { //for each vertex of the triangle
				int vertexId = currentTriangle.vertexIds[v];
				if(hashVertexIds.insert(vertexId).second){
					distinctVertexIds.push_back(vertexId);
				}
			}
		}

		//we have this loop because we want to make the transformations on top of each other. so we will change only this list in the below loops.
		
		for(size_t j = 0; j < distinctVertexIds.size(); j++){
			Vec3 vertexCoord;
			vertexCoord.x = vertices[distinctVertexIds[j]-1]->x;
			vertexCoord.y = vertices[distinctVertexIds[j]-1]->y;
			vertexCoord.z = vertices[distinctVertexIds[j]-1]->z;
			transformedVerticesMap.insert(make_pair(distinctVertexIds[j], vertexCoord)); // vertexId, vertexCoord pairi
		}

		for(int i = 0; i < mesh->numberOfTransformations; i++){

			char transformationType = mesh->transformationTypes[i];
			int transformationId = mesh->transformationIds[i];

			if(transformationType == 't'){
				Translation *translation;
				translation = translations[transformationId-1];
				
				for(size_t j = 0; j < distinctVertexIds.size(); j++){
					int vertexId = distinctVertexIds[j];
					transformedVerticesMap[vertexId].x += translation->tx;
					transformedVerticesMap[vertexId].y += translation->ty;
					transformedVerticesMap[vertexId].z += translation->tz;
				}
			}

			else if(transformationType == 's'){
				Scaling *scaling;
				scaling = scalings[transformationId-1];
				
				// acaba burda direkt scale mi edicez orijine göre yoksa merkeeze taşıyıp mı scale edicez aynı noktada büyüsün diye ?
				//after this loop, distinctVertexIds will contain all the distinct vertex ids of the mesh so we can apply the transformation exactly once to them
				for(size_t j = 0; j < distinctVertexIds.size(); j++){
					int vertexId = distinctVertexIds[j];
					transformedVerticesMap[vertexId].x *= scaling->sx;
					transformedVerticesMap[vertexId].y *= scaling->sy;
					transformedVerticesMap[vertexId].z *= scaling->sz;
				}

			}

			else if(transformationType == 'r'){
				Rotation *rotation;
				rotation = rotations[transformationId-1];

				//rodrigez formula
				//after this loop, distinctVertexIds will contain all the distinct vertex ids of the mesh so we can apply the transformation exactly once to them
				for(size_t j = 0; j < distinctVertexIds.size(); j++){
					double angle = rotation->angle;

					double ux = rotation->ux/ magnitudeOfVec3(Vec3(rotation->ux, rotation->uy, rotation->uz));
					double uy = rotation->uy/ magnitudeOfVec3(Vec3(rotation->ux, rotation->uy, rotation->uz));
					double uz = rotation->uz/ magnitudeOfVec3(Vec3(rotation->ux, rotation->uy, rotation->uz));

					double c = cos(angle);
					double s = sin(angle);
					double t = 1 - c;

					int vertexId = distinctVertexIds[j];
					double x = transformedVerticesMap[vertexId].x;
					double y = transformedVerticesMap[vertexId].y;
					double z = transformedVerticesMap[vertexId].z;

					double xNew = (t * ux * ux + c) * x + (t * ux * uy - s * uz) * y + (t * ux * uz + s * uy) * z;
					double yNew = (t * ux * uy + s * uz) * x + (t * uy * uy + c) * y + (t * uy * uz - s * ux) * z;
					double zNew = (t * ux * uz - s * uy) * x + (t * uy * uz + s * ux) * y + (t * uz * uz + c) * z;

					transformedVerticesMap[vertexId].x = xNew;
					transformedVerticesMap[vertexId].y = yNew;
					transformedVerticesMap[vertexId].z = zNew;
					
				}
			}


		}

		allTransformedVertices.push_back(transformedVerticesMap);
	}
}



void Scene::forwardRenderingPipeline(Camera *camera)
{
	// TODO: Implement this function

	Matrix4 cameraMatrix;
	cameraMatrix.values[0][0] = camera->u.x;
	cameraMatrix.values[0][1] = camera->u.y;
	cameraMatrix.values[0][2] = camera->u.z;
	cameraMatrix.values[0][3] = -dotProductVec3(camera->position, camera->u);
	cameraMatrix.values[1][0] = camera->v.x;
	cameraMatrix.values[1][1] = camera->v.y;
	cameraMatrix.values[1][2] = camera->v.z;
	cameraMatrix.values[1][3] = -dotProductVec3(camera->position, camera->v);
	cameraMatrix.values[2][0] = camera->w.x;
	cameraMatrix.values[2][1] = camera->w.y;
	cameraMatrix.values[2][2] = camera->w.z;
	cameraMatrix.values[2][3] = -dotProductVec3(camera->position, camera->w);
	cameraMatrix.values[3][3] = 1;

	Matrix4 orthographicMatrix;
	orthographicMatrix.values[0][0] = 2 / (camera->right - camera->left);
	orthographicMatrix.values[1][1] = 2 / (camera->top - camera->bottom);
	orthographicMatrix.values[2][2] = -2 / (camera->far - camera->near);
	orthographicMatrix.values[0][3] = -(camera->right + camera->left) / (camera->right - camera->left);
	orthographicMatrix.values[1][3] = -(camera->top + camera->bottom) / (camera->top - camera->bottom);
	orthographicMatrix.values[2][3] = -(camera->far + camera->near) / (camera->far - camera->near);
	orthographicMatrix.values[3][3] = 1;

	Matrix4 p2oMatrix;
	p2oMatrix.values[0][0] = camera->near;
	p2oMatrix.values[1][1] = camera->near;
	p2oMatrix.values[2][2] = camera->near + camera->far;
	p2oMatrix.values[2][3] = camera->near * camera->far;
	p2oMatrix.values[3][2] = -1;


	Matrix4 viewportMatrix;
	viewportMatrix.values[0][0] = camera->horRes / 2;
	viewportMatrix.values[1][1] = camera->verRes / 2;
	viewportMatrix.values[2][2] = 1/2;
	viewportMatrix.values[2][3] = 1/2;
	viewportMatrix.values[0][3] = (camera->horRes - 1) / 2;
	viewportMatrix.values[1][3] = (camera->verRes - 1) / 2;

	//scaling,translation,rotatiion 
	std::vector<std::unordered_map<int, Vec3>> allTransformedVertices;
	
	makeTransformations(allTransformedVertices);

	//first convert the vertices to ortographic view and  then  do frustum culling
	
	vector<int> meshesToCull; //cull edilecek meshleri tutuyoruz
	vector<unordered_set<int>> backfacingTriangleIndices; //hangi meshte hangi triangller backfacing tutuyoruz
	
	
	std::vector<std::unordered_map<int, Vec4>> allTransformedVerticesVec4;

	for(size_t i = 0; i< meshes.size();i++){

		std::unordered_map<int, Vec3>::iterator it= allTransformedVertices[i].begin();
		bool isCulled = true;
		for(size_t j = 0 ; j < allTransformedVertices[i].size() ;j++ , it++){

			Vec3 vertex = it->second;
			Vec4 vertexVec4 = Vec4(vertex.x, vertex.y, vertex.z, 1);
			
			vertexVec4 = multiplyMatrixWithVec4(cameraMatrix, vertexVec4);
			if(camera->projectionType == PERSPECTIVE_PROJECTION){
				vertexVec4 = multiplyMatrixWithVec4(p2oMatrix, vertexVec4);
			}

			allTransformedVerticesVec4[i].insert(make_pair(it->first, vertexVec4));

			//now we have the vertices in the ortographic view	volume and make frustum and backface culling
			if(cullingEnabled){

				//volume-frustum culling
				if(-vertex.z >= camera->near && -vertex.z <= camera->far){
					if(vertex.y >= camera->bottom && vertex.y <= camera->top){
						if(vertex.x >= camera->left && vertex.x <= camera->right){
							isCulled = false; //eğer içeride bir tane bile vertexi varsa meshin, bu meshi cull edemeyiz, clip etmemiz lazım.
						}
					}
				}				
			}
		}	
		if(cullingEnabled && isCulled){meshesToCull.push_back(i);}
		//backface culling , eğer volumün içindeyse yapıyorum
		if(cullingEnabled && !isCulled){
			for(size_t k = 0; k< meshes[i]->triangles.size(); k++){
				Triangle currentTriangle = meshes[i]->triangles[k];
				Vec4 v1 = allTransformedVerticesVec4[i][currentTriangle.vertexIds[0]]; //vertexler mapte id ve vec4 pairi olarak tutuluyor.
				Vec4 v2 = allTransformedVerticesVec4[i][currentTriangle.vertexIds[1]]; // O(1) acces time oldupundan mapten direk alıyoruz.
				Vec4 v3 = allTransformedVerticesVec4[i][currentTriangle.vertexIds[2]];

				Vec3 v1Vec3 = Vec3(v1.x, v1.y, v1.z);
				Vec3 v2Vec3 = Vec3(v2.x, v2.y, v2.z);
				Vec3 v3Vec3 = Vec3(v3.x, v3.y, v3.z);
				Vec3 center = multiplyVec3WithScalar(addVec3(addVec3(v1Vec3, v2Vec3), v3Vec3), 1/3);

				Vec3 normal = normalizeVec3((subtractVec3(v2Vec3, v1Vec3), subtractVec3(v3Vec3, v1Vec3)));
				Vec3 cameraToTriangle = normalizeVec3(subtractVec3(center, Vec3(camera->position.x, camera->position.y, camera->position.z)));

				if(dotProductVec3(normal, cameraToTriangle) > 0){ // eğer 0dan büyükse aralarında açı 90dan küçüktür, backface slaytlara göre
					backfacingTriangleIndices[i].insert(k);
				}
			}
		}
	}

	//now we have the vertices in the ortographic view and we have done the frustum and backface culling. 
	//
			

	//clipping  but just done for wireframe meshes
	//shared vertices icin yine indexleri tutan bir set olusturdum
	std::unordered_set<int> processedVertices;
	for(size_t i = 0; i < meshes.size(); i++){

		if(meshes[i]->type == WIREFRAME_MESH){
			for(size_t j = 0; j < meshes[i]->triangles.size(); j++){ // her triangle için clipping yap.
				//clipping fonkisyonu yazılacak

				// asagisini anlasilirlik icin unrolled bir sekilde her vertice'i yazdim, direkt bir for loop'ta da yazilir


				Triangle currentTriangle = meshes[i]->triangles[k];
				Vec4 v1 = allTransformedVerticesVec4[i][currentTriangle.vertexIds[0]]; 
				int v1_index = currentTriangle.vertexIds[0];
				Vec4 v2 = allTransformedVerticesVec4[i][currentTriangle.vertexIds[1]]; 
				int v2_index = currentTriangle.vertexIds[1];
				Vec4 v3 = allTransformedVerticesVec4[i][currentTriangle.vertexIds[2]];
				int v3_index = currentTriangle.vertexIds[2];
				
                float x1 = v1.x, y1 = v1.y, z1 = v1.z, w1 = v1.w;
                float x2 = v2.x, y2 = v2.y, z2 = v2.z, w2 = v2.w;
                float x3 = v3.x, y3 = v3.y, z3 = v3.z, w3 = v3.w;
				if (processedVertices.find(v1_index) == processedVertices.end() ||
					processedVertices.find(v2_index) == processedVertices.end()) {
                    if (liangBarsky_3D(x1, y1, z1, x2, y2, z2, w1, w2)) {
                        x1 /= w1; y1 /= w1; z1 /= w1;
                        x2 /= w2; y2 /= w2; z2 /= w2;

                        allTransformedVerticesVec4[i][v1_index] = Vec4(x1, y1, z1, 1.0f);
                        allTransformedVerticesVec4[i][v2_index] = Vec4(x2, y2, z2, 1.0f);

                        processedVertices.insert(v1_index);
                        processedVertices.insert(v2_index);
                    }
                }
				if (processedVertices.find(v2_index) == processedVertices.end() ||
					processedVertices.find(v3_index) == processedVertices.end()) {
                    if (liangBarsky_3D(x2, y2, z2, x3, y3, z3, w2, w3)) {
						x2 /= w2; y2 /= w2; z2 /= w2;
						x3 /= w3; y3 /= w3; z3 /= w3;

						allTransformedVerticesVec4[i][v2_index] = Vec4(x1, y1, z1, 1.0f);
                        allTransformedVerticesVec4[i][v3_index] = Vec4(x2, y2, z2, 1.0f);

                        processedVertices.insert(v2_index);
                        processedVertices.insert(v3_index);
					}
                }
				if (processedVertices.find(v3_index) == processedVertices.end() ||
					processedVertices.find(v1_index) == processedVertices.end()) {
                    if (liangBarsky_3D(x3, y3, z3, x1, y1, z1, w3, w1)) {
						x3 /= w3; y3 /= w3; z3 /= w3;
						x1 /= w1; y1 /= w1; z1 /= w1;
					
						allTransformedVerticesVec4[i][v3_index] = Vec4(x1, y1, z1, 1.0f);
                        allTransformedVerticesVec4[i][v1_index] = Vec4(x2, y2, z2, 1.0f);

                        processedVertices.insert(v3_index);
                        processedVertices.insert(v1_index);
					}
                }

				Vec4 screen_v1 = multiplyMatrixWithVec4(viewportMatrix, Vec4(x1, y1, z1, 1.0f));
                Vec4 screen_v2 = multiplyMatrixWithVec4(viewportMatrix, Vec4(x2, y2, z2, 1.0f));
				Vec4 screen_v3 = multiplyMatrixWithVec4(viewportMatrix, Vec4(x3, y3, z3, 1.0f));
				allTransformedVerticesVec4[i][currentTriangle.vertexIds[0]] = screen_v1;
				allTransformedVerticesVec4[i][currentTriangle.vertexIds[1]] = screen_v2;
                allTransformedVerticesVec4[i][currentTriangle.vertexIds[2]] = screen_v3;
			}	
		
		}
		else if(meshes[i]->type == SOLID_MESH ){
			//
			//
		}
		else
			return;


	}

	//rasterize
}