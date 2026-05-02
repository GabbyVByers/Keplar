
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

        enum class TYPE {
            NONE,
            SUN,
            PLANET,
            MOON
        };

        std::string name{};
        TYPE type = TYPE::NONE;
        Vi::ID self_id = Vi::InvalidID;
        Vi::ID parent_id = Vi::InvalidID;
        Vi::SiVector<Vi::ID> child_ids{};

        Vi::Mesh mesh{};

        Vi::Vec3d init_pos{};
        Vi::Vec3d init_vel{};
        double radius{};
        double mass{};
    };

    class Map {
    public:
        Map();
        void render(Vi::Window&);
        void control_camera(Vi::Window&);
        
        Vi::ID sun_id = Vi::InvalidID;
        Vi::SiVector<Planet> planets{};
        
        double scale = 1.0E-12;
        Vi::ID focus_id = Vi::InvalidID;
        Vi::Camera camera{};
    };
}

