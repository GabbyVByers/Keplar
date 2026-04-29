
/*
    Map.h
*/

#pragma once

#include "Violet/Rendering.h"
#include "Violet/SiVector.h"
#include "Violet/Logging.h"
#include <string>
#include <sstream>
#include <fstream>

namespace App {

    class Map;

    class Planet {
    public:
        std::string name{};
        Vi::ID self_id = Vi::InvalidID;
        Vi::ID parent_id = Vi::InvalidID;
        std::vector<Vi::ID> child_ids{};
        Vi::Mesh mesh{};
    private:
        friend Map;
        Vi::Vec3d init_pos{};
        Vi::Vec3d init_vel{};
    };

    class Map {
    public:
        Map();
        void render(Vi::Window&);
        void control_camera();
        Vi::SiVector<Planet> planets{};
        Vi::Camera camera{};
    };
}

