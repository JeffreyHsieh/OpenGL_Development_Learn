/*
	Jeffrey Hsieh
	Nav Project
	April 18, 2019

	Creating a simple camera to observe the map
*/

#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
//#define GLEW_STATIC

enum Camera_Motion
{
	MoveLeft,
	MoveRight,
	MoveForward,
	MoveBackward
};

const GLfloat Sensitivity = 0.3f;
const GLfloat Zoom = 45.0f;
const GLfloat Yaw = -100.f;
const GLfloat Pitch = 0.0f;
const GLfloat Speed = 7.0f;

class Camera {
	private:
		GLfloat yaw;
		GLfloat pitch;
		GLfloat movement_speed;
		GLfloat mouse_sens;
		GLfloat zoom;

		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 world_up;

		void UpdateCameraVec() {
			glm::vec3 front;
			front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			front.y = sin(glm::radians(this->pitch));
			front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			this->front = glm::normalize(front);
			this->right = glm::normalize(glm::cross(this->front, this->world_up));
			this->up = glm::normalize(glm::cross(this->right, this->front));
		};
	public:
		Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up_vec = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = Yaw, GLfloat pitch = Pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(Speed), mouse_sens(Sensitivity), zoom(Zoom) {
			this->UpdateCameraVec();
			this->yaw = yaw;
			this->pitch = pitch;
			this->world_up = up_vec;
			this->position = position;
		}

		Camera(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat up_x, GLfloat up_y, GLfloat up_z, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(Speed), mouse_sens(Sensitivity), zoom(Zoom) {
			this->UpdateCameraVec();
			this->yaw = yaw;
			this->pitch = pitch;
			this->world_up = glm::vec3(up_x, up_y, up_z);
			this->position = glm::vec3(pos_x, pos_y, pos_z);
		}

		// Getters
		GLfloat GetZoom() {
			return this->zoom;
		}

		glm::mat4 GetViewM() {
			return glm::lookAt(this->position, this->position + this->front, this->up);
		}

		// Setters
		void ChangeMouseSens(GLfloat sensitivity) {
			this->mouse_sens = sensitivity;
		}

		// Key actions for camera movement
		void KeyActions(Camera_Motion dir, GLfloat dTime) {
			GLfloat v = this->movement_speed * dTime;

			if (MoveLeft == dir)
			{
				this->position -= this->right * v;
			}
			if (MoveRight == dir)
			{
				this->position += this->right * v;
			}
			if (MoveForward == dir)
			{
				this->position += this->front * v;
			}
			if (MoveBackward == dir)
			{
				this->position -= this->front * v;
			}


		}

		// Mouse action for camera movement
		void MouseActions(GLfloat xOff, GLfloat yOff, GLboolean pitch_bound = true) {
			this->pitch += yOff;
			this->yaw += xOff;
			
			xOff *= this->mouse_sens;
			yOff *= this->mouse_sens;

			if (pitch_bound) {
				if (this->pitch > 90.0f)
				{
					this->pitch = 90.0f;
				}
				
				if (this->pitch < -90.0f)
				{
					this->pitch = -90.0f;
				}
			}

			this->UpdateCameraVec();
		}
	
		void MouseScroll(GLfloat yOff) {
			if (this->zoom >= 1.0f && this->zoom <= 50.0f) {
				this->zoom -= yOff;
			}
			if (this->zoom <= 1.0f) {
				this->zoom = 1.0f;
			}
			if (this->zoom >= 50.0f) {
				this->zoom = 50.0f;
			}
		}
};