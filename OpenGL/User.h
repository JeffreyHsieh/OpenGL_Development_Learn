/*
	Jeffrey Hsieh
	Nav Project
	April 18, 2019
*/

#pragma once
#include <vector>
#include <queue>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class User{
	private:
		float c_coordinateX;
		float c_coordinateY;
		float c_coordinateZ;
		float speed;
		std::vector<glm::vec3> traveling_coordinates;
		std::queue<glm::vec3> path;
	public:
		// Need fixing for passing vector more efficiently.
		User(float cx, float cy, float cz, float speed, std::vector<glm::vec3> travel_coord) {
			this->speed = speed;
			this->traveling_coordinates = travel_coord;
			this->c_coordinateX = cx;
			this->c_coordinateY = cy;
			this->c_coordinateZ = cz;

			// Making it more dynamic : pushing coordinates
			for (int i = 0; i < travel_coord.size(); i++) {
				path.push(travel_coord.at(i));
			}
		}
		float GetCurrentX() {
			return this->c_coordinateX;
		}
		float GetCurrentY() {
			return this->c_coordinateY;
		}
		float GetCurrentZ() {
			return this->c_coordinateZ;
		}
		int GetSize() {
			return this->traveling_coordinates.size();
		}
		glm::vec3 GetElement(int i) {
			return this->traveling_coordinates.at(i);
		}
		void SetX(float x) {
			this->c_coordinateX = x;
		}
		void SetY(float y) {
			this->c_coordinateY = y;
		}
		void SetZ(float z) {
			this->c_coordinateZ = z;
		}
		// Checking if it is done pathing
		bool Pathing() {
			//std::cout << "checking" << std::endl;
			return (path.empty()) ? false : true;
		}
		float GetX() {
			if (!path.empty()) {
				return path.front().x;
			}
			return 0.0;
		}
		float GetY() {
			if (!path.empty()) {
				return path.front().y;
			}
			return 0.0;
		}
		float GetZ() {
			if (!path.empty()) {
				return path.front().z;
			}
			return 0.0;
		}
		void NextCoord() {
			if (!path.empty()) {
				path.pop();
			}
		}
		void ChangeSpeed(float speed) {
			this->speed = speed;
		}
		// User motion functions
		// Motion direction unit vectors
		// glm::vec3 forward(1.0f, 0.0f, 0.0f);
		// glm::vec3 backward(-1.0f, 0.0f, 0.0f);
		// glm::vec3 left(0.0f, 0.0f, 1.0f);
		// glm::vec3 right(0.0f, 0.0f, 1.0f);
		// glm::vec3 stop(0.0f, 0.0f, 0.0f);

		glm::mat4 Forward(glm::mat4 model, glm::vec3 user) {
			glm::vec3 forward(1.0f, 0.0f, 0.0f);
			return glm::translate(model, ((GLfloat)glfwGetTime() * speed * forward) + user);
		}

		glm::mat4 Backward(glm::mat4 model, glm::vec3 user) {
			glm::vec3 backward(-1.0f, 0.0f, 0.0f);
			return glm::translate(model, ((GLfloat)glfwGetTime() * speed * backward) + user);
		}

		glm::mat4 Left(glm::mat4 model, glm::vec3 user) {
			glm::vec3 left(0.0f, 0.0f, -1.0f);
			return glm::translate(model, ((GLfloat)glfwGetTime() * speed * left) + user);
		}

		glm::mat4 Right(glm::mat4 model, glm::vec3 user) {

			glm::vec3 right(0.0f, 0.0f, 1.0f);
			return glm::translate(model, ((GLfloat)glfwGetTime() * speed * right) + user);

		}

		glm::mat4 Stop(glm::mat4 model, glm::vec3 user) {
			glm::vec3 stop(0.0f, 0.0f, 0.0f);
			return glm::translate(model, ((GLfloat)glfwGetTime() * speed * stop) + user);
		}

};