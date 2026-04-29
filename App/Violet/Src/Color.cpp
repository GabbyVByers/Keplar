
/*
    Color.cpp
*/

#include "../Rendering.h"
#include <cstdlib>

namespace Vi {
    
    Color Color::random() {
        return Color(
            (float)std::rand() / (float)RAND_MAX,
            (float)std::rand() / (float)RAND_MAX,
            (float)std::rand() / (float)RAND_MAX
        );
    }

    Color Color::white()  { return Color(1.0, 1.0, 1.0); }
    Color Color::black()  { return Color(0.0, 0.0, 0.0); }
    Color Color::red()    { return Color(1.0, 0.0, 0.0); }
    Color Color::green()  { return Color(0.0, 1.0, 0.0); }
    Color Color::blue()   { return Color(0.0, 0.0, 1.0); }
    Color Color::cyan()   { return Color(0.0, 1.0, 1.0); }
    Color Color::purple() { return Color(1.0, 0.0, 1.0); }
    Color Color::yellow() { return Color(1.0, 1.0, 0.0); }
}

