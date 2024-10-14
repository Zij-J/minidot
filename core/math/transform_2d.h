// 2D translation + scaling + shear + rotation

#ifndef TRANSFORM_2D_H
#define TRANSFORM_2D_H

#include "../math/vector2.h"

struct Transform2D {    
    Vector2 basis_x; 
    Vector2 basis_y; 
    Vector2 origin_offset; 

    Transform2D &operator*=(Transform2D &input) const {   // input is transformed by this
        input.basis_x.transformed_by(basis_x, basis_y);
        input.basis_y.transformed_by(basis_x, basis_y);

        input.origin_offset.transformed_by(basis_x, basis_y);
        input.origin_offset += origin_offset;
        return input;
    }

    Transform2D operator*(const Transform2D &right) const { // right is transformed by left
        Transform2D result = right;
        *this *= result;
        return result;  // copy elision do things
    }

    Vector2 &operator*=(Vector2 &input) const {   // vector is transformed by this
        input.transformed_by(basis_x, basis_y);
        input += origin_offset;
        return input;
    }

    Vector2 operator*(const Vector2 &right) const { // right is transformed by left
        Vector2 result = right;
        *this *= result;
        return result;  // copy elision do things
    }
};


#endif