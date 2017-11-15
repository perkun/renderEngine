#include "RenderEngine.h"

#define GLM_FORCE_RADIANS

int Texture::num_textures = 0;



RenderEngine::RenderEngine(int w, int h, bool visible, bool off_screen_rendering)
{
	DISPLAY_WIDTH = w;
	DISPLAY_HEIGHT = h;
	shadow_size = 1024 * 4;

	Xdisplay.initWindow(w, h, visible);


	// 	shadow_shader_id = addShader("shadowShader");
	shadow_shader_id = 0;
	Shader *temp = new Shader("/usr/local/include/shadowShader");
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

RenderEngine::RenderEngine(int w, int h, bool visible,
						   bool off_screen_rendering, bool _interactive ):
						   display(w, h, "RenderEngine window")
{

	DISPLAY_WIDTH = w;
	DISPLAY_HEIGHT = h;
	shadow_size = 1024 * 2;

//      shadow_shader_id = addShader("shadowShader");
	shadow_shader_id = 0;
	Shader *temp = new Shader("/usr/local/include/shadowShader");
	shaders.push_back(temp);
	shaders_camera.push_back(0);
	shaders_texture.push_back(0);

	shadow_fb_id = addFramebuffer(shadow_size, shadow_size);
	shadow_texture_id = addTexture(shadow_size, shadow_size);


	fb = addFramebuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	fb_tx = addTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	render_off_screen = false;
	interactive = _interactive;

	camera_rotation_speed = 0.005;
	speed = 2e-6;


}

void RenderEngine::renderScene()
{

	if (!render_off_screen)
   	{
		if (interactive)
//  			display.clear(0., 0., 0., 1.);
//  			display.clear(1., 1., 1., 1.);
				display.clear(clear_color);
		else
			Xdisplay.clear(0.0, 0., 0., 1.);
	}

	// create shadow map
	framebuffers[shadow_fb_id]->bind();
	framebuffers[shadow_fb_id]->clear(0., 0., 0., 1.);
	shaders[shadow_shader_id]->bind();
	for (int i = 0; i < models.size(); i++)
   	{
		if (models[i]->casting_shadow) {
			shaders[shadow_shader_id]->update(models[i]->transform, *cameras[0],
					*cameras[1]);
			models[i]->draw();
		}
	}

	// normal drawing
	if (!render_off_screen)
   	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}
	else
   	{
// 		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[2]->frame_buffer);
		framebuffers[fb]->bind();
		GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers (1, draw_buffers);
		textures[fb_tx]->bind();
		framebuffers[fb]->clear(0., 0., 0., 1.);

	}

	for (int i = 0; i < models.size(); i++)
   	{
		if (models[i]->visible)
	   	{
			shaders[models_shader[i]]->bind();
// 			for (int j = 0; j < 3; j++)
				shaders[models_shader[i]]->RGBA_value =
				   	models[i]->RGBA_value;
			shaders[models_shader[i]]->update(models[i]->transform, *cameras[0],
					*cameras[1]);
			models[i]->draw();
		}
	}


	if (!render_off_screen)
   	{
		if (interactive)
	   	{
			userInput();
			display.update();
		}
		else
			Xdisplay.update();
	}

}


void RenderEngine::renderSceneNoShadow()
{

	if (!render_off_screen)
   	{
		if (interactive)
// 			display.clear(0., 0., 0., 1.);
			display.clear(1., 1., 1., 1.);
		else
			Xdisplay.clear(0.0, 0., 0., 1.);
	}


	// normal drawing
	if (!render_off_screen)
   	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}
	else
   	{
// 		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[2]->frame_buffer);
		framebuffers[fb]->bind();
		GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers (1, draw_buffers);
		textures[fb_tx]->bind();
		framebuffers[fb]->clear(0., 0., 0., 1.);

	}


	for (int i = 0; i < models.size(); i++)
   	{
		if (models[i]->visible)
	   	{
			shaders[models_shader[i]]->bind();
				shaders[models_shader[i]]->RGBA_value =
				   	models[i]->RGBA_value;
			shaders[models_shader[i]]->update(models[i]->transform, *cameras[0],
					*cameras[1]);
			models[i]->draw();
		}
	}

	if (!render_off_screen)
   	{
		if (interactive)
	   	{
			userInput();
			display.update();
		}
		else
			Xdisplay.update();
	}

}

void RenderEngine::renderSceneRadar()
{
	// tak jak dla normalnie, tylko że bez zbędnych opcji i z użyciem
	// update radar
	//
	// create shadow map
	framebuffers[shadow_fb_id]->bind();
	framebuffers[shadow_fb_id]->clear(0., 0., 0., 1.);
	shaders[shadow_shader_id]->bind();
	for (int i = 0; i < models.size(); i++)
   	{
		if (models[i]->casting_shadow) {
			shaders[shadow_shader_id]->update(models[i]->transform, *cameras[0],
					*cameras[1]);
			models[i]->draw();
		}
	}

	// normal drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!render_off_screen)
   	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);
	}
	else
   	{
// 		glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[2]->frame_buffer);
		framebuffers[fb]->bind();
		GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers (1, draw_buffers);
		textures[fb_tx]->bind();
		framebuffers[fb]->clear(0., 0., 0., 1.);

	}

	for (int i = 0; i < models.size(); i++)
   	{
		if (models[i]->visible)
	   	{
			shaders[models_shader[i]]->bind();
// 			for (int j = 0; j < 3; j++)
// 				shaders[models_shader[i]]->RGBA_value[j] =
// 				   	models[i]->RGBA_value[j];
			shaders[models_shader[i]]->updateRadar(models[i]->transform,
					*cameras[0], *cameras[1]);
			models[i]->draw();
		}
	}

	if (!render_off_screen)
		display.update();
}

void RenderEngine::userInput()
{
// 	float camera_rotation_speed = 0.05;


	if ( camera_mode == FREE )
	{
		if (display.keys.A) {
			cameras[0]->moveLeft(0.01);
		}
		if (display.keys.D) {
			cameras[0]->moveRight(0.01);
		}
		if (display.keys.W) {
			cameras[0]->moveForward(0.01);
		}
		if (display.keys.S) {
			cameras[0]->moveBack(0.01);
		}
		if (display.keys.R) {
			cameras[0]->moveUp(0.01);
		}
		if (display.keys.F) {
			cameras[0]->moveDown(0.01);
		}


		if (display.keys.UP) {
			cameras[0]->rotateUp(camera_rotation_speed);
		}

		if (display.keys.DOWN) {
			cameras[0]->rotateDown(camera_rotation_speed);
		}
		if (display.keys.LEFT) {
			cameras[0]->rotateLeft(camera_rotation_speed);
		}
		if (display.keys.RIGHT) {
			cameras[0]->rotateRight(camera_rotation_speed);
		}
		// 	if (display.keys.C)
		// 		cameras[0]->resetView();

		// 	if (display.keys.SPACE)
		// 		models[0]->transform.gamma += 0.005;

		if (display.keys.O)
			models[0]->transform.alpha -= speed;

		if (display.keys.P)
			models[0]->transform.alpha += speed;

		if (display.keys.K)
			models[0]->transform.beta -=  speed;

		if (display.keys.L)
			models[0]->transform.beta +=  speed;

		if (display.keys.M)
			models[0]->transform.gamma += speed;
		if (display.keys.N)
			models[0]->transform.gamma -= speed;

		if (display.keys.EQUAL)
			speed *= 2.;
		if (display.keys.MINUS)
			speed /= 2.;
	}

	if ( camera_mode == FIXED_POINT)
	{

		if (display.keys.D)
			cameras[0]->fixedRotateRight(step);
		if (display.keys.A)
			cameras[0]->fixedRotateLeft(step);
		if (display.keys.W)
			cameras[0]->fixedRotateUp(step);
		if (display.keys.S)
			cameras[0]->fixedRotateDown(step);

		if (display.keys.UP)
			cameras[0]->zoomIn(step);
		if (display.keys.DOWN)
			cameras[0]->zoomOut(step);

		if (display.keys.EQUAL)
// 			step += 0.001;
			step *= 2.;
		if (display.keys.MINUS)
			if (step>=0.001)
// 				step -= 0.001;
				step /= 2.;

	}



// 	if (!render_off_screen)
// 		display.update();
}

void RenderEngine::linkShaderToModel(int shader_index, int model_index)
{
	models_shader[model_index] = shader_index;
}

void RenderEngine::linkTextureToShader(int texture_index, int shader_index)
{
	shaders_texture[shader_index] = texture_index;
}

void RenderEngine::linkCameraToShaders(int camera_index, int shader_index)
{
	shaders_camera[shader_index] = camera_index;
}

void RenderEngine::linkBasicCamerasToShader()
{
	/**
	 * linkuje dwie podstawowe kamery do shaderów
	 */
	for (int i = 0; i < shaders.size(); i++) {
		linkCameraToShaders(0, i);
		linkCameraToShaders(1, i);
	}
}

void RenderEngine::clearModels()
{
	for (int i = 0; i < models.size(); i++)
		delete models[i];

	models.clear();
	models_shader.clear();
}

int RenderEngine::addModel(const std::string& file_name)
{
//  	std::cout << "adding model...\n";
	Mesh *temp = new Mesh(file_name);
//  	std::cout << "still adding model...\n";
	models.push_back(temp);
//  	std::cout << "model added...\n";

	models_shader.push_back(0);

	return models.size() -1 ;
}

int RenderEngine::addModel(float vertices[][3], int num_pkt, int indices[][3],
			   														int num_tr)
{

	Mesh *temp = new Mesh(vertices, num_pkt, indices, num_tr);
	models.push_back(temp);

	models_shader.push_back(0);

	return models.size() -1 ;
}

int RenderEngine::addModelAsteroidFormat(const std::string& filename)
{
	/**
	 *  Funkcja wkładająca model do RenderEngine
	 * dla plików w formacie Bartczakowym (pierwsza linia ma liczbę punktów
	 * i liczbę tr)
	 */

	// dodać wczytywanie dodatkowego pliku, np file_name.uv albo cos
	// i jeśli istnieje to wpisuj koordynaty

	FILE *f = fopen(filename.c_str(), "r");
	if (f == NULL) {
		perror("Error");
	}

	bool texture_coords_available = false;
	std::string tex_filename = filename + ".uv";
	FILE *f_tex = fopen(tex_filename.c_str(), "r");
	if (f_tex == NULL) {
// 		perror("No texture coordinates .uv file");
		texture_coords_available = false;
	}
	else
		texture_coords_available = true;



	int num_indices, num_vertices;

	fscanf(f, "%d %d", &num_vertices, &num_indices);
	//printf("%d %d\n", num_vertices, num_indices);

	float vertices[num_vertices][3];
	int indices[num_indices][3];
	float texture_coords[num_vertices][2];

	for (int i = 0; i < num_vertices; i++) {
		fscanf(f, "%f %f %f ", &vertices[i][0], &vertices[i][1], &vertices[i][2]);
	}
	for (int i = 0; i < num_indices; i++) {
		fscanf(f, "%d %d %d ", &indices[i][0], &indices[i][1], &indices[i][2]);
	}
	fclose(f);

	if (texture_coords_available) {
		for (int i = 0; i < num_vertices; i++) {
			fscanf(f_tex, "%f %f ", &texture_coords[i][0], &texture_coords[i][1]);
		}
		fclose(f_tex);
	}

	if (texture_coords_available) {
		Mesh *temp = new Mesh(vertices, num_vertices, indices, num_indices,
																texture_coords);
		models.push_back(temp);
	}
	else {
		Mesh *temp = new Mesh(vertices, num_vertices, indices, num_indices);
		models.push_back(temp);
	}

	models_shader.push_back(0);

// 	printf("model %s added\n", filename.c_str());
	return models.size() -1 ;
}

int RenderEngine::addShader(const std::string& file_name)
{
	Shader *temp = new Shader(file_name);
	shaders.push_back(temp);

	shaders_camera.push_back(0);
	shaders_texture.push_back(0);

	// automatyczne dodanie uniform name "depth_map" do shadera
	textures[shadow_texture_id]->setUniform(*shaders[shaders.size() -1] ,
			"dddd", "depth_map");

	return shaders.size() -1 ;
}

int RenderEngine::addTexture(const std::string& file_name)
{
	Texture *temp = new Texture(file_name);
	textures.push_back(temp);
	return textures.size() -1 ;
}

int RenderEngine::addTexture(int w, int h)
{
	Texture *temp = new Texture(w ,h);
	textures.push_back(temp);
	return textures.size() -1 ;
}

int RenderEngine::addTexture(int w, int h, GLenum target, GLvoid* data,
		GLint internal_format, GLenum format, GLenum type)
{
	Texture *temp = new Texture(w ,h, target, data, internal_format, format,
			type);
	textures.push_back(temp);
	return textures.size() -1 ;
}


int RenderEngine::addFramebuffer(int w, int h)
{
	FrameBuffer *temp = new FrameBuffer(w,h);
	framebuffers.push_back(temp);
// 	framebuffers[framebuffers.size()-1].initialize(w, h);
	return framebuffers.size() -1 ;
}

int RenderEngine::addCamera(const glm::vec3 pos, glm::vec3 targ, float left,
			   	float right, float bottom, float top, float zNear, float zFar)
{
	Camera *temp = new Camera(pos, targ, left, right, bottom, top, zNear, zFar);
	cameras.push_back(temp);
// 	cameras[cameras.size()-1].initialize(pos, targ, left, right, top, bottom, zNear, zFar);
	return cameras.size() -1;
}

void RenderEngine::updateCamera(const glm::vec3 pos, glm::vec3 targ,
				float left, float right, float bottom, float top, float zNear,
			   	float zFar, int camera_id)
{
	Camera *temp = new Camera(pos, targ, left, right, bottom, top, zNear, zFar);
	cameras.at(camera_id) = temp;
	// 	cameras.push_back(temp);
}

int RenderEngine::addCamera(const glm::vec3 pos, glm::vec3 targ, float fov,
				float aspect, float zNear, float zFar)
{
	Camera *temp = new Camera(pos, targ, fov, aspect, zNear, zFar);
	cameras.push_back(temp);
// 	cameras[cameras.size()-1].initialize(pos, targ, fov, aspect, zNear, zFar);
	return cameras.size() -1;
}

RenderEngine::~RenderEngine()
{
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

