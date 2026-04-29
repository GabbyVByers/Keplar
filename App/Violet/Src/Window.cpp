
/*
    Window.cpp
*/

#include "../Rendering.h"

namespace Vi {

    /* Public */

    Window::Window(std::string title, int width, int height) {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window != nullptr) {
            Log::error("Cannot create Window object, GLFW context already exists");
            std::terminate();
        }

        GLuint status = glfwInit();
        if (status == NULL) {
            Log::error("Failed to initialize GLFW");
            std::terminate();
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (window == nullptr) {
            Log::error("Failed to create GLFW window");
            std::terminate();
        }

        glfwMakeContextCurrent(window);
        window = glfwGetCurrentContext();
        if (window == nullptr) {
            Log::error("Failed to make GLFW current context");
            std::terminate();
        }

        status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (status == NULL) {
            Log::error("Failed to load GLAD");
            std::terminate();
        }

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glfwSetFramebufferSizeCallback(window, callback_window_resize);
        glfwSetKeyCallback(window, callback_keyboard);
        glfwSetMouseButtonCallback(window, callback_mouse);
        glfwSetScrollCallback(window, callback_mousescroll);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = 1.0f;
        io.IniFilename = nullptr;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    Window::~Window() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(glfwGetCurrentContext());
        glfwTerminate();
    }

    Vec2i Window::size() {
        GLFWwindow* window = glfwGetCurrentContext();
        int x, y; glfwGetFramebufferSize(window, &x, &y);
        return Vec2i(x, y);
    }

    void Window::vsync(bool sync) {
        glfwSwapInterval((int)sync);
    }

    bool Window::is_open() {
        GLFWwindow* window = glfwGetCurrentContext();
        return !glfwWindowShouldClose(window);
    }

    float Window::imgui_scale(float scale) {
        if (scale == 0.0f) { /* if no argument is given, by default scale = 0.0f, and we just return the current scale */
            ImGuiIO& io = ImGui::GetIO();
            return io.FontGlobalScale;
        }
        else {
            ImGuiIO& io = ImGui::GetIO();
            io.FontGlobalScale = scale;
            return io.FontGlobalScale;
        }
    }

    void Window::poll_events() {
        Mouse& mouse = Window::mouse();
        Keyboard& keyboard = Window::keyboard();
        mouse.reset();
        keyboard.reset();
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::clear(const Color& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::draw(const Mesh& mesh, const Camera& camera) {
        const GLuint vao = mesh.vao;
        const GLuint vbo = mesh.vbo;
        const GLuint shader = mesh.shader;
        const GLuint primitive = mesh.primitive;
        const GLuint texture = mesh.texture_id;
        const std::vector<Vertex>& vertices = mesh.vertices;

        if (vertices.size() == 0)
            return;

        glUseProgram(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shader, "ourTexture"), NULL);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        Mat4 model_matrix = mesh.model_matrix();
        Mat4 view_matrix = camera.view_matrix();
        Mat4 projection_matrix = camera.projection_matrix(size());
        Mat4 mvp = projection_matrix * view_matrix * model_matrix;
        Mat4f mvp_f = static_cast<Mat4f>(mvp);
        glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewProject"), 1, GL_TRUE, mvp_f.ptr());

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
        glDrawArrays(primitive, NULL, (GLsizei)vertices.size());
    }

    void Window::display() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLFWwindow* window = glfwGetCurrentContext();
        glfwSwapBuffers(window);
    }

    Mouse& Window::mouse() {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window == nullptr) {
            Log::error("Cannot invoke Mouse without an existing GLFW context.");
        }
        static Mouse mouse;
        return mouse;
    }

    Keyboard& Window::keyboard() {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window == nullptr) {
            Log::error("Cannot invoke Keyboard without an existing GLFW context.");
        }
        static Keyboard keyboard;
        return keyboard;
    }

    /* Private */

    void Window::callback_window_resize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Keyboard::KeyboardEvent event = {
            .key = key,
            .scancode = scancode,
            .action = action,
            .mods = mods,
        };
        Keyboard& keyboard = Window::keyboard();
        keyboard.keyboard_events.push_back(event);
    }

    void Window::callback_mouse(GLFWwindow* window, int button, int action, int mods) {
        Mouse::MouseEvent event = {
            .button = button,
            .action = action,
            .mods = mods
        };
        Mouse& mouse = Window::mouse();
        mouse.mouse_events.push_back(event);
    }

    void Window::callback_mousescroll(GLFWwindow* window, double xoffset, double yoffset) {
        Mouse::ScrollEvent event = {
            .xoffset = xoffset,
            .yoffset = yoffset
        };
        Mouse& mouse = Window::mouse();
        mouse.scroll_events.push_back(event);
    }
}

