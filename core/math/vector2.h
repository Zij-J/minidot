// 2 float tuple for easy operaters & function passings  

#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2 {
    float x;
    float y;

    const Vector2 &operator+=(const Vector2 &input) {
        x += input.x;
        y += input.y;
        return *this;
    }

    Vector2 operator+(const Vector2 &right) const {
        Vector2 result = *this;
        result += right;
        return result;  // copy elision do things
    }

    Vector2 &transformed_by(const Vector2 &basis_x, const Vector2 &basis_y) {
        *this = {
            .x = basis_x.x * x + basis_y.x * y,
            .y = basis_x.y * x + basis_y.y * y
        };
        return *this;
    }
};



#endif