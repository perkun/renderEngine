#include "RenderEngine.h"

#define GLM_FORCE_RADIANS

int Texture::num_textures = 0;



RenderEngine::RenderEngine(int w, int h, bool visible, bool off_screen_rendering)
{
	DISPLAY_WIDTH = w;
	DISPLAY_HEIGHT = h;
	shadow_size = 1024 * 2;
	
	Xdisplay.initWindow(w, h, visible);
	
	
	// 	shadow_shader_id = addShader("shadowShader");
	shadow_shader_id = 0;
	Shader *temp = new Shader("shadowShader");
	shaders.push_back(temp);
	shaders_camera.push_back(0);
	shaders_texture.push_back(0);
	
	shadow_fb_id = addFramebuffer(shadow_size, shadow_size);
	shadow_texture_id = addTexture(shadow_size, shadow_size);
	
	fb = addFramebuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	fb_tx = addTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	
	render_off_screen = off_screen_rendering;
	interactive = false;
}



void RenderEngine::renderScene() {


	if (!render_off_screen) {
			Xdisplay.clear(0.0, 0., 0., 1.);
	}

	// create shadow map
	framebuffers[shadow_fb_id]->bind();
	framebuffers[shadow_fb_id]->clear(0., 0., 0., 1.);
	shaders[shadow_shader_id]->bind();
	for (int i = 0; i < models.size(); i++) {
		shaders[shadow_shader_id]->update(models[i]->transform, *cameras[0], *cameras[1]);
		models[i]->draw();
	}

	// normal drawing
	if (!render_off_screen) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}
	else {
// 		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[2]->frame_buffer);
		framebuffers[fb]->bind();
		GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers (1, draw_buffers);
		textures[fb_tx]->bind();
		framebuffers[fb]->clear(0., 0., 0., 1.);

	}

	for (int i = 0; i < models.size(); i++) {
		shaders[models_shader[i]]->bind();
		shaders[models_shader[i]]->update(models[i]->transform, *cameras[0], *cameras[1]);
		models[i]->draw();
	}



	if (!render_off_screen) {
			Xdisplay.update();
	}
}




void RenderEngine::linkShaderToModel(int shader_index, int model_index) {
	models_shader[model_index] = shader_index;
}

void RenderEngine::linkTextureToShader(int texture_index, int shader_index) {
	shaders_texture[shader_index] = texture_index;
}

void RenderEngine::linkCameraToShader(int camera_index, int shader_index) {
	shaders_camera[shader_index] = camera_index;
}


int RenderEngine::addModel(const std::string& file_name) {
	Mesh *temp = new Mesh(file_name);
	models.push_back(temp);
	
	models_shader.push_back(0);
	
	return models.size() -1 ;
}

int RenderEngine::addModel(float vertices[][3], int num_pkt, int indices[][3], int num_tr) {
	Mesh *temp = new Mesh(vertices, num_pkt, indices, num_tr);
	models.push_back(temp);
	
	models_shader.push_back(0);
	
	return models.size() -1 ;
}



int RenderEngine::addShader(const std::string& file_name) {
	Shader *temp = new Shader(file_name);
	shaders.push_back(temp);
	
	shaders_camera.push_back(0);
	shaders_texture.push_back(0);
	
	// automatyczne dodanie uniform name "depth_map" do shadera
	textures[shadow_texture_id]->setUniform(*shaders[shaders.size() -1] , "dddd", "depth_map");
	
	return shaders.size() -1 ;
}
/*
int RenderEngine::addTexture(const std::string& file_name) {
	Texture *temp = new Texture(file_name);
	textures.push_back(temp);
	return textures.size() -1 ;
}
*/
int RenderEngine::addTexture(int w, int h) {
	Texture *temp = new Texture(w ,h);
	textures.push_back(temp);
	return textures.size() -1 ;
}


int RenderEngine::addFramebuffer(int w, int h) {
	FrameBuffer *temp = new FrameBuffer(w,h);
	framebuffers.push_back(temp);
// 	framebuffers[framebuffers.size()-1].initialize(w, h);
	return framebuffers.size() -1 ;
}

int RenderEngine::addCamera(const glm::vec3 pos, glm::vec3 targ, float left, float right, float bottom, float top, float zNear, float zFar) {
	Camera *temp = new Camera(pos, targ, left, right, bottom, top, zNear, zFar);
	cameras.push_back(temp);
// 	cameras[cameras.size()-1].initialize(pos, targ, left, right, top, bottom, zNear, zFar);
	return cameras.size() -1;
}

void RenderEngine::updateCamera(const glm::vec3 pos, glm::vec3 targ, float left, float right, float bottom, float top, float zNear, float zFar, int camera_id) {
	Camera *temp = new Camera(pos, targ, left, right, bottom, top, zNear, zFar);
	cameras.at(camera_id) = temp;
	// 	cameras.push_back(temp);
}

int RenderEngine::addCamera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect, float zNear, float zFar) {
	Camera *temp = new Camera(pos, targ, fov, aspect, zNear, zFar);
	cameras.push_back(temp);
// 	cameras[cameras.size()-1].initialize(pos, targ, fov, aspect, zNear, zFar);
	return cameras.size() -1;
}




RenderEngine::~RenderEngine() {
	for (int i = 0; i < models.size(); i++)
		delete models[i];
	
	for (int i = 0; i < shaders.size(); i++)
		delete shaders[i];
	
	for (int i = 0; i < textures.size(); i++)
		delete textures[i];
	
	for (int i = 0; i < framebuffers.size(); i++)
		delete framebuffers[i];
	
	for (int i = 0; i < cameras.size(); i++)
		delete cameras[i];
	
	if (!interactive)	// czyli że Xwindow jest
		XDestroyWindow(Xdisplay.dpy, Xdisplay.win);
}







