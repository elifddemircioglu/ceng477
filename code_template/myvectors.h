#ifndef __myvectors__
#define __myvectors__

#include <vector>
#include <cmath>



    struct Vec3i
    {
        int x, y, z;
    };

    struct Vec4f
    {
        float x, y, z, w;
    };

    struct Vec3f
    {
        float x, y, z;


        Vec3f() {}

        Vec3f(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vec3f(const Vec3i rhs) {
            this->x = rhs.x;
            this->y = rhs.y;
            this->z = rhs.z;
        }

        Vec3f operator+(const Vec3f rhs) const {
            return Vec3f(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Vec3f operator-(const Vec3f rhs) const {
            return Vec3f(x - rhs.x, y - rhs.y, z - rhs.z);
        }


        // Dot product.
        float operator*(const Vec3f rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        Vec3f operator*(float rhs) const { return Vec3f(x * rhs, y * rhs, z * rhs); }

        Vec3f operator/(float rhs) const {
            Vec3f result(x / rhs, y / rhs, z / rhs);
            return result;
        }

        Vec3f& operator/=(float rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }

        Vec3f& operator*=(float rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        Vec3f& operator+=(const Vec3f rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vec3f Negate() const {
            return Vec3f(0 - x, 0 - y, 0 - z);
        }

        Vec3f CrossProduct(const Vec3f rhs) const {
            return Vec3f(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
                        x * rhs.y - y * rhs.x);
        }

        Vec3f PointWise(const Vec3f rhs) const {
            return Vec3f(x * rhs.x, y * rhs.y, z * rhs.z);
        }

        float Length() const { return sqrt(*this * *this); }

        // const Vec3f Normalized() const { return *this / this->Length(); }

        Vec3f& Normalize() {
            *this /= this->Length();
            return *this;
        }

        Vec3f calculateDirection(Vec3f toVector) 
        {
            Vec3f directionVector = toVector - *this;
            return directionVector.Normalize();
        }
    };




#endif