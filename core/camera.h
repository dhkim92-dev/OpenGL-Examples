#ifndef __GL_EXAMPLE_CAMERA_H__
#define __GL_EXAMPLE_CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

using namespace std;
using namespace glm;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class BaseCamera{
public : 
    virtual mat4 getView()=0;
    virtual void update()=0;
    virtual void mouseMoveHandler(float x, float y) = 0;
    virtual void mouseScrollHandler(float y)=0;
    virtual void keyInputHandler(Camera_Movement input, float delta)=0;
};

class ECamera : public BaseCamera // Euclidian Camera
{
private:
    // Camera Attributes
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    
	// Euler Angles
    float yaw;
    float pitch;
    
	// Camera options
    float m_speed;
    float m_sensi;
    float zoom;
public :
	ECamera();
	ECamera(vec3 pos, vec3 front, vec3 world_up, float yaw, float pitch);
	mat4 getView();
	void update();

	void mouseMoveHandler(float x, float y);
	void mouseScrollHandler(float y);
	void keyInputHandler(Camera_Movement input, float delta);
};

class QuatCamera : public BaseCamera{
private :
    glm::vec3 pos;
	// pitch yaw roll euler angle
    float yaw;
    float pitch;
    float roll;

    //quat 

    glm::quat q_yaw;
    glm::quat q_pitch;
    glm::quat q_roll;
    
	// Camera options
    float m_speed;
    float m_sensi;
    float zoom;
public :
	QuatCamera();
	QuatCamera(vec3 pos, float yaw, float pitch);
	mat4 getView();
	void update();

	void mouseMoveHandler(float x, float y);
	void mouseScrollHandler(float y);
	void keyInputHandler(Camera_Movement input, float delta);

};


#endif