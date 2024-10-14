// 2 int tuple for easy operaters & function passings  

#ifndef VECTOR2I_H
#define VECTOR2I_H

struct Vector2i  {
    int x;
    int y;

    const Vector2i &operator+=(const Vector2i &input) {
        x += input.x;
        y += input.y;
        return *this;
    }

    Vector2i operator+(const Vector2i &right) const {
        Vector2i result = *this;
        result += right;
        return result;  // copy elision do things
    }

    Vector2i &transformed_by(const Vector2i &basis_x, const Vector2i &basis_y) {
        *this = {
            .x = basis_x.x * x + basis_y.x * y,
            .y = basis_x.y * x + basis_y.y * y
        };
        return *this;
    }
};



#endif