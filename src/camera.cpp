#include "camera.h"

Camera::Camera(glm::vec3 startPos, glm::vec3 startUp, float startYaw, float startPitch)
    : position(startPos), up(startUp), yaw(startYaw), pitch(startPitch) {
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::normalize(glm::cross(front, up));
    movementSpeed = 2.5f;
    mouseSensitivity = 0.05f;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardInput(GLFWwindow* window, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += velocity * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= velocity * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(front, up)) * velocity;
}

void Camera::processMouseMovement(float xpos, float ypos) {
    float xoffset = xpos - 400.0f; // center of screen
    float yoffset = 300.0f - ypos;
    yaw += xoffset * mouseSensitivity;
    pitch += yoffset * mouseSensitivity;
    
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, up));
}
