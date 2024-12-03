#version 330 core

layout (location = 0) in vec3 aPos;       // Vertex position
layout (location = 1) in vec3 aNormal;    // Vertex normal
layout (location = 2) in vec2 aTexCoords; // Texture coordinates

out vec3 FragPos;      // Position of the fragment in world space
out vec3 Normal;       // Normal vector in world space
out vec2 TexCoords;    // Texture coordinates passed to the fragment shader

uniform mat4 model;      // Model transformation matrix
uniform mat4 view;       // View transformation matrix
uniform mat4 projection; // Projection transformation matrix

void main() {
    // Transform vertex position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normal to world space using inverse transpose of the model matrix
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass the texture coordinates to the fragment shader
    TexCoords = aTexCoords;

    // Apply the view and projection transformations to the vertex position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
