#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "lighting.h"
#include "plane.h"
#include <iostream>

float lastX = 400, lastY = 300;
bool firstMouse = true;
float xpos, ypos;

void checkOpenGLErrors(const char* function) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in " << function << ": " << error << std::endl;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    (void)window;
    if (firstMouse) {
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }
    xpos = xposIn;
    ypos = yposIn;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS requires this for core profile

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;  
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    // Check if OpenGL context was created successfully
    if (glGetString(GL_VERSION) == nullptr) {
        std::cerr << "Error: OpenGL context creation failed!" << std::endl;
        return -1;
    }

    // Log OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    GLuint planeVAO, planeVBO, planeTexture;
    initPlane(planeVAO, planeVBO, planeTexture, "assets/textures/wooden_floor.jpg");

    Light light = {
        glm::vec3(2.0f, 2.0f, 2.0f),  
        glm::vec3(0.3f, 0.3f, 0.3f),   // Ambient
        glm::vec3(0.7f, 0.7f, 0.7f),   // Diffuse
        glm::vec3(1.0f, 1.0f, 1.0f)    // Specular
    };

    float deltaTime = 0.0f, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        std::cout << "Rendering frame..." << std::endl; // tests if frame renders: can be uncommented later
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input and update camera
        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos;  // Reversed Y-axis
        lastX = xpos;
        lastY = ypos;
        camera.processMouseMovement(xOffset, yOffset);
        camera.processKeyboardInput(window, deltaTime);

        // quit by pressing esc key
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }        
        
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        shader.use();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f));
        setLightProperties(shader, light, camera.position);

        // Set model matrix for plane
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));  // Adjust plane's position
        shader.setMat4("model", model);

        // Render plane
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
