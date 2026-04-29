
/*
    Shapes.cpp
*/

#include "../Rendering.h"
#include <ranges>
#include <numbers>

namespace Vi {

    Mesh Shapes::sphere(unsigned int divisions) {
		Mesh sphere_mesh;

		std::vector<Vec3d> points;
		size_t reserve_size = (((size_t)divisions + 1) * ((size_t)divisions + 1)) * 4;
		points.reserve(reserve_size);
		double stride = 2.0 / (double)(divisions + 1);
		for (size_t y = 0; y < (size_t)divisions + 1; y++) {
			for (size_t x = 0; x < (size_t)divisions + 1; x++) {
				Vec2d pos = Vec2d(-1.0 + (x * stride), 1.0 - (y * stride));
				points.push_back(Vec3d(pos.x, pos.y, 1.0));
				points.push_back(Vec3d(pos.x + stride, pos.y - stride, 1.0));
				points.push_back(Vec3d(pos.x + stride, pos.y, 1.0));
				points.push_back(Vec3d(pos.x, pos.y, 1.0));
				points.push_back(Vec3d(pos.x, pos.y - stride, 1.0));
				points.push_back(Vec3d(pos.x + stride, pos.y - stride, 1.0));
			}
		}

		for (size_t i = 0; i < 6; i++) {
			if (i < 4) {
				Vec3d up = Vec3d::ypos();
				double theta = std::numbers::pi / 2.0;
				auto face = points
					| std::views::transform([&](Vec3d p) { return Vec3d::normalize(p); })
					| std::views::transform([&](Vec3d p) { return Vec3d::rotate(p, up, i * theta); });
				for (const Vec3d& point : face) {
					sphere_mesh.vertices.push_back({
						.position = static_cast<Vec3f>(point),
						.color = Color::random(),
						.tex_coord = Vec2f()
					});
				}
			}
			else {
				Vec3d right = Vec3d::xpos();
				double half_pi = std::numbers::pi / 2.0;
				double theta = (i == 5) ? -half_pi : half_pi;
				auto face = points
					| std::views::transform([&](Vec3d p) { return Vec3d::normalize(p); })
					| std::views::transform([&](Vec3d p) { return Vec3d::rotate(p, right, theta); });
				for (const Vec3d& point : face) {
					sphere_mesh.vertices.push_back({
						.position = static_cast<Vec3f>(point),
						.color = Color::random(),
						.tex_coord = Vec2f()
					});
				}
			}
		}

		for (Vertex& vertex : sphere_mesh.vertices) {
			Vec3d pos = static_cast<Vec3d>(vertex.position);
			double pi = std::numbers::pi;
			vertex.tex_coord.x = (float)(0.5 + std::atan2(pos.x, pos.z) / (2.0 * pi));
			vertex.tex_coord.y = (float)((std::asin(pos.y) + (pi / 2.0)) / pi);
		}

		for (size_t i = 0; i < sphere_mesh.vertices.size(); i += 3) {
			auto width = [](
				const Vertex& a,
				const Vertex& b,
				const Vertex& c
				) -> float {
					float min = std::min(a.tex_coord.x, std::min(b.tex_coord.x, c.tex_coord.x));
					float max = std::max(a.tex_coord.x, std::max(b.tex_coord.x, c.tex_coord.x));
					return abs(min - max);
				};

			Vertex& a = sphere_mesh.vertices[i];
			Vertex& b = sphere_mesh.vertices[i + 1];
			Vertex& c = sphere_mesh.vertices[i + 2];

			if (width(a, b, c) > 0.5f) {
				if (a.tex_coord.x < 0.5f) a.tex_coord.x += 1.0f;
				if (b.tex_coord.x < 0.5f) b.tex_coord.x += 1.0f;
				if (c.tex_coord.x < 0.5f) c.tex_coord.x += 1.0f;
			}
		}

		return sphere_mesh;
    }

    Mesh Shapes::cube(unsigned int divisions) {
        return Mesh();
    }
}

