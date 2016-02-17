#include "Xwindow.h"

#define GLM_FORCE_RADIANS

Xwindow::Xwindow() {

}

void Xwindow::initWindow(int x, int y, bool visible) {
	att[0] = GLX_RGBA;	att[1] = GLX_DEPTH_SIZE;	att[2] = 24;	att[3] = GLX_DOUBLEBUFFER;	att[4] = None;
	
	window_dim_x = x;
	window_dim_y = y;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(0);
	}

	root = DefaultRootWindow(dpy) ;

	vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(0);
	}

	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;
	win = XCreateWindow(dpy, root, 0, 0, window_dim_x, window_dim_y, 0,
			vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	if (visible)
		XMapWindow(dpy, win);

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	//glXMakeCurrent(dpy, None,  glc);
	//puts("dupa window");
	
	glewInit();
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace (GL_CCW);
	

}

void Xwindow::clear(float r, float g, float b, float a) {
	
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 	glClear(GL_DEPTH_BUFFER_BIT);
}

void Xwindow::update() {
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glXSwapBuffers(dpy, win);
}


Xwindow::~Xwindow() {
// 	XDestroyWindow(dpy, win);
}

