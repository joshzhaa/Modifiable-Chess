#ifndef __VECTOR_H__
#define __VECTOR_H__

//Vector representing directions on board
//confusing name near-collision with std::vector
//the name is much better for this struct though
struct Vector {
    int x;
    int y;
};
Vector operator* (const Vector& vec, int scalar);
Vector operator* (int scalar, const Vector& vec);
Vector operator/ (const Vector& vec, int scalar);
Vector operator+ (const Vector& a, const Vector& b);
Vector operator- (const Vector& a, const Vector& b);
Vector& operator+= (Vector& a, const Vector& b);

#endif //__VECTOR_H__
