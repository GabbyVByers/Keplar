
/*
    Keyboard.cpp
*/

#include "../Rendering.h"

namespace Vi {
    
    /* Public */

    bool Keyboard::pressed(int key, int action) const {
        for (const KeyboardEvent& event : keyboard_events) {
            if (key == event.key) {
                if (action == event.action) {
                    return true;
                }
            }
        }
        return false;
    }

    bool Keyboard::pressing(int key) const {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window == nullptr) {
            Log::error("GLFW context does not exist");
            std::terminate();
        }
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool Keyboard::imgui_captured() const {
        ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureKeyboard;
    }

    /* Private */

    void Keyboard::reset() {
        keyboard_events.clear();
    }
}

