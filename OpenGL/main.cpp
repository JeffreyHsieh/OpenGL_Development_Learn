/*
	Jeffrey Hsieh
	Nav Project
	April 18, 2019
*/
#include <iostream>

// Static bind
//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <queue>

// Headers
#include "Camera.h"
#include "Shader.h"
#include "User.h"
#include "SOIL2.h"

int screen_width, screen_height;

const GLint WIDTH = 800;
const GLint HEIGHT = 600;


// Functions
void KeyListener(GLFWwindow *window, int key, int code, int action, int mode);
void MouseScrollListener(GLFWwindow* window, double xOff, double yOff);
void MouseListener(GLFWwindow* window, double xPos, double yPos);
void Move();

// Create camera object
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat dTime = 0.0f;
GLfloat postFrame = 0.0f;

GLfloat postX = WIDTH / 2.0f;
GLfloat postY = WIDTH / 2.0f;

bool keys[1024];
bool fMouse = true;

int main() {
	// Init
	glfwInit();

	// Setting GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Nav Project - Jeffrey Hsieh", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screen_width, &screen_height);

	// Window check
	if (nullptr == window) {
		std::cout << "Error: Creating window failed" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// Use glfw to get callbacks on inputs
	glfwSetScrollCallback(window, MouseScrollListener);
	glfwSetCursorPosCallback(window, MouseListener);
	glfwSetKeyCallback(window, KeyListener);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	// Check glew
	if (GLEW_OK != glewInit()) {
		std::cout << "Error: Initializing GLEW failed" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screen_width, screen_height);

	glEnable(GL_DEPTH_TEST);

	// Create shader
	Shader shader("core.vs", "core.frag");

	// Create vertex
	GLfloat vertices[] = {

		// front
		// x building height x x x
		-0.5f, -1.0f, -0.5f,  0.0f, 0.0f,
		0.5f, -1.0f, -0.5f,  1.0f, 0.0f,
		0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
		0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
		-0.5f,  1.0f, -0.5f,  0.0f, 1.0f,
		-0.5f, -1.0f, -0.5f,  0.0f, 0.0f,

		// back
		// x building height x x x
		-0.5f, -1.0f,  0.5f,  0.0f, 0.0f,
		0.5f, -1.0f,  0.5f,  1.0f, 0.0f,
		0.5f,  1.0f,  0.5f,  1.0f, 1.0f,
		0.5f,  1.0f,  0.5f,  1.0f, 1.0f,
		-0.5f,  1.0f,  0.5f,  0.0f, 1.0f,
		-0.5f, -1.0f,  0.5f,  0.0f, 0.0f,

		// left
		// x building height x x x
		-0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
		-0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
		-0.5f, -1.0f, -0.5f,  0.0f, 1.0f,
		-0.5f, -1.0f, -0.5f,  0.0f, 1.0f,
		-0.5f, -1.0f,  0.5f,  0.0f, 0.0f,
		-0.5f,  1.0f,  0.5f,  1.0f, 0.0f,

		// right
		// x building height x x x
		0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
		0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
		0.5f, -1.0f, -0.5f,  0.0f, 1.0f,
		0.5f, -1.0f, -0.5f,  0.0f, 1.0f,
		0.5f, -1.0f,  0.5f,  0.0f, 0.0f,
		0.5f,  1.0f,  0.5f,  1.0f, 0.0f,

		// bot
		// x building height x x x
		-0.5f, -1.0f, -0.5f,  0.0f, 1.0f,
		0.5f, -1.0f, -0.5f,  1.0f, 1.0f,
		0.5f, -1.0f,  0.5f,  1.0f, 0.0f,
		0.5f, -1.0f,  0.5f,  1.0f, 0.0f,
		-0.5f, -1.0f,  0.5f,  0.0f, 0.0f,
		-0.5f, -1.0f, -0.5f,  0.0f, 1.0f,

		// top
		// x building height x x x
		-0.5f,  1.0f, -0.5f,  0.0f, 1.0f,
		0.5f,  1.0f, -0.5f,  1.0f, 1.0f,
		0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
		0.5f,  1.0f,  0.5f,  1.0f, 0.0f,
		-0.5f,  1.0f,  0.5f,  0.0f, 0.0f,
		-0.5f,  1.0f, -0.5f,  0.0f, 1.0f
	};
	GLfloat vertices_road[] = {
		-1.5f, -0.1f, -1.5f,  0.0f, 0.0f,
		1.5f, -0.1f, -1.5f,  1.0f, 0.0f,
		1.5f,  0.1f, -1.5f,  1.0f, 1.0f,
		1.5f,  0.1f, -1.5f,  1.0f, 1.0f,
		-1.5f,  0.1f, -1.5f,  0.0f, 1.0f,
		-1.5f, -0.1f, -1.5f,  0.0f, 0.0f,

		-1.5f, -0.1f,  1.5f,  0.0f, 0.0f,
		1.5f, -0.1f,  1.5f,  1.0f, 0.0f,
		1.5f,  0.1f,  1.5f,  1.0f, 1.0f,
		1.5f,  0.1f,  1.5f,  1.0f, 1.0f,
		-1.5f,  0.1f,  1.5f,  0.0f, 1.0f,
		-1.5f, -0.1f,  1.5f,  0.0f, 0.0f,

		-1.5f,  0.1f,  1.5f,  1.0f, 0.0f,
		-1.5f,  0.1f, -1.5f,  1.0f, 1.0f,
		-1.5f, -0.1f, -1.5f,  0.0f, 1.0f,
		-1.5f, -0.1f, -1.5f,  0.0f, 1.0f,
		-1.5f, -0.1f,  1.5f,  0.0f, 0.0f,
		-1.5f,  0.1f,  1.5f,  1.0f, 0.0f,

		1.5f,  0.1f,  1.5f,  1.0f, 0.0f,
		1.5f,  0.1f, -1.5f,  1.0f, 1.0f,
		1.5f, -0.1f, -1.5f,  0.0f, 1.0f,
		1.5f, -0.1f, -1.5f,  0.0f, 1.0f,
		1.5f, -0.1f,  1.5f,  0.0f, 0.0f,
		1.5f,  0.1f,  1.5f,  1.0f, 0.0f,

		-1.5f, -0.1f, -1.5f,  0.0f, 1.0f,
		1.5f, -0.1f, -1.5f,  1.0f, 1.0f,
		1.5f, -0.1f,  1.5f,  1.0f, 0.0f,
		1.5f, -0.1f,  1.5f,  1.0f, 0.0f,
		-1.5f, -0.1f,  1.5f,  0.0f, 0.0f,
		-1.5f, -0.1f, -1.5f,  0.0f, 1.0f,

		-1.5f,  0.1f, -1.5f,  0.0f, 1.0f,
		1.5f,  0.1f, -1.5f,  1.0f, 1.0f,
		1.5f,  0.1f,  1.5f,  1.0f, 0.0f,
		1.5f,  0.1f,  1.5f,  1.0f, 0.0f,
		-1.5f,  0.1f,  1.5f,  0.0f, 0.0f,
		-1.5f,  0.1f, -1.5f,  0.0f, 1.0f
	};
	GLfloat vertices_user[] = {
		-0.25f, -0.25f, -0.25f,  0.0f, 0.0f,
		0.25f, -0.25f, -0.25f,  1.0f, 0.0f,
		0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		-0.25f,  0.25f, -0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 0.0f,

		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
		0.25f,  0.25f,  0.25f,  1.0f, 1.0f,
		0.25f,  0.25f,  0.25f,  1.0f, 1.0f,
		-0.25f,  0.25f,  0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,

		-0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		-0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		-0.25f,  0.25f,  0.25f,  1.0f, 0.0f,

		0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		0.25f,  0.25f,  0.25f,  1.0f, 0.0f,

		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
		0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
		0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,

		-0.25f,  0.25f, -0.25f,  0.0f, 1.0f,
		0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		-0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
		-0.25f,  0.25f, -0.25f,  0.0f, 1.0f
	};
	// Generate multiple positions : building/road locations
	glm::vec3 cubes[] =
	{
		// Distance between, elevation, across
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 0.0f, 0.0f),
		glm::vec3(4.0f, 0.0f, 0.0f),
		glm::vec3(6.0f, 0.0f, 0.0f),
		glm::vec3(8.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(2.0f, 0.0f, 5.0f),
		glm::vec3(4.0f, 0.0f, 5.0f),
		glm::vec3(6.0f, 0.0f, 5.0f),
		glm::vec3(8.0f, 0.0f, 5.0f)
	};

	glm::vec3 planes[] =
	{
		// Distance between, elevation, across
		glm::vec3(0.0f, -1.0f, 2.5f),
		glm::vec3(3.0f, -1.0f, 2.5f),
		glm::vec3(6.0f, -1.0f, 2.5f),
		glm::vec3(9.0f, -1.0f, 2.5f),
		glm::vec3(12.0f, -1.0f, 2.5f)
	};

	glm::vec3 users[] =
	{
		glm::vec3(0.0f, -0.25f, 2.5f)
	};

	// Create buffers
	GLuint v_buff, v_arr;
	//GLuint e_buff;
	glGenVertexArrays(1, &v_arr);
	glGenBuffers(1, &v_buff);
	//glGenBuffers(1, &e_buff);

	glBindVertexArray(v_arr);
	glBindBuffer(GL_ARRAY_BUFFER, v_buff);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e_buff);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position : change the number for number of vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Binding
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// TEXTURE ----------------------------------------------------------------
	// Texture : building
	GLuint texture_building;

	int height = 0;
	int width = 0;
	glGenTextures(1, &texture_building);
	glBindTexture(GL_TEXTURE_2D, texture_building);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	unsigned char* img = SOIL_load_image("res/images/building.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	// Clean data : building
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	// Texture : road
	GLuint texture_road;

	glGenTextures(1, &texture_road);
	glBindTexture(GL_TEXTURE_2D, texture_road);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	img = SOIL_load_image("res/images/road.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Clean data : road
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Texture : user
	GLuint texture_user;

	glGenTextures(1, &texture_user);
	glBindTexture(GL_TEXTURE_2D, texture_user);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	img = SOIL_load_image("res/images/user.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Clean data : user
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);
	// TEXTURE ----------------------------------------------------------------


	// Making dummy travel coordinates
	// Say t0 at vec3(0.0f, 0.0f, 2.5f)
	//     t1 at vec3(1.0f, 0.0f, 2.5f)
	//     t2 at vec3(2.0f, 0.0f, 2.5f)
	//     t3 at vec3(2.5f, 0.0f, 1.5f)
	//     t4 at vec3(6.0f, 0.0f, 0.5f)

	// Create user / travel data
	std::vector<glm::vec3> v;
	v.push_back(glm::vec3(0.0f, -0.2f, 2.5f));
	v.push_back(glm::vec3(2.0f, -0.2f, 2.5f));
	v.push_back(glm::vec3(4.0f, -0.2f, 2.5f));
	v.push_back(glm::vec3(6.0f, -0.2f, 2.5f));
	v.push_back(glm::vec3(10.0f, -0.2f, 2.5f));
	User user(0.0f, -0.2f, 2.5f, 1.0f, v);

	// Loop here
	while (!glfwWindowShouldClose(window)) {
		// Set Frame
		GLfloat preFrame = glfwGetTime();
		dTime = preFrame - postFrame;
		postFrame = preFrame;
		
		glfwPollEvents();
		Move();
		
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw
		shader.Use();

		glm::mat4 projection = glm::perspective(cam.GetZoom(), (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 1000.0f);

		glm::mat4 model(1);
		glm::mat4 view(1);
		
		// Can chain multiple transformations by duplicating it.
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.5f, 1.0f, 0.0f));
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = cam.GetViewM();
	
		// Change speed test
		user.ChangeSpeed(5.0f);

		GLint model_location = glGetUniformLocation(shader.Program, "model");
		GLint view_location = glGetUniformLocation(shader.Program, "view");
		GLint projection_location = glGetUniformLocation(shader.Program, "projection");
		
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));	

		// USER CREATION -----------------------------------------------------------------------------------------------
		glBindVertexArray(v_arr);
		for (GLuint i = 0; i < 1; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, v_buff);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_road), vertices_user, GL_STATIC_DRAW);

			// Position : change the number for number of vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			// Binding
			//glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Set Texture : road
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_user);
			glUniform1i(glGetUniformLocation(shader.Program, "texture"), 0);

			glm::mat4 model(1);
			
			// Create pathing algorithm here: from start to end, steps
			// Check if still in motion
			//if (user.Pathing()) {
				// Check if current position reached the given position.

				// Debug
				std::cout << "cx: " << user.GetCurrentX() << std::endl;
				std::cout << "x: " << user.GetX() << std::endl;
				if (user.GetCurrentX() == user.GetX() && user.GetCurrentY() == user.GetY() && user.GetCurrentZ() == user.GetZ()) {
					user.NextCoord();
				}
				else if (user.GetCurrentX() < user.GetX()) {
					model = user.Forward(model, users[i]);
					//user.SetX(user.GetCurrentX() + 1.0f);
				}
				else if (user.GetCurrentX() > user.GetX()) {
					model = user.Backward(model, users[i]);
					//user.SetX(user.GetCurrentX() - 1.0f);
				}
				else if (user.GetCurrentZ() > user.GetZ()) {
					model = user.Right(model, users[i]);
					//user.SetZ(user.GetCurrentZ() + 1.0f);
				}
				else if (user.GetCurrentX() > user.GetX()) {
					model = user.Left(model, users[i]);
					//user.SetZ(user.GetCurrentZ() - 1.0f);
				}
			//}

			// Debug
			// model = user.Forward(model, users[i]);
			glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		
		glBindVertexArray(v_arr);

		for (GLuint i = 0; i < 5; i++) {
			// test here---------------------------
			glBindBuffer(GL_ARRAY_BUFFER, v_buff);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_road), vertices_road, GL_STATIC_DRAW);

			// Position : change the number for number of vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			// Set Texture : road
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_road);
			glUniform1i(glGetUniformLocation(shader.Program, "texture"), 0);

			glm::mat4 model(1);
			model = glm::translate(model, planes[i]);
			glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
	

		glBindVertexArray(v_arr);
		// Can make this into a function of instantiations!!
		for (GLuint i = 0; i < 10; i++) {
			glBindBuffer(GL_ARRAY_BUFFER, v_buff);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// Position : change the number for number of vertices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			// Set Texture : building
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_building);
			glUniform1i(glGetUniformLocation(shader.Program, "texture"), 0);

			glm::mat4 model(1);
			// moving
			model = glm::translate(model, cubes[i]);
			glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	
		glBindVertexArray(0);

		/*
		// Transformation
		glm::mat4 transform(1);
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		GLint transformLocation = glGetUniformLocation(shader.Program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
		*/

		/*
		glBindVertexArray(v_arr);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		*/

		// Start creating objects
		glfwSwapBuffers(window);
	}

	// Deallocating
	glDeleteVertexArrays(1, &v_arr);
	glDeleteBuffers(1, &v_buff);
	//glDeleteBuffers(1, &e_buff);

	// Kill program
	glfwTerminate();

	return EXIT_SUCCESS;
}

// Function Implementation
void Move() {
	if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]) {
		cam.KeyActions(MoveForward, dTime);
	}
	if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]) {
		cam.KeyActions(MoveLeft, dTime);
	}
	if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]) {
		cam.KeyActions(MoveBackward, dTime);
	}
	if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) {
		cam.KeyActions(MoveRight, dTime);
	}
}

void KeyListener(GLFWwindow* window, int key, int code, int action, int mode) {
	// Listen 
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (GLFW_PRESS == action) {
			keys[key] = true;
		}
		else if (GLFW_RELEASE == action) {
			keys[key] = false;
		}
	}
}

void MouseScrollListener(GLFWwindow* window, double xOff, double yOff) {
	cam.MouseScroll(yOff);
}

void MouseListener(GLFWwindow* window, double xPos, double yPos) {
	if (fMouse) {
		postX = xPos;
		postY = yPos;
		fMouse = false;
	}

	GLfloat x_offset = xPos - postX;
	GLfloat y_offset = postY - yPos;

	postX = xPos;
	postY = yPos;

	cam.MouseActions(x_offset, y_offset);
}