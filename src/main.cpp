#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "lighting.h"
#include <iostream>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const char* WINDOW_NAME = "Louvre Simulation";

struct ApplicationState {
    Camera camera;
    Shader shader;
    // Ceiling
    GLuint ceilingVAO;
    GLuint ceilingVBO;
    GLuint ceilingTexture;
    // Floor
    GLuint planeVAO;
    GLuint planeVBO;
    GLuint planeTexture;
    // Walls
    GLuint wallVAO;
    GLuint wallVBO;
    GLuint wallTexture;
    // Lighting
    Light light;
    DirectionalLight dirLight;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    ApplicationState() : camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                        shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl") {}
};

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

GLFWwindow* initializeWindow() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (!window) {
        std::cerr << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return nullptr;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    return window;
}

void setupGeometry(ApplicationState& state) {
    float ceilingVertices[] = {
        -10.0f, 10.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        10.0f, 10.0f, -10.0f,  0.0f, 1.0f, 0.0f,   5.0f, 0.0f,
        10.0f, 10.0f,  10.0f,  0.0f, 1.0f, 0.0f,   5.0f, 5.0f,
        -10.0f, 10.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        10.0f, 10.0f,  10.0f,  0.0f, 1.0f, 0.0f,   5.0f, 5.0f,
        -10.0f, 10.0f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 5.0f
    };

    float planeVertices[] = {
        -10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   10.0f, 10.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,   10.0f, 10.0f,
         10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    };

    float wallVertices[] = {
        // Front wall
        -10.0f,  0.0f, -10.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
        10.0f,   0.0f, -10.0f,  0.0f, 0.0f, 1.0f,   5.0f, 0.0f,  // bottom right
        10.0f,   10.0f, -10.0f,  0.0f, 0.0f, 1.0f,   5.0f, 10.0f, // top right
        -10.0f,  0.0f, -10.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
        10.0f,   10.0f, -10.0f,  0.0f, 0.0f, 1.0f,   5.0f, 10.0f, // top right
        -10.0f,  10.0f, -10.0f,  0.0f, 0.0f, 1.0f,   0.0f, 10.0f, // top left

        // Back wall
        -10.0f,  0.0f, 10.0f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        10.0f,   0.0f, 10.0f,   0.0f, 0.0f, -1.0f,  5.0f, 0.0f,
        10.0f,   10.0f, 10.0f,   0.0f, 0.0f, -1.0f,  5.0f, 10.0f,
        -10.0f,  0.0f, 10.0f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        10.0f,   10.0f, 10.0f,   0.0f, 0.0f, -1.0f,  5.0f, 10.0f,
        -10.0f,  10.0f, 10.0f,   0.0f, 0.0f, -1.0f,  0.0f, 10.0f,

        // Left wall
        -10.0f,  0.0f,  10.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -10.0f,  0.0f, -10.0f,  1.0f, 0.0f, 0.0f,   5.0f, 0.0f,
        -10.0f,  10.0f, -10.0f,  1.0f, 0.0f, 0.0f,   5.0f, 10.0f,
        -10.0f,  0.0f,  10.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -10.0f,  10.0f, -10.0f,  1.0f, 0.0f, 0.0f,   5.0f, 10.0f,
        -10.0f,  10.0f,  10.0f,  1.0f, 0.0f, 0.0f,   0.0f, 10.0f,

        // Right wall
        10.0f,  0.0f,  10.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        10.0f,  0.0f, -10.0f,   -1.0f, 0.0f, 0.0f,  5.0f, 0.0f,
        10.0f,  10.0f, -10.0f,   -1.0f, 0.0f, 0.0f,  5.0f, 10.0f,
        10.0f,  0.0f,  10.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        10.0f,  10.0f, -10.0f,   -1.0f, 0.0f, 0.0f,  5.0f, 10.0f,
        10.0f,  10.0f,  10.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 10.0f,
    };

    // Setup ceiling VAO/VBO
    GLuint ceilingVAO, ceilingVBO;
    glGenVertexArrays(1, &ceilingVAO);
    glGenBuffers(1, &ceilingVBO);

    glBindVertexArray(ceilingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ceilingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ceilingVertices), ceilingVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Setup floor VAO/VBO
    glGenVertexArrays(1, &state.planeVAO);
    glGenBuffers(1, &state.planeVBO);
    
    glBindVertexArray(state.planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, state.planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Setup wall VAO/VBO
    glGenVertexArrays(1, &state.wallVAO);
    glGenBuffers(1, &state.wallVBO);
    
    glBindVertexArray(state.wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, state.wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void setupLighting(ApplicationState& state) {
    state.light.position = glm::vec3(2.0f, 2.0f, 2.0f);
    state.light.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    state.light.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
    state.light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    state.dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    state.dirLight.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
    state.dirLight.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
    state.dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void render(GLFWwindow* window, ApplicationState& state) {
    float currentFrame = glfwGetTime();
    state.deltaTime = currentFrame - state.lastFrame;
    state.lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    state.camera.processKeyboardInput(window, state.deltaTime);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (width == 0 || height == 0) return;

    state.shader.use();
    
    // Set lighting uniforms
    state.shader.setVec3("light.position", state.light.position);
    state.shader.setVec3("light.ambient", state.light.ambient);
    state.shader.setVec3("light.diffuse", state.light.diffuse);
    state.shader.setVec3("light.specular", state.light.specular);
    state.shader.setFloat("material.shininess", 32.0f);
    state.shader.setVec3("viewPos", state.camera.position);

    state.shader.setVec3("dirLight.direction", state.dirLight.direction);
    state.shader.setVec3("dirLight.ambient", state.dirLight.ambient);
    state.shader.setVec3("dirLight.diffuse", state.dirLight.diffuse);
    state.shader.setVec3("dirLight.specular", state.dirLight.specular);

    // Set matrices
    glm::mat4 view = state.camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    state.shader.setMat4("view", view);
    state.shader.setMat4("projection", projection);

    // Render floor
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    state.shader.setMat4("model", model);
    
    glBindVertexArray(state.planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.planeTexture);
    state.shader.setInt("material.diffuse", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Render walls
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    state.shader.setMat4("model", model);
    
    glBindVertexArray(state.wallVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.wallTexture);
    state.shader.setInt("material.diffuse", 0);
    glDrawArrays(GL_TRIANGLES, 0, 24); // 4 walls * 2 triangles * 3 vertices

    // Render ceiling
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f)); // Position it above the walls
    state.shader.setMat4("model", model);

    glBindVertexArray(state.ceilingVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.wallTexture); // You can reuse the same texture or set a new one
    state.shader.setInt("material.diffuse", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2 triangles * 3 vertices

}

int main() {
    GLFWwindow* window = initializeWindow();
    if (!window) return -1;

    ApplicationState state;
    glfwSetWindowUserPointer(window, &state.camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    setupGeometry(state);
    setupLighting(state);

    // load textures 
    state.planeTexture = loadTexture("assets/textures/wood.jpg");
    state.wallTexture = loadTexture("assets/textures/gray.png");
    state.ceilingTexture = loadTexture("assets/textures/white_bordered.png");

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        render(window, state);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &state.planeVAO);
    glDeleteBuffers(1, &state.planeVBO);
    glfwTerminate();
    return 0;
}