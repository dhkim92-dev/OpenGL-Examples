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
	m_zoom = ZOOM;
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
	m_zoom = ZOOM;
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
	if (pitch >= 89.0f)
		pitch = 90.0f;
	if (pitch <= -90.0f)
		pitch = -90.0f;

	// Update Front, Right and Up Vectors using the updated Euler angles
	update();
}

void ECamera::mouseScrollHandler(float y){
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= y;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void ECamera::keyInputHandler(Camera_Movement input, 
float delta) {
	float velocity = m_speed * delta;
	// cout << "m_speed : " << m_speed <<" input delta : " << delta  << " velocity : " << velocity << endl;
	// cout << "before key :: " << pos.x << ", " << pos.y << ", " << pos.z << endl; 
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
	// cout << "after key :: " << pos.x << ", " << pos.y << ", " << pos.z << endl; 
}

void ECamera::setPerspective(float fov, float aspect, float near, float far)
{
	proj = glm::perspective(glm::radians(fov), aspect, near, far );
}

mat4 ECamera::getPerspective(){
	return proj;
}

QuatCamera::QuatCamera()
{
	pos = glm::vec3(0.0, 0.0, 1.0f);
	orientation = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
	pitch = 0.0f;
	yaw = 0.0f;
	m_speed = SPEED;
	m_sensi = SENSITIVITY;
	m_zoom = ZOOM;

	setPerspective(45.0f, 16.0/9.0, 0.1f, 100.0f);
	update();
}

QuatCamera::QuatCamera(vec3 pos, float yaw, float pitch)
	: pos(pos), yaw(yaw), pitch(pitch)
{
	m_speed = SPEED;
	m_sensi = SENSITIVITY;
	m_zoom = ZOOM;


	setPerspective(45.0f, 16.0/9.0, 0.1f, 100.0f);
	update();
}

mat4 QuatCamera::getView()
{
	glm::quat view_direction = glm::normalize(glm::conjugate(orientation));
	glm::mat4 rotate = glm::mat4_cast(view_direction);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), -pos);

	return rotate * view;
}

mat4 QuatCamera::getPerspective(){
	return proj;
}

void QuatCamera::setPerspective(float fov, float aspect, float near, float far){
	proj = glm::perspective(glm::radians(fov), aspect, near, far);
}

void QuatCamera::update()
{
	//x axis rotate
	q_pitch = glm::angleAxis(glm::radians(pitch), vec3(1.0f, 0.0f, 0.0f));
	//y axis rotate
	q_yaw = glm::angleAxis(glm::radians(-yaw), vec3(0.0f, 1.0f, 0.0f));
	orientation = glm::normalize(q_yaw * q_pitch);
	front = orientation * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * glm::conjugate(orientation);
	// cout << "pos : " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	// cout << "orientation : " << orientation.x << ", " << orientation.y << ", " << orientation.z << endl;
	// cout << "front : " << front.x << ", " << front.y << ", " << front.z << endl;
}

void QuatCamera::mouseMoveHandler(float x, float y)
{
	x *= m_sensi;
	y *= m_sensi;

	pitch  -= y;
	yaw += x;

	cout << "pitch : " << pitch << endl;
	cout << "yaw : " << yaw << endl;

	if (pitch >= 90.0f)
		pitch = 90.0f;
	if (pitch <= -90.0f)
		pitch = -90.0f;

	update();
}

void QuatCamera::mouseScrollHandler(float y)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= y;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void QuatCamera::keyInputHandler(Camera_Movement input, float delta)
{
	float velocity = m_speed * delta;

	vec3 fvec = {front.x, front.y, front.z};
	vec3 right = glm::normalize(glm::cross(fvec, glm::vec3(0.0, 1.0, 0.0)));

	cout << "before move camera pos : " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	switch(input){
		case FORWARD:
			pos += fvec * velocity;
			cout << "FORWARD" << endl;
			break;
		case BACKWARD : 
			pos -= fvec * velocity;
			cout << "BACKWARD" << endl;
			break;
		case LEFT :
			pos -= right * velocity;
			cout << "LEFT" << endl;
			break;
		case RIGHT :
			pos += right * velocity;
			cout << "RIGHT" << endl;
			break;
	}
	cout << "after camera pos : " << pos.x << ", " << pos.y << ", " << pos.z << endl;
}
#endif