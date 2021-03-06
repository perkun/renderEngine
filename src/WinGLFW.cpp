#include "WinGLFW.h"

#define GLM_FORCE_RADIANS

void Keys::update(GLFWwindow* window)
{
	glfwPollEvents();

	if (glfwGetKey (window, GLFW_KEY_I)) {
		I = true;
	}
	else
		I = false;

	if (glfwGetKey (window, GLFW_KEY_J)) {
		J = true;
	}
	else
		J = false;


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

	if (glfwGetKey (window, GLFW_KEY_U)) {
		U = true;
	}
	else
		U = false;

	if (glfwGetKey (window, GLFW_KEY_H)) {
		H = true;
	}
	else
		H = false;

	if (glfwGetKey (window, GLFW_KEY_SPACE )) {
		glfwWaitEvents();
		SPACE = true;
	}
	else
		SPACE = false;
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
		glfwWaitEvents();
		C = true;
	}
	else
		C = false;

	if (glfwGetKey (window, GLFW_KEY_EQUAL)) {
		glfwWaitEvents();
		EQUAL = true;
	}
	else
		EQUAL = false;

	if (glfwGetKey (window, GLFW_KEY_MINUS)) {
		glfwWaitEvents();
		MINUS = true;
	}
	else
		MINUS = false;

}

WinGLFW::WinGLFW(int width, int height, const std::string& title )
{
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


	//automatic texture generation
// 	glEnable(GL_TEXTURE_GEN_S);
// 	glEnable(GL_TEXTURE_GEN_T);
// 	glEnable(GL_TEXTURE_GEN_R);
// 	glEnable(GL_TEXTURE_GEN_Q);
//
// 	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
// 	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

// 	glDisable(GL_CULL_FACE);

// 	glEnable (GL_BLEND);
// 	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glfwSwapInterval(0);


	is_closed = false;

	keys = Keys();
	keys.SPACE = false;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
}

WinGLFW::~WinGLFW()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void WinGLFW::clear(float r, float g, float b, float a)
{

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	glClear(GL_DEPTH_BUFFER_BIT);
}

void WinGLFW::clear(glm::vec4 color)
{
	glClearColor(color[0], color[1], color[2], color[3] );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	glClear(GL_DEPTH_BUFFER_BIT);
}


void WinGLFW::update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glfwSwapBuffers (window);

	glfwPollEvents();

	int escape_key_status = glfwGetKey(window, GLFW_KEY_ESCAPE);

	if (escape_key_status == GLFW_PRESS) {
		puts("Bye bye :)");
		glfwSetWindowShouldClose(window, 1);
		is_closed = true;
	}

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










