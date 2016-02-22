/*
 * WinGLFW.cpp
 *
 *  Created on: 15 wrz 2015
 *      Author: grzeslaff
 */

#include "WinGLFW.h"

#define GLM_FORCE_RADIANS

void Keys::update(GLFWwindow* window)
{
	glfwPollEvents();


	if (glfwGetKey (window, GLFW_KEY_W)) {
		W = true;
	}
	else
		W = false;

	if (glfwGetKey (window, GLFW_KEY_A)) {
		A = true;
	}
	else
		A = false;

	if (glfwGetKey (window, GLFW_KEY_S)) {
		S = true;
	}
	else
		S = false;

	if (glfwGetKey (window, GLFW_KEY_D)) {
		D = true;
	}
	else
		D = false;


	if (glfwGetKey (window, GLFW_KEY_O)) {
		O = true;
	}
	else
		O = false;

	if (glfwGetKey (window, GLFW_KEY_P)) {
		P = true;
	}
	else
		P = false;

	if (glfwGetKey (window, GLFW_KEY_K)) {
		K = true;
	}
	else
		K = false;

	if (glfwGetKey (window, GLFW_KEY_L)) {
		L = true;
	}
	else
		L = false;

	if (glfwGetKey (window, GLFW_KEY_Z)) {
		Z = true;
	}
	else
		Z = false;

	if (glfwGetKey (window, GLFW_KEY_X)) {
		X = true;
	}
	else
		X = false;

	if (glfwGetKey (window, GLFW_KEY_R)) {
		R = true;
	}
	else
		R = false;

	if (glfwGetKey (window, GLFW_KEY_F)) {
		F = true;
	}
	else
		F = false;

	if (glfwGetKey (window, GLFW_KEY_N)) {
		N = true;
	}
	else
		N = false;

	if (glfwGetKey (window, GLFW_KEY_M)) {
		M = true;
	}
	else
		M = false;

	if (glfwGetKey (window, GLFW_KEY_SPACE )) {
		glfwWaitEvents();
		SPACE = !SPACE;
// 		std::cout << "status SPACE: " << SPACE << std::endl;
	}
// 	else
// 		BACKSPACE = false;






	if (glfwGetKey (window, GLFW_KEY_UP)) {
		UP = true;
	}
	else
		UP = false;

	if (glfwGetKey (window, GLFW_KEY_DOWN)) {
		DOWN = true;
	}
	else
		DOWN = false;

	if (glfwGetKey (window, GLFW_KEY_LEFT)) {
		LEFT = true;
	}
	else
		LEFT = false;

	if (glfwGetKey (window, GLFW_KEY_RIGHT)) {
		RIGHT = true;
	}
	else
		RIGHT = false;

	if (glfwGetKey (window, GLFW_KEY_C)) {
		C = true;
	}
	else
		C = false;

}



WinGLFW::WinGLFW(int width, int height, const std::string& title ) {
	disp_height = height;
	disp_width = width;

	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		exit(0);
	}


	window = glfwCreateWindow (width, height, title.c_str(), NULL, NULL);
	if (!window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		exit(0);
	}

	glfwMakeContextCurrent (window);

	// start GLEW extension handler
	glewInit ();


	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace (GL_CCW);


	is_closed = false;

	keys = Keys();
	keys.SPACE = false;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
}

WinGLFW::~WinGLFW() {
	glfwTerminate();
}

void WinGLFW::clear(float r, float g, float b, float a) {

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	glClear(GL_DEPTH_BUFFER_BIT);
}


void WinGLFW::update() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glfwSwapBuffers (window);

	glfwPollEvents ();

	if (glfwGetKey (window, GLFW_KEY_ESCAPE)) {
		puts("ESCAPE key pushed");
		glfwSetWindowShouldClose (window, 1);
		is_closed = true;
	}

// 	if (glfwGetKey (window, GLFW_KEY_A)) {
// // 		printf("A___");
// 		keys.A = true;
// 	}
//


	keys.update(window);


}

void WinGLFW::setDefaultViewport()
{
	glViewport(0, 0, disp_width, disp_height);
}

void WinGLFW::setViewport(int x0, int y0, int width, int height)
{
	glViewport(x0, y0, width, height);
}










