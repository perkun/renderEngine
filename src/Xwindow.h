#ifndef XWINDOW_H_
#define XWINDOW_H_
#include "stdio.h"
#include <stdlib.h>

#include <GL/glew.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

#define GLM_FORCE_RADIANS

class Xwindow {
public:
	Display *dpy;
	Window root;
	GLint att[5];// = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	Window win;
	GLXContext glc;
	XWindowAttributes gwa;
	XEvent xev;

	int window_dim_x, window_dim_y;

	
	Xwindow();
	void initWindow(int, int, bool);
	void update();
	void clear(float r, float g, float b, float a);
	
	~Xwindow();
};

#endif /* XWINDOW_H_ */

