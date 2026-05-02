
/*
    Rendering.h
*/

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Logging.h"
#include "Math.h"
#include <iostream>
#include <vector>
#include <string>

#define Vi Vi
namespace Vi {

    class Color;
    class Vertex;
    class Camera;
    class Mesh;
    class Shapes;
    class Keyboard;
    class Mouse;
    class Window;

    class Color {
    public:
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 1.0f;
        static Color random();
        static Color white();
        static Color black();
        static Color red();
        static Color green();
        static Color blue();
        static Color cyan();
        static Color purple();
        static Color yellow();
    };

    class Vertex {
    public:
        Vec3f position = Vec3f();
        Color color = Color::white();
        Vec2f tex_coord = Vec2f();
    };

    class Camera {
    public:
        bool is_inf = false;
        double far = 100.0f;
        double near = 0.1f;
        double fov_deg = 70.0f;
        Vec3d position = Vec3d();
        Quat orientation = Quat();
        Vec3d forward() const;
        Vec3d up() const;
        Vec3d right() const;
        Mat4 view_matrix() const;
        Mat4 projection_matrix(const Vec2i) const;
    };

    class Mesh {
    public:
        Mesh();
        Mesh(const Mesh&);
        Mesh(Mesh&&) noexcept;
        Mesh& operator = (const Mesh&);
        Mesh& operator = (Mesh&&) noexcept;
        ~Mesh();
        double scale = 1.0;
        Vec3d position = Vec3d();
        Quat orientation = Quat();
        std::vector<Vertex> vertices = {};
        void paint(const Color&);
        void texture(const std::string& = "");
        void material(const std::string& = "default", const GLenum = GL_TRIANGLES);
        Mat4 model_matrix() const;
    private:
        friend Window;
        std::string texture_path = "";
        GLuint texture_id = NULL;
        std::string shader_path = "";
        GLuint primitive = NULL;
        GLuint vao = NULL;
        GLuint vbo = NULL;
        GLuint shader = NULL;
        void destroy_texture();
        void destroy_material();
    };

    class Shapes {
    public:
        static Mesh sphere(unsigned int);
        static Mesh cube(unsigned int);
    private:
        Shapes() = delete;
        Shapes(const Shapes&) = delete;
        Shapes(Shapes&&) noexcept = delete;
    };

    class Keyboard {
    public:
        bool pressed(int, int) const;
        bool pressing(int) const;
        bool imgui_captured() const;
    private:
        friend Window;
        Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        Keyboard(Keyboard&&) noexcept = delete;
        ~Keyboard() = default;
        struct KeyboardEvent { int key; int scancode; int action; int mods; };
        std::vector<KeyboardEvent> keyboard_events = {};
        void reset();
    };

    class Mouse {
    public:
        Vec2d position() const;
        Vec2d velocity() const;
        bool pressing(int) const;
        bool clicked(int, int) const;
        double scroll() const;
        bool imgui_captured() const;
    private:
        friend Window;
        Mouse() = default;
        Mouse(const Mouse&) = delete;
        Mouse(Mouse&&) noexcept = delete;
        ~Mouse() = default;
        struct MouseEvent { int button; int action; int mods; };
        struct ScrollEvent { double xoffset; double yoffset; };
        Vec2d pos = Vec2d();
        Vec2d vel = Vec2d();
        std::vector<MouseEvent> mouse_events = {};
        std::vector<ScrollEvent> scroll_events = {};
        void reset();
    };

    class Window {
    public:
        Window(std::string, int, int);
        ~Window();
        Vec2i size();
        void vsync(bool);
        bool is_open();
        float imgui_scale(float = 0.0f);
        void poll_events();
        void clear(const Color&);
        void draw(const Mesh&, const Camera&);
        void display();
        static Mouse& mouse();
        static Keyboard& keyboard();
    private:
        Window(const Window&) = delete;
        Window(Window&&) noexcept = delete;
        static void callback_window_resize(GLFWwindow*, int, int);
        static void callback_keyboard(GLFWwindow*, int, int, int, int);
        static void callback_mouse(GLFWwindow*, int, int, int);
        static void callback_mousescroll(GLFWwindow*, double, double);
    };
}

