#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera()
	{
	}

	void initCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	glm::vec3 getPos()
	{
		return this->pos;
	}

	inline glm::mat4 getViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	inline glm::mat4 getProjection() const
	{
		return projection;
	}

	inline glm::mat4 getView() const
	{
		return glm::lookAt(pos, pos + forward, up);
	}

	void setLook(glm::vec3 modelPos)
	{
		forward = glm::vec3(glm::normalize(modelPos - pos));
	}

	// Returns a right‐vector perpendicular to forward & up
	glm::vec3 getRight() const 
	{
		return glm::normalize(glm::cross(forward, up));
	}

	// Moves camera along its forward axis
	void moveForward(float amt) 
	{
		pos += forward * amt;
	}

	// Moves camera along its right axis
	void moveRight(float amt) 
	{
		pos += getRight() * amt;
	}

	// Moves camera along its up vector
	void moveUp(float amount) 
	{
		pos += up * amount;
	}

	void rotate(float yawOffset, float pitchOffset) 
	{
		const float maxPitch = 89.0f;
		const float minPitch = -89.0f;

		yaw += yawOffset;
		pitch += pitchOffset;
		// clamp pitch so you don’t flip upside-down
		if (pitch > maxPitch) pitch = maxPitch;
		if (pitch < minPitch) pitch = minPitch;

		// Recalculate forward vector from spherical angles:
		glm::vec3 f;
		f.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		f.y = sin(glm::radians(pitch));
		f.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		forward = glm::normalize(f);
	}

protected:
private:
	float yaw = -90.0f;
	float pitch = 0.0f;

	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};


