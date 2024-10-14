// 3D translation + scaling + shear + rotation

#ifndef TRANSFORM_3D_H
#define TRANSFORM_3D_H

#include "../math/vector3.h"

struct Transform3D {    
    Vector3 basis_x; 
    Vector3 basis_y; 
    Vector3 basis_z; 
    Vector3 origin_offset; 

    Transform3D &operator*=(Transform3D &input) const {   // input is transformed by this
        input.basis_x.transformed_by(basis_x, basis_y, basis_z);
        input.basis_y.transformed_by(basis_x, basis_y, basis_z);
        input.basis_z.transformed_by(basis_x, basis_y, basis_z);

        input.origin_offset.transformed_by(basis_x, basis_y, basis_z);
        input.origin_offset += origin_offset;
        return input;
    }

    Transform3D operator*(const Transform3D &right) const { // right is transformed by left
        Transform3D result = right;
        *this *= result;
        return result;  // copy elision do things
    }


    Vector3 &operator*=(Vector3 &input) const {   // vector is transformed by this
        input.transformed_by(basis_x, basis_y, basis_z);
        input += origin_offset;
        return input;
    }

    Vector3 operator*(const Vector3 &right) const { // right is transformed by left
        Vector3 result = right;
        *this *= result;
        return result;  // copy elision do things
    }
};


#endif