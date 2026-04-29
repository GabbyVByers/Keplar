
/*
    Vector.cpp
*/

#include "../Math.h"
#include "../Logging.h"
#include <iostream>
#include <cmath>

namespace Vi {

    static void zero_error() {
        Log::error("Division by zero");
        std::terminate();
    }
    static void zero(int x)    { if (x == 0)    { zero_error(); } }
    static void zero(float x)  { if (x == 0.0f) { zero_error(); } }
    static void zero(double x) { if (x == 0.0)  { zero_error(); } }
    
    /* Vec2i */

    double Vec2i::hypot() const { return std::hypot(x, y); }
    Vec2i Vec2i::xpos() { return Vec2i( 1, 0); }
    Vec2i Vec2i::xneg() { return Vec2i(-1, 0); }
    Vec2i Vec2i::ypos() { return Vec2i( 0, 1); }
    Vec2i Vec2i::yneg() { return Vec2i( 0,-1); }
    int Vec2i::dot(const Vec2i& a, const Vec2i& b) { return (a.x * b.x) + (a.y * b.y); }
    int Vec2i::cross(const Vec2i& a, const Vec2i& b) { return (a.x * b.y) - (a.y * b.x); }
    Vec2i  Vec2i::operator +  (const Vec2i& vec) const { return Vec2i(x + vec.x, y + vec.y); }
    Vec2i  Vec2i::operator -  (const Vec2i& vec) const { return Vec2i(x - vec.x, y - vec.y); }
    Vec2i  Vec2i::operator *  (const int scale)  const { return Vec2i(x * scale, y * scale); }
    Vec2i  Vec2i::operator /  (const int scale)  const { zero(scale); return Vec2i(x / scale, y / scale); }
    Vec2i& Vec2i::operator += (const Vec2i& vec) { x += vec.x; y += vec.y; return *this; }
    Vec2i& Vec2i::operator -= (const Vec2i& vec) { x -= vec.x; y -= vec.y; return *this; }
    Vec2i& Vec2i::operator *= (const int scale)  { x *= scale; y *= scale; return *this; }
    Vec2i& Vec2i::operator /= (const int scale)  { zero(scale); x /= scale; y /= scale; return *this; }
    Vec2i::operator Vec2f() const { return Vec2f((float)x, (float)y); }
    Vec2i::operator Vec2d() const { return Vec2d((double)x, (double)y); }

    /* Vec2f */
    
    float Vec2f::hypot() const { return std::hypot(x, y); }
    Vec2f Vec2f::xpos() { return Vec2f( 1.0f, 0.0f); }
    Vec2f Vec2f::xneg() { return Vec2f(-1.0f, 0.0f); }
    Vec2f Vec2f::ypos() { return Vec2f( 0.0f, 1.0f); }
    Vec2f Vec2f::yneg() { return Vec2f( 0.0f,-1.0f); }
    float Vec2f::dot(const Vec2f& a, const Vec2f& b) { return (a.x * b.x) + (a.y * b.y); }
    float Vec2f::cross(const Vec2f& a, const Vec2f& b) { return (a.x * b.y) - (a.y * b.x); }
    Vec2f Vec2f::normalize(const Vec2f& vec) {
        float len = vec.hypot();
        return Vec2f(vec.x / len, vec.y / len);
    }
    Vec2f  Vec2f::operator +  (const Vec2f& vec)   const { return Vec2f(x + vec.x, y + vec.y); }
    Vec2f  Vec2f::operator -  (const Vec2f& vec)   const { return Vec2f(x - vec.x, y - vec.y); }
    Vec2f  Vec2f::operator *  (const float scale)  const { return Vec2f(x * scale, y * scale); }
    Vec2f  Vec2f::operator /  (const float scale)  const { zero(scale); return Vec2f(x / scale, y / scale); }
    Vec2f& Vec2f::operator += (const Vec2f& vec)  { x += vec.x; y += vec.y; return *this; }
    Vec2f& Vec2f::operator -= (const Vec2f& vec)  { x -= vec.x; y -= vec.y; return *this; }
    Vec2f& Vec2f::operator *= (const float scale) { x *= scale; y *= scale; return *this; }
    Vec2f& Vec2f::operator /= (const float scale) { zero(scale); x /= scale; y /= scale; return *this; }
    Vec2f::operator Vec2i() const { return Vec2i((int)x, (int)y); }
    Vec2f::operator Vec2d() const { return Vec2d((double)x, (double)y); }

    /* Vec2d */

    double Vec2d::hypot() const { return std::hypot(x, y); }
    Vec2d Vec2d::xpos() { return Vec2d( 1.0, 0.0); }
    Vec2d Vec2d::xneg() { return Vec2d(-1.0, 0.0); }
    Vec2d Vec2d::ypos() { return Vec2d( 0.0, 1.0); }
    Vec2d Vec2d::yneg() { return Vec2d( 0.0,-1.0); }
    double Vec2d::dot(const Vec2d& a, const Vec2d& b) { return (a.x * b.x) + (a.y * b.y); }
    double Vec2d::cross(const Vec2d& a, const Vec2d& b) { return (a.x * b.y) - (a.y * b.x); }
    Vec2d Vec2d::normalize(const Vec2d& vec) {
        double len = vec.hypot();
        return Vec2d(vec.x / len, vec.y / len);
    }
    Vec2d  Vec2d::operator +  (const Vec2d& vec)   const { return Vec2d(x + vec.x, y + vec.y); }
    Vec2d  Vec2d::operator -  (const Vec2d& vec)   const { return Vec2d(x - vec.x, y - vec.y); }
    Vec2d  Vec2d::operator *  (const double scale) const { return Vec2d(x * scale, y * scale); }
    Vec2d  Vec2d::operator /  (const double scale) const { zero(scale); return Vec2d(x / scale, y / scale); }
    Vec2d& Vec2d::operator += (const Vec2d& vec)   { x += vec.x; y += vec.y; return *this; }
    Vec2d& Vec2d::operator -= (const Vec2d& vec)   { x -= vec.x; y -= vec.y; return *this; }
    Vec2d& Vec2d::operator *= (const double scale) { x *= scale; y *= scale; return *this; }
    Vec2d& Vec2d::operator /= (const double scale) { zero(scale); x /= scale; y /= scale; return *this; }
    Vec2d::operator Vec2i() const { return Vec2i((int)x, (int)y); }
    Vec2d::operator Vec2f() const { return Vec2f((float)x, (float)y); }

    /* Vec3i */

    double Vec3i::hypot() const { return std::hypot(x, y, z); }
    Vec3i Vec3i::xpos() { return Vec3i( 1, 0, 0); }
    Vec3i Vec3i::xneg() { return Vec3i(-1, 0, 0); }
    Vec3i Vec3i::ypos() { return Vec3i( 0, 1, 0); }
    Vec3i Vec3i::yneg() { return Vec3i( 0,-1, 0); }
    Vec3i Vec3i::zpos() { return Vec3i( 0, 0, 1); }
    Vec3i Vec3i::zneg() { return Vec3i( 0, 0,-1); }
    int Vec3i::dot(const Vec3i& a, const Vec3i& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
    Vec3i Vec3i::cross(const Vec3i& a, const Vec3i& b) {
        return Vec3i(
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.x * b.y) - (a.y * b.x)
        );
    }
    Vec3i  Vec3i::operator +  (const Vec3i& vec) const { return Vec3i(x + vec.x, y + vec.y, z + vec.z); }
    Vec3i  Vec3i::operator -  (const Vec3i& vec) const { return Vec3i(x - vec.x, y - vec.y, z - vec.z); }
    Vec3i  Vec3i::operator *  (const int scale)  const { return Vec3i(x * scale, y * scale, z * scale); }
    Vec3i  Vec3i::operator /  (const int scale)  const { zero(scale); return Vec3i(x / scale, y / scale, z / scale); }
    Vec3i& Vec3i::operator += (const Vec3i& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
    Vec3i& Vec3i::operator -= (const Vec3i& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
    Vec3i& Vec3i::operator *= (const int scale)  { x *= scale; y *= scale; z *= scale; return *this; }
    Vec3i& Vec3i::operator /= (const int scale)  { zero(scale); x /= scale; y /= scale; z /= scale; return *this; }
    Vec3i::operator Vec3f() const { return Vec3f((float)x, (float)y, (float)z); }
    Vec3i::operator Vec3d() const { return Vec3d((double)x, (double)y, (double)z); }

    /* Vec3f */
    
    float Vec3f::hypot() const { return std::hypot(x, y, z); }
    Vec3f Vec3f::xpos() { return Vec3f( 1.0f, 0.0f, 0.0f); }
    Vec3f Vec3f::xneg() { return Vec3f(-1.0f, 0.0f, 0.0f); }
    Vec3f Vec3f::ypos() { return Vec3f( 0.0f, 1.0f, 0.0f); }
    Vec3f Vec3f::yneg() { return Vec3f( 0.0f,-1.0f, 0.0f); }
    Vec3f Vec3f::zpos() { return Vec3f( 0.0f, 0.0f, 1.0f); }
    Vec3f Vec3f::zneg() { return Vec3f( 0.0f, 0.0f,-1.0f); }
    float Vec3f::dot(const Vec3f& a, const Vec3f& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
    Vec3f Vec3f::cross(const Vec3f& a, const Vec3f& b) {
        return Vec3f(
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.x * b.y) - (a.y * b.x)
        );
    }
    Vec3f Vec3f::normalize(const Vec3f& vec) {
        float len = vec.hypot();
        return Vec3f(vec.x / len, vec.y / len, vec.z / len);
    }
    Vec3f Vec3f::rotate(const Vec3f& vec, const Vec3f& axis, const double theta) {
        Quat q_rot = Quat::rotation(axis, theta);
        return rotate(vec, q_rot);
    }
    Vec3f Vec3f::rotate(const Vec3f& vec, const Quat& q_rot) {
        Quat q_inv = q_rot.complex_conjugate();
        Quat p = Quat(0.0, (double)vec.x, (double)vec.y, (double)vec.z);
        Quat result = q_rot * p * q_inv;
        return Vec3f(
            (float)result.x,
            (float)result.y,
            (float)result.z
        );
    }
    Vec3f  Vec3f::operator +  (const Vec3f& vec)  const { return Vec3f(x + vec.x, y + vec.y, z + vec.z); }
    Vec3f  Vec3f::operator -  (const Vec3f& vec)  const { return Vec3f(x - vec.x, y - vec.y, z - vec.z); }
    Vec3f  Vec3f::operator *  (const float scale) const { return Vec3f(x * scale, y * scale, z * scale); }
    Vec3f  Vec3f::operator /  (const float scale) const { zero(scale); return Vec3f(x / scale, y / scale, z / scale); }
    Vec3f& Vec3f::operator += (const Vec3f& vec)  { x += vec.x; y += vec.y; z += vec.z; return *this; }
    Vec3f& Vec3f::operator -= (const Vec3f& vec)  { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
    Vec3f& Vec3f::operator *= (const float scale) { x *= scale; y *= scale; z *= scale; return *this; }
    Vec3f& Vec3f::operator /= (const float scale) { zero(scale); x /= scale; y /= scale; z /= scale; return *this; }
    Vec3f::operator Vec3i() const { return Vec3i((int)x, (int)y, (int)z); }
    Vec3f::operator Vec3d() const { return Vec3d((double)x, (double)y, (double)z); }
    
    /* Vec3d */

    double Vec3d::hypot() const { return std::hypot(x, y, z); }
    Vec3d Vec3d::xpos() { return Vec3d( 1.0, 0.0, 0.0); }
    Vec3d Vec3d::xneg() { return Vec3d(-1.0, 0.0, 0.0); }
    Vec3d Vec3d::ypos() { return Vec3d( 0.0, 1.0, 0.0); }
    Vec3d Vec3d::yneg() { return Vec3d( 0.0,-1.0, 0.0); }
    Vec3d Vec3d::zpos() { return Vec3d( 0.0, 0.0, 1.0); }
    Vec3d Vec3d::zneg() { return Vec3d( 0.0, 0.0,-1.0); }
    double Vec3d::dot(const Vec3d& a, const Vec3d& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
    Vec3d Vec3d::cross(const Vec3d& a, const Vec3d& b) {
        return Vec3d(
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.x * b.y) - (a.y * b.x)
        );
    }
    Vec3d Vec3d::normalize(const Vec3d& vec) {
        double len = vec.hypot();
        return Vec3d(vec.x / len, vec.y / len, vec.z / len);
    }
    Vec3d Vec3d::rotate(const Vec3d& vec, const Vec3d& axis, const double theta) {
        Quat q_rot = Quat::rotation(axis, theta);
        return rotate(vec, q_rot);
    }
    Vec3d Vec3d::rotate(const Vec3d& vec, const Quat& q_rot) {
        Quat q_inv = q_rot.complex_conjugate();
        Quat p = Quat(0.0, vec.x, vec.y, vec.z);
        Quat result = q_rot * p * q_inv;
        return Vec3d(
            result.x,
            result.y,
            result.z
        );
    }
    Vec3d  Vec3d::operator +  (const Vec3d& vec)   const { return Vec3d(x + vec.x, y + vec.y, z + vec.z); }
    Vec3d  Vec3d::operator -  (const Vec3d& vec)   const { return Vec3d(x - vec.x, y - vec.y, z - vec.z); }
    Vec3d  Vec3d::operator *  (const double scale) const { return Vec3d(x * scale, y * scale, z * scale); }
    Vec3d  Vec3d::operator /  (const double scale) const { zero(scale); return Vec3d(x / scale, y / scale, z / scale); }
    Vec3d& Vec3d::operator += (const Vec3d& vec)   { x += vec.x; y += vec.y; z += vec.z; return *this; }
    Vec3d& Vec3d::operator -= (const Vec3d& vec)   { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
    Vec3d& Vec3d::operator *= (const double scale) { x *= scale; y *= scale; z *= scale; return *this; }
    Vec3d& Vec3d::operator /= (const double scale) { zero(scale); x /= scale; y /= scale; z /= scale; return *this; }
    Vec3d::operator Vec3i() const { return Vec3i((int)x, (int)y, (int)z); }
    Vec3d::operator Vec3f() const { return Vec3f((float)x, (float)y, (float)z); }
}

