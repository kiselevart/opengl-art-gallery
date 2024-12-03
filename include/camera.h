#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    
    // Member variables to track mouse movement
    float lastX, lastY;  // Last mouse position
    bool firstMouse;      // Flag to detect first mouse movement

    Camera(glm::vec3 startPos, glm::vec3 startUp, float startYaw, float startPitch);
    glm::mat4 getViewMatrix();
    void processKeyboardInput(GLFWwindow* window, float deltaTime);
    void processMouseMovement(float xpos, float ypos);
};

#endif
