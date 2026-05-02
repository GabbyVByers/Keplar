
/*
    main.cpp
*/

#include "Map.h"

int main() {

    Vi::Window window = Vi::Window("My App", 1920, 1080);

    App::Map map{};

    while (window.is_open()) {
        window.poll_events();
        window.clear(Vi::Color::blue());
        map.control_camera(window);
        map.render(window);
        window.display();

        std::cout << map.scale << "\n";
    }

    return 0;
}

