/*
	Jeffrey Hsieh
	Nav Project
	April 18, 2019
*/

#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
	public:
		GLuint Program;
		Shader(const GLchar* vertex_path, const GLchar* fragment_path) {
			std::string vertex_code;
			std::string fragment_code;
			std::ifstream v_file;
			std::ifstream f_file;

			v_file.exceptions(std::ifstream::badbit);
			f_file.exceptions(std::ifstream::badbit);
			try
			{
				// Open
				v_file.open(vertex_path);
				f_file.open(fragment_path);
				std::stringstream v_stream, f_stream;

				// Read
				v_stream << v_file.rdbuf();
				f_stream << f_file.rdbuf();

				// Close
				v_file.close();
				f_file.close();

				vertex_code = v_stream.str();
				fragment_code = f_stream.str();
			}
			catch (std::ifstream::failure e) {
				std::cout << "Error: Cannot read shader files" << std::endl;
			}
			const GLchar* v_code = vertex_code.c_str();
			const GLchar* f_code = fragment_code.c_str();

			// Compiling
			GLuint vertex, fragment;
			GLint clear;
			GLchar log[512];

			// Vertex
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &v_code, NULL);
			glCompileShader(vertex);

			// Error check
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &clear);
			if (!clear) {
				glGetShaderInfoLog(vertex, 512, NULL, log);

				std::cout << "Error: Vertex shader compilation failed" << log << std::endl;
			}

			// Fragment
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &f_code, NULL);
			glCompileShader(fragment);

			// Error check
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &clear);
			if (!clear) {
				glGetShaderInfoLog(fragment, 512, NULL, log);

				std::cout << "Error: Vertex shader compilation failed" << log << std::endl;
			}
			this->Program = glCreateProgram();
			glAttachShader(this->Program, vertex);
			glAttachShader(this->Program, fragment);
			glLinkProgram(this->Program);
			
			glGetProgramiv(this->Program, GL_LINK_STATUS, &clear);

			if (!clear) {
				glGetProgramInfoLog(this->Program, 512, NULL, log);

				std::cout << "Error: Program shader linking failed" << log << std::endl;
			}
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}
		void Use() {
			glUseProgram(this->Program);
		}
};
#endif
