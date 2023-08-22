#include "Engine.h"


#include <libs/glm/glm.h>



#include "vulkan/Renderer.h"
#include "GLFW.h"
#include "inputs/InputManager.h"
#include "physics/PhysicsEngine.h"

#include "asset_loaders/ImageLoader.h"
#include "asset_loaders/MeshLoader.h"
#include "asset_loaders/FontLoader.h"

#include "Logging.h"

#include <format>


namespace ph{

	struct MeshInfo{
		uint32_t vertex_offset;
		uint32_t index_offset;
		uint32_t index_count;
	};

	struct EngineData{
		GLFW::Window window{};
		vulkan::Renderer renderer{};
		InputManager input_manager{};
		PhysicsEngine physics{};

		assets::ImageLoader image_loader{};
		assets::MeshLoader mesh_loader{};
		assets::FontLoader font_loader{};

		bool suspended = false;

		std::function<void()> update_callback;
		std::function<void()> render_callback_3D;
		std::function<void()> render_callback_2D;

		std::vector<MeshInfo> mesh_infos_3D{};
		std::vector<MeshInfo> mesh_infos_2D{};

		Mesh3D default_mesh_3D;
		Mesh2D default_mesh_2D;
		TextureID default_texture;

		evo::time::Nanoseconds frame_start_time;
		float frame_time;
	};



	EngineInterface::EngineInterface() noexcept {
		this->data = new EngineData();
	}
	
	EngineInterface::~EngineInterface() noexcept {
		delete this->data;	
	}


	//////////////////////////////////////////////////////////////////////
	// engine

	auto EngineInterface::init() noexcept -> bool {
		if(GLFW::init() == false) return false;

		if(this->data->window.create(1280, 720, "Phoenix Engine | Playground") == false){
			PH_FATAL("Failed to create Window");
			return false;
		}

		#if defined(PH_PLATFORM_WINDOWS)
			this->data->window.use_darkmode();
		#endif


		if(this->data->input_manager.init(this->data->window) == false){
			PH_FATAL("Failed to initialize input manager");
			return false;
		}


		if(this->data->renderer.init(&this->data->window) == false){
			PH_FATAL("Failed to initialize renderer");
			return false;
		}



		if(this->data->physics.init() == false){
			PH_FATAL("Failed to initialize physics");
			return false;	
		}



		if(this->data->image_loader.init(&this->data->renderer) == false){
			PH_FATAL("Failed to initialize image loader");
			return false;	
		}


		if(this->data->mesh_loader.init() == false){
			PH_FATAL("Failed to initialize mesh loader");
			return false;	
		}


		if(this->data->font_loader.init() == false){
			PH_FATAL("Failed to initialize font loader");
			return false;	
		}



		///////////////////////////////////
		// default 3D mesh (cube)

		{
			const auto vertices = std::to_array<vulkan::Vertex3D>({
				{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f,  1.0f,  0.0f}},
				{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f,  1.0f,  0.0f}},
				{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f,  1.0f,  0.0f}},
				{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, {0.0f,  1.0f,  0.0f}},

				{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f,  0.0f}},
				{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f,  0.0f}},
				{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f,  0.0f}},
				{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f,  0.0f}},


				{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f,  0.0f,  0.0f}},
				{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, {1.0f,  0.0f,  0.0f}},
				{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f,  0.0f,  0.0f}},
				{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f,  0.0f,  0.0f}},

				{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
				{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
				{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},
				{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},


				{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, {0.0f,  0.0f,  1.0f}},
				{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f,  0.0f,  1.0f}},
				{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, {0.0f,  0.0f,  1.0f}},
				{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {0.0f,  0.0f,  1.0f}},

				{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f,  0.0f, -1.0f}},
				{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f,  0.0f, -1.0f}},
				{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f,  0.0f, -1.0f}},
				{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f,  0.0f, -1.0f}},
			});


			const auto indices = std::to_array<uint32_t>({
				 0,  2,  1,      0,  3,  2,
				 4,  5,  6,      4,  6,  7,

				 8,  9, 10,      9, 11, 10,
				12, 14, 13,     13, 14, 15,

				16, 18, 17,     17, 18, 19,
				20, 21, 22,     21, 23, 22
			});


			auto mesh_result = this->data->renderer.create_mesh_3D(vertices, indices);

			if(mesh_result.has_value() == false){
				PH_ERROR("Failed to create mesh");
				return false;
			}

			this->data->mesh_infos_3D.emplace_back(mesh_result->first, mesh_result->second, uint32_t(indices.size()));

			this->data->default_mesh_3D = Mesh3D{ uint32_t(this->data->mesh_infos_3D.size() - 1) };
		}



		///////////////////////////////////
		// default 2D mesh

		{
			const auto vertices = std::to_array<vulkan::Vertex2D>({
				vulkan::Vertex2D{{-0.5, -0.5}, {0, 0}},
				vulkan::Vertex2D{{ 0.5, -0.5}, {1, 0}},
				vulkan::Vertex2D{{-0.5,  0.5}, {0, 1}},
				vulkan::Vertex2D{{ 0.5,  0.5}, {1, 1}},
			});


			const auto indices = std::to_array<uint32_t>({
				0, 1, 2,
				1, 3, 2,
			});


			auto mesh_result = this->data->renderer.create_mesh_2D(vertices, indices);

			if(mesh_result.has_value() == false){
				PH_ERROR("Failed to create mesh");
				return false;
			}

			this->data->mesh_infos_2D.emplace_back(mesh_result->first, mesh_result->second, uint32_t(indices.size()));

			this->data->default_mesh_2D = Mesh2D{ uint32_t(this->data->mesh_infos_2D.size() - 1) };
		}


		///////////////////////////////////
		// default texture

		{
			const auto pixels = std::array<byte, 4>{255, 255, 255, 255};
			auto texture_result = this->data->renderer.create_texture(pixels, 1, 1, false);
			if(texture_result.has_value() == false){
				PH_ERROR("Failed to create default texture");
				return false;
			}

			this->data->default_texture = TextureID{ uint32_t(*texture_result) };
		}


		//////////////////////////////////////////////////////////////////////
		// temp:

		{
			assets::FontLoader::FontData pixels = this->data->font_loader.load_text("C:\\Windows\\Fonts\\trebuc.ttf").value();

			this->data->renderer.create_texture(pixels.data, pixels.width, pixels.height, false);
		}

		// temp:
		//////////////////////////////////////////////////////////////////////



		PH_INFO("Initialzed: Phoenix Engine");
		return true;
	};




	auto EngineInterface::shutdown() noexcept -> void {
		this->data->font_loader.shutdown();
		this->data->mesh_loader.shutdown();
		this->data->image_loader.shutdown();

		this->data->physics.shutdown();
		this->data->renderer.shutdown();
		this->data->input_manager.shutdown();

		this->data->window.destroy();

		GLFW::shutdown();

		PH_INFO("Shutdown: Phoenix Engine");
	};



	auto EngineInterface::execute() noexcept -> bool {

	    this->data->window.set_resize_callback([&](int width, int height){
	    	bool resize_result = this->resize(uint32_t(width), uint32_t(height));

	    	if(resize_result == false){
	    		PH_FATAL("Engine failed to resize");
	    		// TODO: error handling
	    	}
	    });

	    this->data->frame_start_time = evo::time::now();

	    // TODO: better check for if window should close
	    while(this->data->window.should_close() == false){
	    	this->calculate_timings();

	    	this->data->input_manager.update();
	    	GLFW::poll_events();

	    	if(this->data->suspended == false){
	    		if(this->draw_frame() == false){
	    			PH_FATAL("Renderer failed to draw frame");
	    			return false;
	    		}
	    	}

	    	this->data->window.set_title( std::format("Phoenix Engine | FPS: {:.0f}", 1.0f / this->data->frame_time) );
	    };

	    return true;
	};




	auto EngineInterface::set_update_callback(Callback callback) noexcept -> void { this->data->update_callback = callback; };


	auto EngineInterface::get_frame_time() const noexcept -> float { return this->data->frame_time; };


	//////////////////////////////////////////////////////////////////////
	// inputs

	auto EngineInterface::is_mouse_down(int button) const noexcept -> bool {
		return this->data->input_manager.is_mouse_down(static_cast<InputCodes::Mouse>(button));
	};

	auto EngineInterface::is_mouse_up(int button) const noexcept -> bool {
		return this->data->input_manager.is_mouse_up(static_cast<InputCodes::Mouse>(button));
	};

	auto EngineInterface::was_mouse_down(int button) const noexcept -> bool {
		return this->data->input_manager.was_mouse_down(static_cast<InputCodes::Mouse>(button));
	};

	auto EngineInterface::was_mouse_up(int button) const noexcept -> bool {
		return this->data->input_manager.was_mouse_up(static_cast<InputCodes::Mouse>(button));
	};




	auto EngineInterface::is_key_down(int key) const noexcept -> bool {
		return this->data->input_manager.is_key_down(static_cast<InputCodes::Key>(key));
	};

	auto EngineInterface::is_key_up(int key) const noexcept -> bool {
		return this->data->input_manager.is_key_up(static_cast<InputCodes::Key>(key));
	};


	auto EngineInterface::was_key_down(int key) const noexcept -> bool {
		return this->data->input_manager.was_key_down(static_cast<InputCodes::Key>(key));
	};

	auto EngineInterface::was_key_up(int key) const noexcept -> bool {
		return this->data->input_manager.was_key_up(static_cast<InputCodes::Key>(key));
	};



	auto EngineInterface::mouse_x() const noexcept -> int32_t { return this->data->input_manager.get_mouse_x(); };
	auto EngineInterface::mouse_y() const noexcept -> int32_t { return this->data->input_manager.get_mouse_y(); };
	auto EngineInterface::mouse_dx() const noexcept -> int32_t { return this->data->input_manager.get_mouse_dx(); };
	auto EngineInterface::mouse_dy() const noexcept -> int32_t { return this->data->input_manager.get_mouse_dy(); };



	//////////////////////////////////////////////////////////////////////
	// renderer

	auto EngineInterface::set_render_callback_3D(Callback callback) noexcept -> void { this->data->render_callback_3D = callback; };
	auto EngineInterface::set_render_callback_2D(Callback callback) noexcept -> void { this->data->render_callback_2D = callback; };


	auto EngineInterface::bind_material(Material3D material) noexcept -> void {
		this->data->renderer.bind_descriptor_set_3D(material.id);
	};

	auto EngineInterface::bind_material_2D(Material2D material) noexcept -> void {
		this->data->renderer.bind_descriptor_set_2D(material.id);
	};


	auto EngineInterface::render_mesh(const glm::mat4& model, Mesh3D mesh) noexcept -> void {
		PH_ASSERT(mesh.id < this->data->mesh_infos_3D.size(), "Invalid mesh id");

		this->data->renderer.set_model_push_constant_3D(model);

		const auto& mesh_info = this->data->mesh_infos_3D[mesh.id];
		this->data->renderer.draw_indexed(mesh_info.index_count, mesh_info.index_offset, mesh_info.vertex_offset);
	};


	auto EngineInterface::render_mesh_2D(const glm::mat4& model) noexcept -> void {
		this->data->renderer.set_model_push_constant_2D(model);

		const auto& mesh_info = this->data->mesh_infos_2D[this->data->default_mesh_2D.id];
		this->data->renderer.draw_indexed(mesh_info.index_count, mesh_info.index_offset, mesh_info.vertex_offset);
	};



	auto EngineInterface::set_camera(const glm::mat4& transform) noexcept -> void {
		const auto [width, height] = this->data->window.size();

		auto ubo = vulkan::GlobalUBO3D{
			.view = transform,
			.proj = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 1000.0f),
		};
		ubo.proj[1][1] *= -1;

		this->data->renderer.set_global_ubo_3D(&ubo);
	};


	auto EngineInterface::set_camera_2D(const glm::mat4& transform) noexcept -> void {
		const auto [width, height] = this->data->window.size();
		float window_half_width = float(width);
		float window_half_height = float(height);

		auto ubo = vulkan::GlobalUBO2D{
			.view = transform,
			.proj = glm::ortho(-window_half_width, window_half_width, window_half_height, -window_half_height, -1.0f, 1.0f),
		};

		this->data->renderer.set_global_ubo_2D(&ubo);
	};





	//////////////////////////////////////////////////////////////////////
	// asset manager

	auto EngineInterface::load_mesh(const char* filepath, Mesh3D* out_id) noexcept -> bool {

		const auto load_mesh_result = this->data->mesh_loader.load_mesh(filepath);

		if(load_mesh_result.has_value() == false){
			PH_ERROR("Failed to load mesh");
			return false;
		}

		const auto& vertices = load_mesh_result->first;
		const auto& indices = load_mesh_result->second;

		auto renderer_create_mesh_result = this->data->renderer.create_mesh_3D(vertices, indices);

		if(renderer_create_mesh_result.has_value() == false){
			PH_ERROR("Failed to create mesh");
			return false;
		}


		this->data->mesh_infos_3D.emplace_back(renderer_create_mesh_result->first, renderer_create_mesh_result->second, uint32_t(indices.size()));


		*out_id = Mesh3D{ uint32_t(this->data->mesh_infos_3D.size() - 1) };
		return true;
	};




	auto EngineInterface::load_texture(const char* filepath, TextureID* out_id) noexcept -> bool {
		auto image_result = this->data->image_loader.load_image(filepath);

		if(image_result.has_value() == false){ return false; }

		*out_id = *image_result;
		return true;
	};



	auto EngineInterface::set_material_color(Material3D material, glm::vec4 color) noexcept -> void {
		this->data->renderer.set_instance_ubo_3D(material.id, &color);
	};

	auto EngineInterface::set_material_texture(Material3D material, TextureID texture) noexcept -> void {
		this->data->renderer.set_instance_texture_3D(material.id, texture.id);
	};



	auto EngineInterface::set_material_color_2D(Material2D material, glm::vec4 color) noexcept -> void {
		this->data->renderer.set_instance_ubo_2D(material.id, &color);
	};

	auto EngineInterface::set_material_texture_2D(Material2D material, TextureID texture) noexcept -> void {
		this->data->renderer.set_instance_texture_2D(material.id, texture.id);
	};


	auto EngineInterface::get_cube_mesh() const noexcept -> Mesh3D { return this->data->default_mesh_3D;	};
	auto EngineInterface::get_default_texture() const noexcept -> TextureID { return this->data->default_texture; };



	//////////////////////////////////////////////////////////////////////
	// window

	auto EngineInterface::get_window_size(int* width, int* height) const noexcept -> void {
		std::tie(*width, *height) = this->data->window.size();
	};


	auto EngineInterface::capture_mouse() noexcept -> void { this->data->window.capture_mouse();	};
	auto EngineInterface::release_mouse() noexcept -> void { this->data->window.release_mouse();	};



	//////////////////////////////////////////////////////////////////////
	// physics

	///////////////////////////////////
	// static

	auto EngineInterface::create_static_cube(
		glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
	) noexcept -> StaticCollider {

		return this->data->physics.create_static_cube(position, scale, PhysicsMaterial{static_friction, dynamic_friction, restitution});
	};



	///////////////////////////////////
	// dynamic

	auto EngineInterface::create_dynamic_cube(
		glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
	) noexcept -> DynamicCollider {

		return this->data->physics.create_dynamic_cube(position, scale, PhysicsMaterial{static_friction, dynamic_friction, restitution});
	};



	auto EngineInterface::set_dynamic_collider_density(DynamicCollider collider, float density) noexcept -> void {
		this->data->physics.set_dynamic_collider_density(collider, density);
	};

	auto EngineInterface::set_dynamic_collider_mass(DynamicCollider collider, float mass) noexcept -> void {
		this->data->physics.set_dynamic_collider_mass(collider, mass);
	};


	auto EngineInterface::get_dynamic_collider_transform(DynamicCollider collider) const noexcept -> glm::mat4 {
		return this->data->physics.get_dynamic_collider_transform(collider);
	};



	///////////////////////////////////
	// character controller

	auto EngineInterface::create_character_controller(glm::vec3 position, float height, float radius) noexcept -> CharacterController {
		return this->data->physics.create_character_controller(position, height, radius);
	};

	auto EngineInterface::get_character_controller_position(CharacterController controller) const noexcept -> glm::vec3 {
		return this->data->physics.get_character_controller_position(controller);
	};

	auto EngineInterface::character_controller_move(CharacterController controller, glm::vec3 direction, float dt) noexcept -> void {
		this->data->physics.character_controller_move(controller, direction, dt);
	};


	auto EngineInterface::is_character_controller_grounded(CharacterController controller) const noexcept -> bool {
		return this->data->physics.get_character_controller_grounded(controller);
	};


	//////////////////////////////////////////////////////////////////////
	// internal


	auto EngineInterface::resize() noexcept -> bool {
		const auto [width, height] = this->data->window.size();
		return this->resize(uint32_t(width), uint32_t(height));
	};


	auto EngineInterface::resize(uint32_t width, uint32_t height) noexcept -> bool {
		if(width == 0 || height == 0){
    		PH_INFO("Window was minimized or resized to 0 - suspending rendering");
    		this->data->suspended = true;
    		return true;
    	}


		if(this->data->suspended){
			PH_INFO("Window was unminimized or resized - resuming rendering");
			this->data->suspended = false;
		}

    	if(this->data->renderer.resize(width, height) == false){
    		PH_ERROR("Failed to resize renderer");
    		return false;
    	}

    	if(this->draw_frame() == false){
    		PH_ERROR("Failed to draw frame after resize");
    		return false;
    	}

    	PH_TRACE("Engine resized");
    	return true;
	};




	auto EngineInterface::draw_frame() noexcept -> bool {
		vulkan::Renderer::FrameResult begin_result = this->data->renderer.begin_frame();

		while(begin_result == vulkan::Renderer::FrameResult::NeedResize){
			if(this->resize() == false){
				PH_ERROR("Vulkan requested a resize when beginning frame, and resize failed");
				return false;
			}
			begin_result = this->data->renderer.begin_frame();
		};

		if(begin_result == vulkan::Renderer::FrameResult::Error){
			PH_ERROR("Failed to begin frame");
			return false;
		}


		///////////////////////////////////
		// update

		this->data->update_callback();

		this->data->physics.simulate(this->data->frame_time);
		// this->data->physics.simulate(1.0f / 1000.0f);


		///////////////////////////////////
		// render

		this->data->renderer.begin_render_pass_3D();
			this->data->renderer.bind_vertex_buffer_3D();
			this->data->renderer.bind_index_buffer_3D();

			this->data->render_callback_3D();

		this->data->renderer.end_render_pass_3D();


		this->data->renderer.begin_render_pass_2D();
			this->data->renderer.bind_vertex_buffer_2D();
			this->data->renderer.bind_index_buffer_2D();

			this->data->render_callback_2D();

		this->data->renderer.end_render_pass_2D();


		///////////////////////////////////
		// end frame

		vulkan::Renderer::FrameResult end_result = this->data->renderer.end_frame();

		if(end_result == vulkan::Renderer::FrameResult::Error){
			PH_ERROR("Failed to end frame");
			return false;

		}else if(end_result == vulkan::Renderer::FrameResult::NeedResize){
			if(this->resize() == false){
				PH_ERROR("Vulkan requested a resize when ending frame, and resize failed");
				return false;
			}
		}

		return true;
	};



	auto EngineInterface::calculate_timings() noexcept -> void {
		const evo::time::Nanoseconds now = evo::time::now();
		const evo::time::Nanoseconds time_diff = now - this->data->frame_start_time;


		this->data->frame_time = float(time_diff) / float( static_cast<evo::time::Nanoseconds>(evo::time::Seconds{1}) );

		this->data->frame_start_time = now;


		evo::time::Nanoseconds foo = 12;
	};
	

	
};