#include "ph_pch.h"

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// #include <Phoenix.h>



// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
// float WINDOW_WIDTH = 1280.0f;
// float WINDOW_HEIGHT = 720.0f;


// Phoenix::Camera camera(glm::radians(60.0f), WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);


// bool first_mouse = true;
// float last_x = WINDOW_WIDTH / 2.0f;
// float last_y = WINDOW_HEIGHT / 2.0f;

// float delta_time = 0.0f;
// float last_frame = 0.0f;


// //////////////////////////////////////////////////////////////////////////////////////////////////////////////

// void run_inputs(GLFWwindow* _window_get){
// 	float camera_speed = 2.5f * delta_time;

// 	     if(glfwGetKey(_window_get, GLFW_KEY_W) == GLFW_PRESS){ camera.moveForward(camera_speed); }
// 	else if(glfwGetKey(_window_get, GLFW_KEY_S) == GLFW_PRESS){ camera.moveBackward(camera_speed); }

// 		 if(glfwGetKey(_window_get, GLFW_KEY_A) == GLFW_PRESS){ camera.moveLeft(camera_speed); }
// 	else if(glfwGetKey(_window_get, GLFW_KEY_D) == GLFW_PRESS){ camera.moveRight(camera_speed); }

// 	     if(glfwGetKey(_window_get, GLFW_KEY_SPACE) == GLFW_PRESS){ camera.moveUp(camera_speed); }
// 	else if(glfwGetKey(_window_get, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){ camera.moveDown(camera_speed); }
// }





// //////////////////////////////////////////////////////////////////////////////////////////////////////////////

// int main(int argc, char const *argv[]){
// 	Phoenix::_logging::init();

// 	// create the _window_get
// 	Phoenix::WindowConfig win_conf;
// 		win_conf.width = WINDOW_WIDTH;
// 		win_conf.height = WINDOW_HEIGHT;
// 		win_conf.captureMouse = true;
// 	Phoenix::Window window(win_conf);
// 	GLFWwindow* _window_get = window.win();


// 	glfwSetFramebufferSizeCallback(_window_get, [](GLFWwindow* _window_get, int width, int height){
// 		if(width != 0 && height != 0){
// 			PH_LOG("Window Resized to " << width << " / " << height);

// 			glViewport(0, 0, width, height);


// 			WINDOW_WIDTH = (float)width;
// 			WINDOW_HEIGHT = (float)height;

// 			camera.setProjection(camera.getFOV(), WINDOW_WIDTH / WINDOW_HEIGHT, camera.getNear(), camera.getFar());

			

// 			// resize the frame buffer...
// 		}

// 	});


// 	glfwSetCursorPosCallback(_window_get, [](GLFWwindow* window_context, double x, double y){
// 		if(first_mouse){
// 			last_x = (float)x;
// 			last_y = (float)y;
// 			first_mouse = false;
// 		}

// 		float x_offset = (float)x - last_x;
// 		float y_offset = last_y - (float)y;
// 		last_x = (float)x;
// 		last_y = (float)y;

// 		float sensitivity = 0.0015f;
// 		x_offset *= sensitivity;
// 		y_offset *= sensitivity;


// 		glm::vec3 camera_rotation = camera.getRotation();
// 		camera_rotation.y += x_offset;
// 		camera_rotation.z += y_offset;

// 		if(camera_rotation.z > 1.57f){
// 			camera_rotation.z = 1.57f;
// 		}else if(camera_rotation.z < -1.57f){
// 			camera_rotation.z = -1.57f;
// 		};

// 		camera.setRotation(camera_rotation);

// 	});

// 	glfwSetScrollCallback(_window_get, [](GLFWwindow* window_context, double x, double y){
// 		glm::vec3 rotation = camera.getRotation();

// 		rotation.x += glm::radians((float)y);
// 		camera.setRotation(rotation);

// 	});


// 	camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
// 	camera.setRotation(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));



// 	Phoenix::Shader box_shader("assets/shaders/basic lighting.shader");
// 	Phoenix::Shader skybox_shader("assets/shaders/skybox.shader");
// 	Phoenix::Shader postprocessing_shader("assets/shaders/postprocessing.shader");
// 	Phoenix::Shader color_shader("assets/shaders/color.shader");



// 	// vertex data, buffers, and attributes
// 	float verticies[] = {
// 		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
// 		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
// 		 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
// 		-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,     0.0f,  1.0f,  0.0f,

// 		-0.5f, -0.5f, -0.5f,        0.0f, 0.0f,     0.0f, -1.0f,  0.0f,
// 		 0.5f, -0.5f, -0.5f,        1.0f, 0.0f,     0.0f, -1.0f,  0.0f,
// 		 0.5f, -0.5f,  0.5f,        1.0f, 1.0f,     0.0f, -1.0f,  0.0f,
// 		-0.5f, -0.5f,  0.5f,        0.0f, 1.0f,     0.0f, -1.0f,  0.0f,


// 		 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
// 		 0.5f,  0.5f,  0.5f,		0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
// 		 0.5f, -0.5f, -0.5f,        1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
// 		 0.5f, -0.5f,  0.5f,        0.0f, 0.0f,     1.0f,  0.0f,  0.0f,

// 		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
// 		-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,    -1.0f,  0.0f,  0.0f,
// 		-0.5f, -0.5f, -0.5f,        0.0f, 0.0f,    -1.0f,  0.0f,  0.0f,
// 		-0.5f, -0.5f,  0.5f,        1.0f, 0.0f,    -1.0f,  0.0f,  0.0f,



// 		 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
// 		 0.5f, -0.5f,  0.5f,        1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
// 		-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
// 		-0.5f, -0.5f,  0.5f,        0.0f, 0.0f,    0.0f,  0.0f,  1.0f,

// 		 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
// 		 0.5f, -0.5f, -0.5f,        0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
// 		-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
// 		-0.5f, -0.5f, -0.5f,        1.0f, 0.0f,    0.0f,  0.0f, -1.0f,

// 	};

// 	uint32_t indicies[] = {
// 		 0,  1,  2,		 0,  2,  3,
// 		 4,  5,  6,		 4,  6,  7,

// 		 8,  9, 10,		 9, 10, 11,
// 		12, 13, 14,		13, 14, 15,

// 		16, 17, 18,		17, 18, 19,
// 		20, 21, 22,		21, 22, 23
// 	};


// 	// create buffers
// 	Phoenix::VertexArray vertex_array{};

// 	Phoenix::VertexBuffer vertex_buffer(verticies, sizeof(verticies));
// 	vertex_buffer.layout<float>(3); // positions
// 	vertex_buffer.layout<float>(2);	// texture coordinates
// 	vertex_buffer.layout<float>(3); // normals
// 	vertex_array.addVertexBuffer(vertex_buffer);


// 	Phoenix::IndexBuffer index_buffer(indicies, sizeof(indicies));
// 	// vertex_array.addIndexBuffer(index_buffer);





// 	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
// 	float skybox_verticies[] = {
// 		-100000.0f,  100000.0f, -100000.0f,		0.25f,    0.99f,
// 		 100000.0f,  100000.0f, -100000.0f,		0.25f,    0.664f,
// 		 100000.0f,  100000.0f,  100000.0f,		0.49991f, 0.664f,
// 		-100000.0f,  100000.0f,  100000.0f,		0.49991f, 0.99f,

// 		-100000.0f, -100000.0f, -100000.0f,     0.25f,    0.0f,
// 		 100000.0f, -100000.0f, -100000.0f,     0.25f,    0.3334f,
// 		 100000.0f, -100000.0f,  100000.0f,     0.5f,     0.3334f,
// 		-100000.0f, -100000.0f,  100000.0f,     0.5f,     0.0f,


// 		 100000.0f,  100000.0f, -100000.0f,		0.25f, 	  0.664f,
// 		 100000.0f,  100000.0f,  100000.0f,		0.5f,  	  0.664f,
// 		 100000.0f, -100000.0f, -100000.0f,     0.25f, 	  0.3334f,
// 		 100000.0f, -100000.0f,  100000.0f,     0.5f,  	  0.3334f,

// 		-100000.0f,  100000.0f, -100000.0f,		1.0f,     0.664f,
// 		-100000.0f,  100000.0f,  100000.0f,		0.75f,    0.664f,
// 		-100000.0f, -100000.0f, -100000.0f,     1.0f,     0.3334f,
// 		-100000.0f, -100000.0f,  100000.0f,     0.75f,    0.3334f,


// 		 100000.0f,  100000.0f,  100000.0f,		0.5f,     0.664f,
// 		 100000.0f, -100000.0f,  100000.0f,     0.5f,     0.3334f,
// 		-100000.0f,  100000.0f,  100000.0f,		0.75f,    0.664f,
// 		-100000.0f, -100000.0f,  100000.0f,     0.75f, 	  0.3334f,

// 		 100000.0f,  100000.0f, -100000.0f,		0.25f,    0.664f,
// 		 100000.0f, -100000.0f, -100000.0f,     0.25f,    0.3334f,
// 		-100000.0f,  100000.0f, -100000.0f,		0.0f, 	  0.664f,
// 		-100000.0f, -100000.0f, -100000.0f,     0.0f,     0.3334f,
// 	};



// 	uint32_t skybox_indicies[] = {
// 		 0,  1,  2,		 0,  2,  3,
// 		 4,  5,  6,		 4,  6,  7,

// 		 8,  9, 10,		 9, 10, 11,
// 		12, 13, 14,		13, 14, 15,

// 		16, 17, 18,		17, 18, 19,
// 		20, 21, 22,		21, 22, 23
// 	};


// 	Phoenix::VertexArray skybox_va{};

// 	Phoenix::VertexBuffer skybox_vb(skybox_verticies, sizeof(skybox_verticies));
// 	skybox_vb.layout<float>(3);
// 	skybox_vb.layout<float>(2);
// 	skybox_va.addVertexBuffer(skybox_vb);

// 	Phoenix::IndexBuffer skybox_ib(skybox_indicies, sizeof(skybox_indicies));



// 	///////////////////////////////////////////////////////////////////////////////////////////////////////


// 	// wireframe
// 	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// 	float screen_verticies[] = {
// 		-1.0f,  1.0f,	0.0f, 1.0f,
// 		-1.0f, -1.0f,	0.0f, 0.0f,
// 		 1.0f, -1.0f,	1.0f, 0.0f,
// 		 1.0f,  1.0f,	1.0f, 1.0f,
// 	};


// 	uint32_t screen_indicies[]{
// 		0, 1, 2,
// 		0, 2, 3
// 	};

// 	Phoenix::VertexArray screen_va{};

// 	Phoenix::VertexBuffer screen_vb(screen_verticies, sizeof(screen_verticies));
// 	screen_vb.layout<float>(2);
// 	screen_vb.layout<float>(2);
// 	screen_va.addVertexBuffer(screen_vb);

// 	Phoenix::IndexBuffer screen_ib(screen_indicies, sizeof(screen_indicies));

// 	///////////////////////////////////////////////////////////////////////////////////////////////////////


	
// 	float plane_verticies[] = {
// 		-5.0f, -1.0f, -5.0f,
// 		-5.0f, -1.0f,  5.0f,
// 		 5.0f, -1.0f, -5.0f,


// 		-5.0f, -1.0f,  5.0f,
// 		 5.0f, -1.0f, -5.0f,
// 		 5.0f, -1.0f,  5.0f,
// 	};

// 	Phoenix::VertexArray plane_va{};

// 	Phoenix::VertexBuffer plane_vb(plane_verticies, sizeof(plane_verticies));
// 	plane_vb.layout<float>(3);
// 	plane_va.addVertexBuffer(plane_vb);


// 	///////////////////////////////////////////////////////////////////////////////////////////////////////

// 	Phoenix::Lights::Directional sun{};
// 	Phoenix::Lights::Point red_light({-0.6f, -0.6f, -0.6f});
// 		red_light._ambient = {1.0f, 0.0f, 0.0f};
// 		red_light._diffuse = {1.0f, 0.0f, 0.0f};
// 		red_light._specular = {1.0f, 0.0f, 0.0f};


// 	glm::vec3 cam_position(0.0f, 0.0f, -1.0f);
// 	glm::vec3 cam_rotation(0.0f, 0.0f, 0.0f);

// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



// 	// MSAA
// 	Phoenix::FrameBufferConfig msaa_config;
// 		msaa_config.attachments = { Phoenix::FrameBufferTextureFormat::RGBA8, Phoenix::FrameBufferTextureFormat::Depth };
// 		msaa_config.width = (GLint)WINDOW_WIDTH;
// 		msaa_config.height = (GLint)WINDOW_HEIGHT;
// 		msaa_config.samples = 4;
// 	Phoenix::FrameBuffer msaa_buffer(msaa_config);


// 	// postprocessing / output
// 	Phoenix::FrameBufferConfig output_config;
// 		output_config.attachments = { Phoenix::FrameBufferTextureFormat::RGBA8};
// 		output_config.width = (GLint)WINDOW_WIDTH;
// 		output_config.height = (GLint)WINDOW_HEIGHT;
// 	Phoenix::FrameBuffer output_buffer(output_config);	






// 	Phoenix::Texture box_texture("assets/textures/test texture.png");
// 	Phoenix::Texture skybox_texture("assets/environment/skybox.png");



// 	///////////////////////////////////////////////////////////////////////////////////////////////

// 	// render loop
// 	while(!glfwWindowShouldClose(_window_get)){
// 		float current_frame = (float)glfwGetTime();
// 		delta_time = current_frame - last_frame;
// 		last_frame = current_frame;

// 		// glfwPollEvents();
// 		window.pollEvents();
// 		run_inputs(_window_get);


// 		// glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
// 		msaa_buffer.bind();
// 		glEnable(GL_DEPTH_TEST); 


// 		// render
// 		glClearColor(0.05f, 0.1f, 0.15f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


// 		/////////////////////////////////////////////////////////////////////////
// 		// draw box

// 		box_shader.bind();

// 		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
// 		box_shader.uploadMat4("u_model", model);
// 		box_shader.uploadMat4("u_view_projection", camera.getViewProjection());
// 		box_shader.uploadFloat3("u_camera_position", camera.getPosition());


// 		box_texture.bind(2);
// 		box_shader.uploadInt("u_texture", 2);

// 		sun.upload(box_shader);
// 		red_light.upload(box_shader, 0);
// 		box_shader.uploadInt("u_num_point_lights", 1);

// 		box_shader.uploadFloat("shininess", 32);


// 		vertex_array.bind();
// 		glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, 0);
// 		vertex_array.unbind();

// 		/////////////////////////////////////////////////////////////////////////
// 		// draw plane

// 		color_shader.bind();
// 		color_shader.uploadMat4("u_model", glm::mat4(1.0f));
// 		color_shader.uploadMat4("u_view_projection", camera.getViewProjection());


// 		plane_va.bind();
// 		glDrawArrays(GL_TRIANGLES, 0, 6);
// 		plane_va.unbind();

// 		/////////////////////////////////////////////////////////////////////////
// 		// draw skybox

// 		glDepthFunc(GL_LEQUAL);
// 			skybox_shader.bind();


// 			skybox_shader.uploadMat4("u_model", glm::mat4(1.0f));
// 			skybox_shader.uploadMat4("u_view_projection", camera.getViewProjection());


// 			skybox_texture.bind(3);
// 			skybox_shader.uploadInt("u_texture", 3);


// 			skybox_va.bind();
// 			glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, 0);
// 			skybox_va.unbind();
// 		glDepthFunc(GL_LESS);

// 		/////////////////////////////////////////////////////////////////////////
// 		// copy

// 		glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa_buffer.getID());
// 		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output_buffer.getID());
// 		glBlitFramebuffer(0, 0, (GLint)WINDOW_WIDTH, (GLint)WINDOW_HEIGHT, 0, 0, (GLint)WINDOW_WIDTH, (GLint)WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);


// 		/////////////////////////////////////////////////////////////////////////


// 		glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT);
// 		glDisable(GL_DEPTH_TEST);



// 		// output_buffer.bind();

// 		postprocessing_shader.bind();
// 		screen_va.bind();
// 		// glActiveTexture(GL_TEXTURE0);
// 		// glBindTexture(GL_TEXTURE_2D, screenTexture);
// 		output_buffer.textureBind(0);

// 		// postprocessing_shader.uploadInt("u_texture", 0);

// 		glDrawElements(GL_TRIANGLES, sizeof(screen_indicies), GL_UNSIGNED_INT, 0);

// 		/////////////////////////////////////////////////////////////////////////

// 		// glDrawArrays(GL_TRIANGLES, 0, 6);

// 		// glfw swap buffers and poll I/O events
// 		// glfwSwapBuffers(_window_get);
// 		window.render();
// 	};


// 	// glfwTerminate();

// 	return 0;
// }


