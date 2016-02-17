#ifndef RENDERENGINE_H_
#define RENDERENGINE_H_

#define GLM_FORCE_RADIANS

#include <iostream>
#include <vector>
#include <GL/glew.h>
//#include "/home/grzeslaff/instalki/glfw-3.1.2/include/GLFW/glfw3.h"
#include <GLFW/glfw3.h>
#include<X11/X.h>
#include<X11/Xlib.h>


#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Xwindow.h"



class RenderEngine {
public:
	int DISPLAY_WIDTH;
	int DISPLAY_HEIGHT;
	
	bool render_off_screen;
	bool interactive;
	
	std::vector<Mesh*> models;
	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
	std::vector<FrameBuffer*> framebuffers;
	std::vector<Camera*> cameras;
	
	std::vector< int > models_shader;
	std::vector< int > shaders_texture;
	std::vector< int > shaders_camera;
	
	int shadow_shader_id, shadow_texture_id, shadow_fb_id;
	int shadow_size;
	int fb, fb_tx;
	
	glm::vec3 light_position;
	
	Xwindow Xdisplay;
	Camera camera, light;
	

	RenderEngine(int w, int h, bool visible, bool off_screen_rendering);
	~RenderEngine();
	
	void renderScene();

	
	void linkShaderToModel(int shader_index, int model_index);
	void linkTextureToShader(int texture_index, int shader_index);
	void linkCameraToShader(int camera_index, int shader_index);
		
	int addModel(const std::string &file_name);
	int addModel(float vertices[][3], int num_pkt, int indices[][3], int num_tr);
	int addShader(const std::string &file_name);
	int addTexture(const std::string &file_name);
	int addTexture(int w, int h);
	int addFramebuffer(int w, int h);
	int addCamera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect, float zNear, float zFar);
	int addCamera(const glm::vec3 pos, glm::vec3 targ, float left, float right, float top, float bottom, float zNear, float zFar);
	void updateCamera(const glm::vec3 pos, glm::vec3 targ, float left, float right, float bottom, float top, float zNear, float zFar, int camera_id);
	
	
private:
	
};







#endif /* RENDERENGINE_H_ */