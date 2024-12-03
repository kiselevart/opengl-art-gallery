#include "plane.h"
#include "texture.h"
#include <iostream>

float planeVertices[] = {
    -5.0f, 0.0f,  5.0f,  0.0f, 1.0f,
     5.0f, 0.0f,  5.0f,  1.0f, 1.0f,
    -5.0f, 0.0f, -5.0f,  0.0f, 0.0f,

     5.0f, 0.0f,  5.0f,  1.0f, 1.0f,
     5.0f, 0.0f, -5.0f,  1.0f, 0.0f,
    -5.0f, 0.0f, -5.0f,  0.0f, 0.0f,
};

void initPlane(GLuint& planeVAO, GLuint& planeVBO, GLuint& planeTexture, const std::string& texturePath) {
    std::cout << "Initializing plane..." << std::endl;

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    std::cout << "Generated VAO: " << planeVAO << ", VBO: " << planeVBO << std::endl;

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    std::cout << "Vertex data loaded into VBO." << std::endl;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    std::cout << "Position attribute enabled and configured." << std::endl;

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    std::cout << "Texture coordinate attribute enabled and configured." << std::endl;

    glBindVertexArray(0);
    std::cout << "Plane VAO configuration complete." << std::endl;

    std::cout << "Loading texture from path: " << texturePath << std::endl;
    planeTexture = loadTexture(texturePath);

    if (planeTexture == 0) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    } else {
        std::cout << "Texture loaded successfully. Texture ID: " << planeTexture << std::endl;
    }
}
