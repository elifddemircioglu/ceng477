// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <cmath>
// #include "intersections.h"

// const int WIDTH = 800;
// const int HEIGHT = 600;
// const float ASPECT_RATIO = float(WIDTH) / HEIGHT;
// const float FOV = 90.0f; // Field of view

// struct Color {
//     int r, g, b;
//     Color(int r, int g, int b) : r(r), g(g), b(b) {}
// };

// // Simple function to write an image to a PPM file
// void writePPM(const std::string& filename, const std::vector<Color>& framebuffer) {
//     std::ofstream ofs(filename, std::ios::out | std::ios::binary);
//     ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
//     for (const auto& color : framebuffer) {
//         ofs << (unsigned char)(std::min(255, color.r)) 
//             << (unsigned char)(std::min(255, color.g)) 
//             << (unsigned char)(std::min(255, color.b));
//     }
//     ofs.close();
// }

// int main() {
//     std::vector<Color> framebuffer(WIDTH * HEIGHT, Color(0, 0, 0));

//     // Define a triangle
//     Vec3f a(-0.5f, 0.5f, -2.0f);
//     Vec3f b(-0.5f, -0.5f, -2.0f);
//     Vec3f c(0.5f, -0.5f, -2.0f);
//     Vec3f d(0.5f, 0.5f, -2.0f);
//     Vec3f e(0.75f, 0.75f, -2.0f);
//     Vec3f f(1.0f, 0.75f, -2.0f);
//     Vec3f g(0.875f, 1.0f, -2.0f);
//     Vec3f h(-0.875f, 1.0f, -2.0f);

//     // Define a sphere
//     Vec3f sphere_center(0.0f, 0.0f, -6.0f);
//     float sphere_radius = 1.0f;

//     // Loop through each pixel on the image plane
//     for (int j = 0; j < HEIGHT; ++j) {
//         for (int i = 0; i < WIDTH; ++i) {
//             // Calculate normalized screen coordinates
//             float x = (2 * (i + 0.5) / float(WIDTH) - 1) * ASPECT_RATIO * tan(FOV / 2 * M_PI / 180);
//             float y = (1 - 2 * (j + 0.5) / float(HEIGHT)) * tan(FOV / 2 * M_PI / 180);

//             // Define ray from camera origin (0, 0, 0) through the pixel
//             Vec3f ray_origin(0, 0, 0);
//             Vec3f ray_direction = Vec3f(x, y, -1).Normalize();
//             Ray ray(ray_origin, ray_direction);

//             // Check for intersection with the triangle and sphere
//             float t;
//             bool hit_triangle = rayTriangleIntersect(c, a, b, ray, t);
//             bool hit_triangle2 = rayTriangleIntersect(a, c, d, ray, t);
//             //bool hit_triangle3 = rayTriangleIntersect(a, b, c, ray, t);
//             bool hit_sphere = raySphereIntersect(sphere_center, sphere_radius, ray, t);

//             // Basic shading based on hit
//             if (hit_sphere) {
//                 framebuffer[j * WIDTH + i] = Color(255, 0, 0); // Red for triangle
//             } else if (hit_triangle || hit_triangle2) {
//                 framebuffer[j * WIDTH + i] = Color(0, 255, 0); // Green for sphere
//             } else {
//                 framebuffer[j * WIDTH + i] = Color(30, 30, 30); // Background color
//             }
//         }
//     }

//     // Output the framebuffer to a file
//     writePPM("output.ppm", framebuffer);
//     std::cout << "Image rendered to output.ppm" << std::endl;

//     return 0;
// }
