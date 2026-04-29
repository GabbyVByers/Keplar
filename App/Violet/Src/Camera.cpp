
/*
    Camera.cpp
*/

#include "../Rendering.h"
#include <numbers>

namespace Vi {

    Vec3d Camera::forward() const {
        Vec3d forward = Vec3d::zneg();
        return Vec3d::rotate(forward, orientation);
    }

    Vec3d Camera::up() const {
        Vec3d up = Vec3d::ypos();
        return Vec3d::rotate(up, orientation);
    }

    Vec3d Camera::right() const {
        Vec3d right = Vec3d::xpos();
        return Vec3d::rotate(right, orientation);
    }

    Mat4 Camera::view_matrix() const {
        Mat4 translation_matrix_inverse = Mat4::translation_matrix(position * -1.0);
        Mat4 rotation_matrix_inverse = Mat4::rotation_matrix(orientation.complex_conjugate());
        return rotation_matrix_inverse * translation_matrix_inverse;
    }

    Mat4 Camera::projection_matrix(const Vec2i screen_size) const {
        if (is_inf) {
            double fov_rad = (fov_deg / 180.0) * std::numbers::pi;
            double f = 1.0 / tan(fov_rad * 0.5);
            double a = (double)screen_size.x / (double)screen_size.y;
            double A = f / a;
            double D = -2.0 * near;
            return Mat4(
                A, 0, 0, 0,
                0, f, 0, 0,
                0, 0,-1, D,
                0, 0,-1, 0
            );
        }
        else {
            double fov_rad = (fov_deg / 180.0) * std::numbers::pi;
            double f = 1.0 / tan(fov_rad * 0.5);
            double a = (double)screen_size.x / (double)screen_size.y;
            double A = f / a;
            double B = (far + near) / (near - far);
            double C = (2.0 * far * near) / (near - far);
            return Mat4(
                A, 0, 0, 0,
                0, f, 0, 0,
                0, 0, B, C,
                0, 0,-1, 0
            );
        }
    }
}

