
/*
    Mesh.cpp
*/

#include "../Rendering.h"

#define STB_IMAGE_IMPLEMENTATION /* #define STB_IMAGE_IMPLEMENTATION must only exist in exactly one translation unit (*.h or *.cpp) */
#include <stb/stb_image.h>
#include <fstream>
#include <sstream>

namespace Vi {

    /* Special Member Functions */

    Mesh::Mesh() {
        GLFWwindow* window = glfwGetCurrentContext();
        if (window == nullptr) {
            Log::error("Cannot create Mesh object, GLFW context does not exist");
            std::terminate();
        }
        texture();
        material();
    }

    Mesh::Mesh(const Mesh& other) {
        texture(other.texture_path);
        material(other.shader_path, other.primitive);
        scale = other.scale;
        position = other.position;
        orientation = other.orientation;
        vertices = other.vertices;
    }

    Mesh::Mesh(Mesh&& other) noexcept {
        texture_path = other.texture_path;
        texture_id = other.texture_id;
        shader_path = other.shader_path;
        primitive = other.primitive;
        vao = other.vao;
        vbo = other.vbo;
        shader = other.shader;
        scale = other.scale;
        position = other.position;
        orientation = other.orientation;
        vertices = other.vertices;

        other.texture_id = NULL;
        other.primitive = NULL;
        other.vao = NULL;
        other.vbo = NULL;
        other.shader = NULL;
    }

    Mesh& Mesh::operator = (const Mesh& other) {
        if (this == &other)
            return *this;

        texture(other.texture_path);
        material(other.shader_path, other.primitive);
        scale = other.scale;
        position = other.position;
        orientation = other.orientation;
        vertices = other.vertices;

        return *this;
    }

    Mesh& Mesh::operator = (Mesh&& other) noexcept {
        if (this == &other)
            return *this;

        destroy_texture();
        destroy_material();
        texture_path = other.texture_path;
        texture_id = other.texture_id;
        shader_path = other.shader_path;
        primitive = other.primitive;
        vao = other.vao;
        vbo = other.vbo;
        shader = other.shader;
        scale = other.scale;
        position = other.position;
        orientation = other.orientation;
        vertices = other.vertices;

        other.texture_id = NULL;
        other.primitive = NULL;
        other.vao = NULL;
        other.vbo = NULL;
        other.shader = NULL;

        return *this;
    }

    Mesh::~Mesh() {
        destroy_texture();
        destroy_material();
    }

    /* Texture */

    void Mesh::texture(const std::string& path) {
        texture_path = path;
        destroy_texture();

        if (path == "") {
            unsigned char white[] = { 255, 255, 255, 255 };

            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGBA, 1, 1, NULL, GL_RGBA, GL_UNSIGNED_BYTE, white);
            return;
        }

        int width, height;
        int num_color_channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &num_color_channels, NULL);
        if (data == NULL) {
            Log::error("Failed to load image: " + path);
            std::terminate();
        }

        GLenum format = NULL;
        if (num_color_channels == 1) format = GL_RED;
        if (num_color_channels == 3) format = GL_RGB;
        if (num_color_channels == 4) format = GL_RGBA;
        if (format == NULL) {
            Log::error("Could not parse image format");
            std::terminate();
        }

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, NULL, format, width, height, NULL, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    void Mesh::destroy_texture() {
        if (texture_id != NULL) {
            glDeleteTextures(1, &texture_id);
            texture_id = NULL;
        }
    }

    /* Material */

    void Mesh::material(const std::string& path, const GLenum type) {
        shader_path = path;
        primitive = type;
        destroy_material();

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        auto load = [](std::string path) -> std::string {
            std::ifstream file(path);
            if (!file) {
                Log::error("Could not load file: " + path);
                std::terminate();
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        };

        std::string root = __FILE__;
        std::string vert_source = load(root + "../../Shaders/" + path + ".vert");
        std::string frag_source = load(root + "../../Shaders/" + path + ".frag");
        const char* vert_c_str = vert_source.c_str();
        const char* frag_c_str = frag_source.c_str();

        GLuint vert_program = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_program, 1, &vert_c_str, nullptr);
        glCompileShader(vert_program);
        GLint success;
        glGetShaderiv(vert_program, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            Log::error("Failed to compile vertex shader");
            std::terminate();
        }

        GLuint frag_program = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_program, 1, &frag_c_str, nullptr);
        glCompileShader(frag_program);
        glGetShaderiv(frag_program, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            Log::error("Failed to compile fragment shader");
            std::terminate();
        }

        shader = glCreateProgram();
        glAttachShader(shader, vert_program);
        glAttachShader(shader, frag_program);
        glLinkProgram(shader);
        glDeleteShader(vert_program);
        glDeleteShader(frag_program);
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            Log::error("Failed to link shader program");
            std::terminate();
        }
    }

    void Mesh::destroy_material() {
        if (vao != NULL) {
            glDeleteVertexArrays(1, &vao);
            vao = NULL;
        }
        if (vbo != NULL) {
            glDeleteBuffers(1, &vbo);
            vbo = NULL;
        }
        if (shader != NULL) {
            glDeleteProgram(shader);
            shader = NULL;
        }
    }

    /* Other */

    Mat4 Mesh::model_matrix() const {
        Mat4 scalar_matrix = Mat4::scalar_matrix(scale);
        Mat4 translation_matrix = Mat4::translation_matrix(position);
        Mat4 rotation_matrix = Mat4::rotation_matrix(orientation);
        return scalar_matrix * translation_matrix * rotation_matrix;
    }

    void Mesh::paint(const Color& color) {
        for (Vertex& vertex : vertices) {
            vertex.color = color;
        }
    }
}

