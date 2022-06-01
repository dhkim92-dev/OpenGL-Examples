#ifndef __GL_EXAMPLE_CAMERA_CPP__
#define __GL_EXAMPLE_CAMERA_CPP__

#include "camera.h"
#include <iostream>

using namespace std;
using namespace glm;

ECamera::ECamera(){
	pos = vec3(0, 0, 1);
	front = vec3(0, 0, -1);
	world_up = vec3(0, 1, 0);
	yaw = YAW;
	pitch = PITCH;
	m_sensi = SENSITIVITY;
	zoom = ZOOM;
	m_speed = SPEED;

	update();
}

ECamera::ECamera(vec3 pos, 
	vec3 front, 
	vec3 world_up,
	float yaw, float pitch)
:	pos(pos), front(front), world_up(world_up), yaw(yaw), pitch(pitch)
{
	m_sensi = SENSITIVITY;
	zoom = ZOOM;
	m_speed = SPEED;
	update();
}

mat4 ECamera::getView()
{
	return glm::lookAt(pos, pos+front, up);
}

void ECamera::update()
{
	vec3 _front = vec3(
		cos(glm::radians(yaw))* cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	);
	front = glm::normalize(_front);
	right = glm::normalize( glm::cross(front, world_up));
	up = glm::normalize(glm::cross(right, front));
}

void ECamera::mouseMoveHandler(float x, float y){
	x *= m_sensi;
	y *= m_sensi;

	yaw   += x;
	pitch -= y;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Update Front, Right and Up Vectors using the updated Euler angles
	update();
}

void ECamera::mouseScrollHandler(float y){
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= y;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void ECamera::keyInputHandler(Camera_Movement input, 
float delta) {
	float velocity = m_speed * delta;
	switch(input){
		case FORWARD:
			pos += front * velocity;
			break;
		case BACKWARD : 
			pos -= front * velocity;
			break;
		case LEFT :
			pos -= right * velocity;
			break;
		case RIGHT :
			pos += right * velocity;
			break;
	}
}


#endif