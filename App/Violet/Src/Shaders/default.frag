
#version 330 core

in vec4 vertColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main() {
    FragColor = texture(ourTexture, texCoord) * vertColor;
}

