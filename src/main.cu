/** \file */
/** ******************** INSTRUKCJA OBSŁUGI: **********************
 *
 * 1. stworzyć silnik graficzny (klasa RenderEngine)
 * 2. stworzyć shader, co najmniej jeden (klasa Shader)
 * 3. dodać model planetoidy
 * 4. dodać kamery; pierwsza to obserwator, druga to światło
 * 5. zlinkować model z sharderem (funkcja linkShaderToModel)
 * 6. zlinkować kamery z shaderami (funkcja linkCameraToShader)
 * 7. stworzyć silnik do postprocessingu w CUDA (klasa Cuda)
 * 8. RenderEngine.renderScene tworzy scene ze wszystkich modeli
 *
******************************************************************/

// #include <cuda.h>
#define CUDA_VERSION 5000
#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>

#include <unistd.h>

#include <GL/glew.h>
//#include "/home/grzeslaff/instalki/glfw-3.1.2/include/GLFW/glfw3.h"
#include <GLFW/glfw3.h>
#include<X11/X.h>
#include<X11/Xlib.h>

#include <cuda_gl_interop.h>

#include "RenderEngine.h"
#include "Cuda.h"


int DISPLAY_WIDTH = 512 * 2;
int DISPLAY_HEIGHT = 512 * 2;

bool Mesh::normalize = true;
bool Mesh::normal_interpolation = true;

int main() {
	int k;
	clock_t start_t, end_t;
    double cpu_time_used, cpu_ticks;


//    int shadow_size = 1024 * 1;

    start_t = clock();



//     RenderEngine render(DISPLAY_WIDTH, DISPLAY_HEIGHT, false, true);		// brak wyśw., LICZY jasnosci
//     RenderEngine render(DISPLAY_WIDTH, DISPLAY_HEIGHT, true, false);		// wyswietla, NIE LICZY jasnosci
	RenderEngine render(DISPLAY_WIDTH, DISPLAY_HEIGHT, true, false, true);
	cudaGLSetGLDevice(0);

    int basicShader = render.addShader("basicShader");
//     int aster = render.addModel(punkty, num_vertices, trojkaty, num_indices);
    int aster = render.addModelAsteroidFormat("../data/hw1_3.obj");


    glm::vec3 asteroid_pos(0,2,0);
    asteroid_pos *= 10000.;
    render.models[aster]->transform.setAngles(20, 25,  0, 0);
    render.models[aster]->transform.setPosition(asteroid_pos);

    float d = 1., model_camera_distance, model_light_distance;
    glm::vec3 light_position(2., 0., 0.);
    light_position *= 10000.;
    glm::vec3 camera_position(0., 0., 0.);
    camera_position *= 10000.;
    model_camera_distance = glm::length(asteroid_pos - camera_position);
    model_light_distance = glm::length(asteroid_pos - light_position);
// // 	render.addCamera(camera_position, asteroid_pos, 70.0f, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, model_light_distance - 5, model_light_distance + 5);
    render.addCamera(camera_position, asteroid_pos, -d, d, -d, d,
						model_camera_distance - 1, model_camera_distance + 1);
    render.addCamera(light_position, asteroid_pos, -d, d, -d, d,
						model_light_distance - 1, model_light_distance + 1);
    render.linkShaderToModel(basicShader, aster);

    render.linkBasicCamerasToShader();




	/////////////////////////////////

// 	render.textures[render.shadow_texture_id]->setUniform(
// 			*render.shaders[basicShader], "color_shadow", "depth");


// 	render.addTexture(100, 100);


    ///////////// CUDA ////////////////////////
    Cuda cuda(render.textures[ render.fb_tx ]->color_texture, DISPLAY_WIDTH, DISPLAY_HEIGHT);


    /////////////////////// RENDEROWANIE SCENY /////////////////////////

    int ILOSC_SYMULACJI = 90;

    double jasnosci[30][360];
    int counter = 0;
//     render.render_off_screen = false;
//    while (!render.display.is_closed) {

	while (!render.display.is_closed)
	{
//     for (int k = 0; k < 2; k++) {
//      for (int k = 0; k < 90; k++) {
//         render.models[aster]->transform.gamma += 2.*M_PI/ILOSC_SYMULACJI;
        render.renderScene();

        model_camera_distance = glm::length(asteroid_pos - camera_position);
        model_light_distance = glm::length(asteroid_pos - light_position);
//         render.updateCamera(camera_position, asteroid_pos,
// 							-d, d, -d, d, model_camera_distance - 1,
// 							model_camera_distance + 1 , 0);


         if (render.render_off_screen)
             jasnosci[0][k] = cuda.calculateMagnitudes();

// 		sleep(1);
        counter++;
    }


    if (render.render_off_screen)
	{
		for (int i = 0; i < ILOSC_SYMULACJI; i++)
			printf("%d\t%lf\n", i, jasnosci[0][i]);
	}





    end_t = clock();


    cpu_ticks = (double) (end_t - start_t);
    cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
//	printf("computation time: %lf\n", cpu_time_used);

// 	for (int k = 0; k < 360; k++)
// 		printf("%.16lf\t", jasnosci[0][k]);
// 	puts("  ");


    return 0;
}



