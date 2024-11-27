#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "lighting.h"
#include <iostream>

float lastX = 400, lastY = 300;
bool firstMouse = true;
float xpos, ypos;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (firstMouse) {
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }
    xpos = xposIn;
    ypos = yposIn;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    // GLuint texture = loadTexture("path_to_texture.jpg");

    Light light = {
        glm::vec3(1.2f, 1.0f, 2.0f),
        glm::vec3(0.2f),
        glm::vec3(0.5f),
        glm::vec3(1.0f)
    };

    float deltaTime = 0.0f, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float xOffset = xpos - lastX;
        float yOffset = lastY - ypos; // reversed
        lastX = xpos;
        lastY = ypos;
        camera.processMouseMovement(xOffset, yOffset);

        camera.processKeyboardInput(window, deltaTime);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        shader.use();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f));
        setLightProperties(shader, light, camera.position);

        // Render objects here
        // glBindVertexArray(objectVAO);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
