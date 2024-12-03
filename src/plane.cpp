#include "plane.h"
#include "texture.h"

float planeVertices[] = {
    -5.0f, 0.0f,  5.0f,  0.0f, 1.0f,
     5.0f, 0.0f,  5.0f,  1.0f, 1.0f,
    -5.0f, 0.0f, -5.0f,  0.0f, 0.0f,

     5.0f, 0.0f,  5.0f,  1.0f, 1.0f,
     5.0f, 0.0f, -5.0f,  1.0f, 0.0f,
    -5.0f, 0.0f, -5.0f,  0.0f, 0.0f,
};

void initPlane(GLuint& planeVAO, GLuint& planeVBO, GLuint& planeTexture, const std::string& texturePath) {
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    planeTexture = loadTexture(texturePath);
}
