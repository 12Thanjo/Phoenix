#include "ph_pch.h"
#include "Renderer.h"



namespace Phoenix{

	Renderer::Renderer(unsigned int width, unsigned int height){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		PH_INFO("Renderer Initialized");
		
		renderer_2d.init(width, height);
	}

	void Renderer::begin_scene(){
		clear_perf_metrics();
		renderer_2d.begin_scene();
	}

	void Renderer::render(){
		renderer_2d.render();
	}

	void Renderer::end_scene(){
		renderer_2d.end_scene();
		update_perf_metrics();
	}


	void Renderer::set_camera_projection(float width, float height){
		renderer_2d.set_projection(width, height);
	}
	 
}
