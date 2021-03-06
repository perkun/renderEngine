#ifndef WINGLFW_H_
#define WINGLFW_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GLFW/glfw3.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#define GLM_FORCE_RADIANS

class Keys {
public:
	bool I, J, U, H, LSB, RSB, A, W, S, D, C, O, P, K, L, Z, X, F, R, N, M, EQUAL, MINUS;
	bool UP, DOWN, LEFT, RIGHT, SPACE;

	Keys() {
		A = W = S = D = C = false;
		UP = DOWN = LEFT = RIGHT = false;
	}

	void update(GLFWwindow* window);

};

class WinGLFW {
public:
	WinGLFW() {}
	WinGLFW(int width, int height, const std::string& title );
	virtual ~WinGLFW();

	void update();
	void clear(float r, float g, float b, float a);
	void clear(glm::vec4 color);
	void setDefaultViewport();
	void setViewport(int, int, int, int);

	Keys keys;
	int disp_width, disp_height;
	GLFWwindow* window;
	bool is_closed;
};

#endif /* WINGLFW_H_ */
