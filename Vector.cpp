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
//also known as maximum norm
int inf_norm(const Vector& vec) {
    Vector absolute{vec.x < 0 ? -1 * vec.x : vec.x, vec.y < 0 ? -1 * vec.y : vec.y};
    return absolute.x > absolute.y ? absolute.x : absolute.y;
}
