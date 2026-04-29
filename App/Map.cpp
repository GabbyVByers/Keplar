
/*
    Map.cpp
*/

#include "Map.h"

namespace App {

    Map::Map() {
        /* The Sun */
        Planet sun{};
        sun.name = "Sol";
        Vi::ID sun_id = planets.add(sun);
        Planet& s = planets.get_element_by_id(sun_id);
        s.self_id = sun_id;

        /* planets.txt */
        const std::string path = "App/Resources/planets.txt";
        std::ifstream file(path);
        if (!file) {
            Vi::Log::warning("Couldn't load " + path);
        }
        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        std::string token{};
        while (sstream >> token) {
            /* Planets */
            if (token == "Begin_Planets") {
                while (sstream >> token) {
                    if (token == "End_Planets")
                        break;
                    Planet planet{};
                    planet.name = token;
                    sstream >> token; if (token != "X") { Vi::Log::warning("Expected 'X' (" + token + ")"); }
                    sstream >> planet.init_pos.x;
                    sstream >> token; if (token != "Y") { Vi::Log::warning("Expected 'Y' (" + token + ")"); }
                    sstream >> planet.init_pos.y;
                    sstream >> token; if (token != "Z") { Vi::Log::warning("Expected 'Z' (" + token + ")"); }
                    sstream >> planet.init_pos.z;
                    sstream >> token; if (token != "VX") { Vi::Log::warning("Expected 'X' (" + token + ")"); }
                    sstream >> planet.init_vel.x;
                    sstream >> token; if (token != "VY") { Vi::Log::warning("Expected 'Y' (" + token + ")"); }
                    sstream >> planet.init_vel.y;
                    sstream >> token; if (token != "VZ") { Vi::Log::warning("Expected 'Z' (" + token + ")"); }
                    sstream >> planet.init_vel.z;
                    Vi::ID id = planets.add(planet);
                    Planet& p = planets.get_element_by_id(id);
                    p.self_id = id;
                    p.parent_id = sun_id;
                }
            }
            /* Moons */
            Vi::ID parent_planet_id = Vi::InvalidID;
            if (token == "Begin_Moons") {
                while (sstream >> token) {
                    if (token == "End_Moons")
                        break;
                    if (token == "Set_Parent") {
                        sstream >> token;
                        for (const Planet& planet : planets) {
                            if (planet.name == token) {
                                parent_planet_id = planet.self_id;
                            }
                        }
                        sstream >> token;
                    }
                    Planet moon{};
                    moon.name = token;
                    sstream >> token; if (token != "X") { Vi::Log::warning("Expected 'X' (" + token + ")"); }
                    sstream >> moon.init_pos.x;
                    sstream >> token; if (token != "Y") { Vi::Log::warning("Expected 'Y' (" + token + ")"); }
                    sstream >> moon.init_pos.y;
                    sstream >> token; if (token != "Z") { Vi::Log::warning("Expected 'Z' (" + token + ")"); }
                    sstream >> moon.init_pos.z;
                    sstream >> token; if (token != "VX") { Vi::Log::warning("Expected 'X' (" + token + ")"); }
                    sstream >> moon.init_vel.x;
                    sstream >> token; if (token != "VY") { Vi::Log::warning("Expected 'Y' (" + token + ")"); }
                    sstream >> moon.init_vel.y;
                    sstream >> token; if (token != "VZ") { Vi::Log::warning("Expected 'Z' (" + token + ")"); }
                    sstream >> moon.init_vel.z;
                    Vi::ID id = planets.add(moon);
                    Planet& m = planets.get_element_by_id(id);
                    m.self_id = id;
                    m.parent_id = parent_planet_id;
                }
            }
        }

        /* Populate Child IDs */
        for (Planet& parent : planets) {
            for (Planet& child : planets) {
                if (child.self_id == parent.self_id)
                    continue;
                if (child.parent_id == parent.self_id) {
                    parent.child_ids.push_back(child.self_id);
                }
            }
        }

        /* Convert KM and KM/S to M and M/S */
        for (Planet& planet : planets) {
            planet.init_pos *= 1000.0;
            planet.init_vel *= 1000.0;
        }

        /* Relative to Parents */
        for (Planet& planet : planets) {
            if (planet.parent_id == Vi::InvalidID)
                continue;
            if (planet.parent_id == Vi::InvalidID)
                continue;
        }

        { /* Debugging -- can be removed */
            for (const Planet& planet : planets) {
                Vi::Log::info(planet.name);
                Vi::Log::info("ID: " + std::to_string(planet.self_id));

                if (planet.parent_id != Vi::InvalidID) {
                    const Planet& parent = planets.get_element_by_id(planet.parent_id);
                    Vi::Log::info("Parent: " + parent.name);
                }

                if (planet.child_ids.size() > 0) {
                    std::string children_list = "Children: ";
                    for (Vi::ID child_id : planet.child_ids) {
                        const Planet& child = planets.get_element_by_id(child_id);
                        children_list += (child.name + " ");
                    }
                    Vi::Log::info(children_list);
                }

                Vi::Log::info("Px: " + std::to_string(planet.init_pos.x));
                Vi::Log::info("Py: " + std::to_string(planet.init_pos.y));
                Vi::Log::info("Pz: " + std::to_string(planet.init_pos.z));
                Vi::Log::info("Vx: " + std::to_string(planet.init_vel.x));
                Vi::Log::info("Vy: " + std::to_string(planet.init_vel.y));
                Vi::Log::info("Vz: " + std::to_string(planet.init_vel.z));
                std::cout << std::endl;
            }
        }

        /* Planet Meshies */
        for (Planet& planet : planets) {
            planet.mesh = Vi::Shapes::sphere(10);
        }
    }

    void Map::render(Vi::Window& window) {
        for (Planet& planet : planets) {
            window.draw(planet.mesh, camera);
        }
    }

    void Map::control_camera() {

    }
}

