## Specifications
1. Name of the executable will be “rasterizer”.
2. Your executable will take an input file name from command line. Input file names are not static, they can be anything.
3. The scene will be composed of instances of meshes. A set of triangles will form a mesh with a sequence of several transformations (translation, rotation, scaling) you will be able to move, rotate, or resize a mesh (i.e., all of the triangles in the mesh). Transformations will be applied to the meshes in the order specified in the input file.
4. You will not implement any lighting computations in this assignment. The color of each vertex will be provided as input and your goal will be to interpolate color along the edges and the surfaces of the triangles in wireframe and solid modes, respectively.
5. You will implement two types of projection transformations: orthographic projection and perspective projection.
6. Use the midpoint algorithm to draw triangle edges and use the barycentric coordinates based algorithm to rasterize triangle faces.
7. In both wireframe and solid modes, triangles whose backface is facing the viewer will be culled. Backface culling should be enabled/disabled according to the setting in input file. You should calculate normal vectors for backface culling. Default value for backface culling setting is ’disabled’.
8. You will also implement a clipping algorithm of your choice. For example, you can choose an algorithm among algorithms that you saw on the lectures. You will do clipping for wireframe mode only. For solid mode meshes, you can assume that the mesh is completely visible to the camera.
9. Lastly, you will implement depth buffer algorithm in this homework. You should calculate the depth values of objects and paint the color of the object that is in front of the others.
10. There can be multiple camera configurations for each given scene.
11. The objects in the scene can share a vertex. Also, same object can be used multiple times in the scene.
12. You will NOT deal with reading input files. Helper functions for reading inputs, helper functions for mathematical operations (e.g. normalization, matrix multiplication) are given to you in “Scene.cpp” and “Helpers.cpp” files. It is STRONGLY recommended to inspect types in header files and helper functions, which are given to you in homework stub code. You can modify these helper functions as you wish, or you can implement your own helper functions or you can start from scratch.

## To Do's
1. Vertex Processing
	1. Get vertices in specific order and connectivity information
	2.  Process (transform) vertices
	3. in this section
		1. Modelling transformations 
		2. Camera transformations
		3. Projection transformations
2. Primitive Assembly
	1. Create primitives from connected vertices
		1. Lines
		2. Triangles
		3. Quadrangles...
		4. Meshes
3. Clipping and Culling
	1. Clip and cull primitives to eliminate invisible ones
		1. During clipping and culling, primitives outside the CVV must be culled
		2. Primitives partially inside the CVV must be clipped
			1. May produce new vertices
4. Viewport Transform
	1. Transform primitives to screen space (preserve z)
		1. With viewport transform, all surviving( and potentially clipped ) primitives acquire viewport coordinates
			1. We work on integer pixel coordinates from this point on
			2. Depth can be fixed point or floating point number in \[0,1] range (unless changed by glDepthRange)
5. Rasterization
	1. Rasterize primitives to obtain fragments
		1. Rasterization is the process of determining the pixels (fragments) that make up a primitive
			1. Different algorithms for lines, triangles, etc.
6. Fragment Processing
	1. Process fragments to obtain visible pixel with color
		1. Each fragment may be further processed before being a visible pixel on the screen (or being written to a memory buffer)
			1. depth testing
			2. alpha blending
			3. ...
