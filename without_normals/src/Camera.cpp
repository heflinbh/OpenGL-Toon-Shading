#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

void Camera::MouseLook(int mouseX, int mouseY){
    // Taken heavily from https://learnopengl.com/Getting-started/Camera

    // Sets the initial mouse position. 
    // The camera always moves based on a comparision between old and new mouse positions.
    // So the first time through this loop, we get the initial baseline. 
    // After the first loop, this if-statement is no longer used. 
    if(firstMouse) {
        m_oldMousePosition.x = mouseX;
        m_oldMousePosition.y = mouseY;
        firstMouse = false;
        return;
    }

    // Get the difference between old and new mouse positions.
    float xoffset = mouseX - m_oldMousePosition.x;
    float yoffset = m_oldMousePosition.y - mouseY;
    // Then use the new mouse position as the old mouse position for the next time though this loop.
    m_oldMousePosition.x = mouseX;
    m_oldMousePosition.y = mouseY;

    // Adjust the offset by a sensitivity level so that subtle mouse movements don't make the camera go crazy.
    const float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Adjust the global yaw and pitch values by the offset.
    // Yaw is rotation about the y-plane of the camera (left/right motion).
    // Pitch is rotation about the x-plane of the camera (downward/upward motion)
    yaw += xoffset;
    pitch += yoffset;

    // To avoid spacial confusion, we don't want to be able to look past "straight up" or "straight down".
    // So limit the pitch to -90 <= pitch <= +90.
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    // Use the new values of pitch and yaw to calculate the new direction.
    // Normalize this new direction and set our viewDirection to this value.
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_viewDirection = glm::normalize(direction);
}

// OPTIONAL TODO: 
//               The camera could really be improved by
//               updating the eye position along the m_viewDirection.
//               Think about how you can do this for a better camera!

void Camera::MoveForward(float speed){
    m_eyePosition += speed * m_viewDirection;
}

void Camera::MoveBackward(float speed){
    m_eyePosition -= speed * m_viewDirection;
}

void Camera::MoveLeft(float speed){
    m_eyePosition -= glm::normalize(glm::cross(m_viewDirection, m_upVector)) * speed;
}

void Camera::MoveRight(float speed){
    m_eyePosition += glm::normalize(glm::cross(m_viewDirection, m_upVector)) * speed;
}

void Camera::MoveUp(float speed){
    m_eyePosition += speed * m_upVector;
}

void Camera::MoveDown(float speed){
    m_eyePosition -= speed * m_upVector;
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}


Camera::Camera(){
    std::cout << "(Constructor) Created a Camera!\n";
	// Position us at the origin.
    m_eyePosition = glm::vec3(0.0f,0.0f, 0.0f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    m_viewDirection = glm::vec3(0.84f,0.0f, -0.54f);
	// For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
