#include "camera.h"

float maxX = 10.0f, minX = -10.0f;
float maxY = 10.0f, minY = 0.0f;
float maxZ = 10.0f, minZ = -10.0f;

Camera::Camera(glm::vec3 startPos, glm::vec3 startUp, float startYaw, float startPitch)
    : position(startPos), up(startUp), yaw(startYaw), pitch(startPitch) {
    front = glm::vec3(0.0f, 0.0f, -1.0f); // Default front vector
    right = glm::normalize(glm::cross(front, up)); // Default right vector
    movementSpeed = 2.5f;
    mouseSensitivity = 0.1f;
    lastX = 400.0f;  // Initial mouse position X (center of the window)
    lastY = 300.0f;  // Initial mouse position Y (center of the window)
    firstMouse = true;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardInput(GLFWwindow* window, float deltaTime) {
    float velocity = movementSpeed * deltaTime * 2;
    glm::vec3 newPos = position;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        newPos += velocity * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        newPos -= velocity * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        newPos -= glm::normalize(glm::cross(front, up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        newPos += glm::normalize(glm::cross(front, up)) * velocity;

    // Simple collision detection: prevent camera from going out of bounds
    // Assuming the world is a box with limits in the X, Y, and Z axes.
    if (newPos.x > maxX || newPos.x < minX) 
        newPos.x = position.x;  // Prevent moving outside X bounds
    if (newPos.y > maxY || newPos.y < minY) 
        newPos.y = position.y;  // Prevent moving outside Y bounds
    if (newPos.z > maxZ || newPos.z < minZ) 
        newPos.z = position.z;  // Prevent moving outside Z bounds

    // Update the camera's position if no collision occurred
    position = newPos;
}


void Camera::processMouseMovement(float xpos, float ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;  // Calculate the change in X
    float yoffset = lastY - ypos;  // Calculate the change in Y (invert for correct movement)
    lastX = xpos;  // Update last mouse X position
    lastY = ypos;  // Update last mouse Y position

    xoffset *= mouseSensitivity;  // Apply sensitivity
    yoffset *= mouseSensitivity;  // Apply sensitivity

    yaw += xoffset;  // Update yaw (horizontal angle)
    pitch += yoffset;  // Update pitch (vertical angle)

    if (pitch > 89.0f) pitch = 89.0f;  // Limit pitch to avoid flipping
    if (pitch < -89.0f) pitch = -89.0f;

    // Normalize yaw to keep it within 0–360 degrees
    if (yaw > 360.0f) yaw -= 360.0f;
    if (yaw < 0.0f) yaw += 360.0f;

    // Update the front vector based on new yaw and pitch
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);  // Recalculate the normalized front vector
    right = glm::normalize(glm::cross(front, up));  // Recalculate the right vector
}