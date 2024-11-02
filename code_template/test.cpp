#include "intersections.h"
#include "myvectors.h"
#include "ray.h"
#include <iostream>

void runTests() {
    Vec3f a(0, 0, 0);
    Vec3f b(1, 0, 0);
    Vec3f c(0, 1, 0);

    // Case 1: Ray intersects the triangle from the front
    Ray ray1(Vec3f(0.25f, 0.25f, 1), Vec3f(0, 0, -1));
    float t1;
    bool hit1 = rayTriangleIntersect(a, b, c, ray1, t1);
    std::cout << "Test Case 1: Intersection from front - " << (hit1 ? "Hit" : "Miss") << ", t = " << t1 << '\n';

    // Case 2: Ray misses the triangle
    Ray ray2(Vec3f(2, 2, 1), Vec3f(0, 0, -1));
    float t2;
    bool hit2 = rayTriangleIntersect(a, b, c, ray2, t2);
    std::cout << "Test Case 2: Ray misses - " << (hit2 ? "Hit" : "Miss") << '\n';

    // Case 3: Ray intersects triangle's plane but outside triangle boundary
    Ray ray3(Vec3f(1, 1, 1), Vec3f(0, 0, -1));
    float t3;
    bool hit3 = rayTriangleIntersect(a, b, c, ray3, t3);
    std::cout << "Test Case 3: Intersection outside triangle - " << (hit3 ? "Hit" : "Miss") << '\n';

    // Case 4: Ray parallel to the triangle's plane
    Ray ray4(Vec3f(0, 0, 1), Vec3f(1, 1, 0));
    float t4;
    bool hit4 = rayTriangleIntersect(a, b, c, ray4, t4);
    std::cout << "Test Case 4: Ray parallel to plane - " << (hit4 ? "Hit" : "Miss") << '\n';

    // Case 5: Ray intersects exactly on edge `ab`
    Ray ray5(Vec3f(0.5f, -0.1f, 1), Vec3f(0, 0, -1));
    float t5;
    bool hit5 = rayTriangleIntersect(a, b, c, ray5, t5);
    std::cout << "Test Case 5: Intersection on edge ab - " << (hit5 ? "Hit" : "Miss") << ", t = " << t5 << '\n';

    // Case 6: Ray intersects exactly at vertex `a`
    Ray ray6(Vec3f(0, 0, 1), Vec3f(0, 0, -1));
    float t6;
    bool hit6 = rayTriangleIntersect(a, b, c, ray6, t6);
    std::cout << "Test Case 6: Intersection at vertex a - " << (hit6 ? "Hit" : "Miss") << ", t = " << t6 << '\n';

    // New Cases
    // Case 7: Ray originates inside the triangle
    Ray ray7(Vec3f(0.25f, 0.25f, 0), Vec3f(0, 0, 1));
    float t7;
    bool hit7 = rayTriangleIntersect(a, b, c, ray7, t7);
    std::cout << "Test Case 7: Ray originates inside the triangle - " << (hit7 ? "Hit" : "Miss") << ", t = " << t7 << '\n';

    // Case 8: Ray intersects exactly on edge `bc`
    Ray ray8(Vec3f(0.5f, 0.5f, 1), Vec3f(0, 0, -1));
    float t8;
    bool hit8 = rayTriangleIntersect(a, b, c, ray8, t8);
    std::cout << "Test Case 8: Intersection on edge bc - " << (hit8 ? "Hit" : "Miss") << ", t = " << t8 << '\n';

    // Case 9: Ray just misses above the triangle
    Ray ray9(Vec3f(0.5f, 0.5f, 1), Vec3f(0.5f, 0.5f, -1));
    float t9;
    bool hit9 = rayTriangleIntersect(a, b, c, ray9, t9);
    std::cout << "Test Case 9: Ray misses just outside triangle - " << (hit9 ? "Hit" : "Miss") << '\n';

    // Case 10: Ray skims parallel to the triangle's plane
    Ray ray10(Vec3f(0.5f, 0.5f, 0), Vec3f(1, -1, 0));
    float t10;
    bool hit10 = rayTriangleIntersect(a, b, c, ray10, t10);
    std::cout << "Test Case 10: Ray parallel to plane - " << (hit10 ? "Hit" : "Miss") << '\n';

    // Case 11: Ray originates at vertex `b` and points toward vertex `c`
    Ray ray11(b, c - b);
    float t11;
    bool hit11 = rayTriangleIntersect(a, b, c, ray11, t11);
    std::cout << "Test Case 11: Ray originates at vertex b, points towards c - " << (hit11 ? "Hit" : "Miss") << ", t = " << t11 << '\n';

    // Case 12: Ray intersects exactly on edge `ca`
    Ray ray12(Vec3f(-0.1f, 0.5f, 1), Vec3f(0, 0, -1));
    float t12;
    bool hit12 = rayTriangleIntersect(a, b, c, ray12, t12);
    std::cout << "Test Case 12: Intersection on edge ca - " << (hit12 ? "Hit" : "Miss") << ", t = " << t12 << '\n';

}

int main() {
    runTests();
    return 0;
}
