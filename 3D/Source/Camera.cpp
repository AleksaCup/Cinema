#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
      MovementSpeed(SPEED), 
      MouseSensitivity(SENSITIVITY), 
      Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

#include <glm/common.hpp> // za glm::clamp (ako već nemaš uključeno negde)

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;

    //limits in cinema room (added small buffer)
    const float WALL_X  = 4.0f;   // half width
    const float FRONT_Z = 8.0f;   // max forward
    const float BACK_Z  = -2.0f;  // behind screen
    const float FLOOR_Y = 0.5f;
    const float CEIL_Y  = 5.0f;

    const float CAMERA_RADIUS = 0.35f;

    // Clamp with buffer (inner box)
    Position.x = glm::clamp(Position.x, -WALL_X + CAMERA_RADIUS, WALL_X - CAMERA_RADIUS);
    Position.z = glm::clamp(Position.z,  BACK_Z + CAMERA_RADIUS, FRONT_Z - CAMERA_RADIUS);
    Position.y = glm::clamp(Position.y,FLOOR_Y + CAMERA_RADIUS,CEIL_Y - CAMERA_RADIUS);
}


void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // Constrain pitch to prevent screen flip
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // Calculate new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    
    // Recalculate Right and Up vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}