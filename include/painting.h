#ifndef PAINTING_H
#define PAINTING_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h" // Assuming you have a Shader class defined

class Painting {
private:
    GLuint texture;
    glm::vec3 position;
    glm::vec2 size;
    
    // OpenGL buffer and array objects
    GLuint VAO, VBO, EBO;

    // Private method to set up geometry
    void setupGeometry();

public:
    // Constructor
    Painting(const char* texturePath, const glm::vec3& pos, const glm::vec2& dimensions);
    
    // Destructor
    ~Painting();

    // Draw method
    void draw(Shader& shader);

    // Optional: Getter methods if you want to access painting properties
    glm::vec3 getPosition() const { return position; }
    glm::vec2 getSize() const { return size; }
};

#endif // PAINTING_H