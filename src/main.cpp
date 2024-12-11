#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "lighting.h"
#include <iostream>

void checkOpenGLErrors(const char* function) {
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in " << function << ": " << error << std::endl;
        error = glGetError();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width > 0 && height > 0) {
        glViewport(0, 0, width, height);
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    camera->processMouseMovement(xposIn, yposIn);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    glfwSetWindowUserPointer(window, &camera);

    Shader planeShader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    float planeVertices[] = {
        -5.0f, 0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
         5.0f, 0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -5.0f, 0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         5.0f, 0.0f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         5.0f, 0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -5.0f, 0.0f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    };

    GLuint planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint planeTexture = loadTexture("assets/textures/rainbow.jpg");

    Light light;
    light.position = glm::vec3(2.0f, 2.0f, 2.0f);
    light.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float deltaTime = 0.0f, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.processKeyboardInput(window, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (width == 0 || height == 0) continue;

        planeShader.use();
        planeShader.setVec3("light.position", glm::vec3(camera.getViewMatrix() * glm::vec4(light.position, 1.0f)));
        planeShader.setVec3("light.ambient", light.ambient);
        planeShader.setVec3("light.diffuse", light.diffuse);
        planeShader.setVec3("light.specular", light.specular);
        planeShader.setFloat("material.shininess", 20.0f);
        planeShader.setVec3("viewPos", camera.position);

        planeShader.setMat4("view", camera.getViewMatrix());
        planeShader.setMat4("projection", glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f));

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        planeShader.setMat4("model", model);

        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeTexture);
        planeShader.setInt("material.diffuse", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}
