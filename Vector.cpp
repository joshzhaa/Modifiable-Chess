#include "Vector.h"

//Vector
Vector operator* (const Vector& vec, int scalar) {
    return {vec.x * scalar, vec.y * scalar};
}
Vector operator* (int scalar, const Vector& vec) {
    return vec * scalar;
}
Vector operator/ (const Vector& vec, int scalar) {
    return {vec.x / scalar, vec.y / scalar};
}
Vector operator+ (const Vector& a, const Vector& b) {
    return {a.x + b.x, a.y + b.y};
}
Vector operator- (const Vector& a, const Vector& b) {
    return {a.x - b.x, a.y - b.y};
}
Vector& operator+= (Vector& a, const Vector& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}
