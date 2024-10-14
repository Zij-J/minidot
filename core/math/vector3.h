// 3 float tuple for easy operaters & function passings  

#ifndef VECTOR3_H
#define VECTOR3_H

#include "vector2i.h"

struct Vector3 {
    float x;
    float y;
    float z;

    const Vector3 &operator+=(const Vector3 &input) {
        x += input.x;
        y += input.y;
        z += input.z;
        return *this;
    }

    const Vector3 &operator+=(const Vector2i &input) {
        x += static_cast<float>(input.x);
        y += static_cast<float>(input.y);
        return *this;
    }

    template <typename Type>
    Vector3 operator+(const Type &right) const {
        Vector3 result = *this;
        result += right;
        return result;  // copy elision do things
    }

    Vector3 &transformed_by(const Vector3 &basis_x, const Vector3 &basis_y, const Vector3 &basis_z) {
        *this = {
            .x = basis_x.x * x + basis_y.x * y + basis_z.x * z,
            .y = basis_x.y * x + basis_y.y * y + basis_z.y * z,
            .z = basis_x.z * x + basis_y.z * y + basis_z.z * z
        };
        return *this;
    }
};


#endif