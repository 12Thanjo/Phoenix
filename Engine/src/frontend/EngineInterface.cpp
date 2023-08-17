#include "Engine.h"


#include <libs/glm/glm.h>

// TODO: proper lib
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// TODO: proper lib
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


#include "vulkan/Renderer.h"
#include "GLFW.h"
#include "inputs/InputManager.h"
#include "physics/PhysicsEngine.h"

#include "Logging.h"

#include <format>




namespace ph{

	struct MeshInfo{
		uint32_t vertex_offset;
		uint32_t index_offset;
		uint32_t index_count;
	};

	struct EngineBackend{
		GLFW::Window window{};
		vulkan::Renderer renderer{};
		InputManager input_manager{};
		PhysicsEngine physics{};

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
		this->backend = new EngineBackend();
	}
	
	EngineInterface::~EngineInterface() noexcept {
		delete this->backend;	
	}


	//////////////////////////////////////////////////////////////////////
	// engine

	auto EngineInterface::init() noexcept -> bool {
		if(GLFW::init() == false) return false;

		if(this->backend->window.create(1280, 720, "Phoenix Engine | Playground") == false){
			PH_FATAL("Failed to create Window");
			return false;
		}

		#if defined(PH_PLATFORM_WINDOWS)
			this->backend->window.use_darkmode();
		#endif


		if(this->backend->input_manager.init(this->backend->window) == false){
			PH_FATAL("Failed to initialize input manager");
			return false;
		}


		if(this->backend->renderer.init(&this->backend->window) == false){
			PH_FATAL("Failed to initialize renderer");
			return false;
		}



		if(this->backend->physics.init() == false){
			PH_FATAL("Failed to initialize physics");
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


			auto mesh_result = this->backend->renderer.create_mesh(vertices, indices);

			if(mesh_result.has_value() == false){
				PH_ERROR("Failed to create mesh");
				return false;
			}

			this->backend->mesh_infos_3D.emplace_back(mesh_result->first, mesh_result->second, static_cast<uint32_t>(indices.size()));

			this->backend->default_mesh_3D = Mesh3D{ static_cast<uint32_t>(this->backend->mesh_infos_3D.size() - 1) };
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


			auto mesh_result = this->backend->renderer.create_mesh_2D(vertices, indices);

			if(mesh_result.has_value() == false){
				PH_ERROR("Failed to create mesh");
				return false;
			}

			this->backend->mesh_infos_2D.emplace_back(mesh_result->first, mesh_result->second, static_cast<uint32_t>(indices.size()));

			this->backend->default_mesh_2D = Mesh2D{ static_cast<uint32_t>(this->backend->mesh_infos_2D.size() - 1) };
		}


		///////////////////////////////////
		// default texture

		{
			const auto pixels = std::array<byte, 4>{255, 255, 255, 255};
			auto texture_result = this->backend->renderer.create_texture(pixels, 1, 1, false);
			if(texture_result.has_value() == false){
				PH_ERROR("Failed to create default texture");
				return false;
			}

			this->backend->default_texture = TextureID{ static_cast<uint32_t>(*texture_result) };
		}



		PH_INFO("Initialzed: Phoenix Engine");
		return true;
	};




	auto EngineInterface::shutdown() noexcept -> void {
		this->backend->renderer.shutdown();
		this->backend->input_manager.shutdown();
		this->backend->window.destroy();

		GLFW::shutdown();

		PH_INFO("Shutdown: Phoenix Engine");
	};



	auto EngineInterface::execute() noexcept -> bool {

	    this->backend->window.set_resize_callback([&](int width, int height){
	    	bool resize_result = this->resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));

	    	if(resize_result == false){
	    		PH_FATAL("Engine failed to resize");
	    		// TODO: error handling
	    	}
	    });

	    this->backend->frame_start_time = evo::time::now();

	    // TODO: better check for if window should close
	    while(this->backend->window.should_close() == false){
	    	this->calculate_timings();

	    	this->backend->input_manager.update();
	    	GLFW::poll_events();

	    	if(this->backend->suspended == false){
	    		if(this->draw_frame() == false){
	    			PH_FATAL("Renderer failed to draw frame");
	    			return false;
	    		}
	    	}

	    	this->backend->window.set_title( std::format("Phoenix Engine | FPS: {:.0f}", 1.0f / this->backend->frame_time) );
	    };

	    return true;
	};




	auto EngineInterface::set_update_callback(Callback callback) noexcept -> void { this->backend->update_callback = callback; };


	auto EngineInterface::get_frame_time() const noexcept -> float { return this->backend->frame_time; };


	//////////////////////////////////////////////////////////////////////
	// inputs

	auto EngineInterface::is_mouse_down(int button) const noexcept -> bool {
		return this->backend->input_manager.is_mouse_down(static_cast<InputCodes::Mouse>(button));
	};

	auto EngineInterface::is_mouse_up(int button) const noexcept -> bool {
		return this->backend->input_manager.is_mouse_up(static_cast<InputCodes::Mouse>(button));
	};

	auto EngineInterface::was_mouse_down(int button) const noexcept -> bool {
		return this->backend->input_manager.was_mouse_down(static_cast<InputCodes::Mouse>(button));
	};

	auto EngineInterface::was_mouse_up(int button) const noexcept -> bool {
		return this->backend->input_manager.was_mouse_up(static_cast<InputCodes::Mouse>(button));
	};




	auto EngineInterface::is_key_down(int key) const noexcept -> bool {
		return this->backend->input_manager.is_key_down(static_cast<InputCodes::Key>(key));
	};

	auto EngineInterface::is_key_up(int key) const noexcept -> bool {
		return this->backend->input_manager.is_key_up(static_cast<InputCodes::Key>(key));
	};


	auto EngineInterface::was_key_down(int key) const noexcept -> bool {
		return this->backend->input_manager.was_key_down(static_cast<InputCodes::Key>(key));
	};

	auto EngineInterface::was_key_up(int key) const noexcept -> bool {
		return this->backend->input_manager.was_key_up(static_cast<InputCodes::Key>(key));
	};



	auto EngineInterface::mouse_x() const noexcept -> int32_t { return this->backend->input_manager.get_mouse_x(); };
	auto EngineInterface::mouse_y() const noexcept -> int32_t { return this->backend->input_manager.get_mouse_y(); };
	auto EngineInterface::mouse_dx() const noexcept -> int32_t { return this->backend->input_manager.get_mouse_dx(); };
	auto EngineInterface::mouse_dy() const noexcept -> int32_t { return this->backend->input_manager.get_mouse_dy(); };



	//////////////////////////////////////////////////////////////////////
	// renderer

	auto EngineInterface::set_render_callback_3D(Callback callback) noexcept -> void { this->backend->render_callback_3D = callback; };
	auto EngineInterface::set_render_callback_2D(Callback callback) noexcept -> void { this->backend->render_callback_2D = callback; };


	auto EngineInterface::bind_material(Material3D material) noexcept -> void {
		this->backend->renderer.bind_descriptor_set_3D(material.id);
	};

	auto EngineInterface::bind_material_2D(Material2D material) noexcept -> void {
		this->backend->renderer.bind_descriptor_set_2D(material.id);
	};


	auto EngineInterface::render_mesh(const glm::mat4& model, Mesh3D mesh) noexcept -> void {
		PH_ASSERT(mesh.id < this->backend->mesh_infos_3D.size(), "Invalid mesh id");

		this->backend->renderer.set_model_push_constant_3D(model);

		const auto& mesh_info = this->backend->mesh_infos_3D[mesh.id];
		this->backend->renderer.draw_indexed(mesh_info.index_count, mesh_info.index_offset, mesh_info.vertex_offset);
	};


	auto EngineInterface::render_mesh_2D(const glm::mat4& model) noexcept -> void {
		this->backend->renderer.set_model_push_constant_2D(model);

		const auto& mesh_info = this->backend->mesh_infos_2D[this->backend->default_mesh_2D.id];
		this->backend->renderer.draw_indexed(mesh_info.index_count, mesh_info.index_offset, mesh_info.vertex_offset);
	};



	auto EngineInterface::set_camera(const glm::mat4& transform) noexcept -> void {
		const auto [width, height] = this->backend->window.size();

		auto ubo = GlobalUBO3D{
			.view = transform,
			.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f),
		};
		ubo.proj[1][1] *= -1;

		this->backend->renderer.set_global_ubo_3D(&ubo);
	};


	auto EngineInterface::set_camera_2D(const glm::mat4& transform) noexcept -> void {
		const auto [width, height] = this->backend->window.size();
		float window_half_width = static_cast<float>(width);
		float window_half_height = static_cast<float>(height);

		auto ubo = GlobalUBO3D{
			.view = transform,
			.proj = glm::ortho(-window_half_width, window_half_width, window_half_height, -window_half_height, -1.0f, 1.0f),
		};

		this->backend->renderer.set_global_ubo_2D(&ubo);
	};





	//////////////////////////////////////////////////////////////////////
	// asset manager

	// TODO: remove and replace with Assimp
	auto EngineInterface::load_mesh(const char* filepath, Mesh3D* out_id) noexcept -> bool {
		// https://vulkan-tutorial.com/Loading_models
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath)){
			PH_ERROR(warn + err);
			PH_FATAL("Failed to load model");
			return false;
		}

		auto unique_vertices = std::unordered_map<vulkan::Vertex3D, uint32_t>{};

		auto vertices = std::vector<vulkan::Vertex3D>{};
		auto indices = std::vector<uint32_t>{};


		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				auto vertex = vulkan::Vertex3D{};

				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.tex_coord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};


				if(unique_vertices.count(vertex) == 0){
					unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(unique_vertices[vertex]);
			}
		}

		auto mesh_result = this->backend->renderer.create_mesh(vertices, indices);

		if(mesh_result.has_value() == false){
			PH_ERROR("Failed to create mesh");
			return false;
		}


		this->backend->mesh_infos_3D.emplace_back(mesh_result->first, mesh_result->second, static_cast<uint32_t>(indices.size()));


		*out_id = Mesh3D{ static_cast<uint32_t>(this->backend->mesh_infos_3D.size() - 1) };
		return true;
	};




	auto EngineInterface::load_texture(const char* filepath, TextureID* out_id) noexcept -> bool {
		stbi_set_flip_vertically_on_load(true);
		
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(filepath, &texWidth, &texHeight, &texChannels, 4);
		
		auto texture_result = this->backend->renderer.create_texture({pixels, size_t(texWidth * texHeight * 4)}, texWidth, texHeight, true);

		stbi_image_free(pixels);


		if(texture_result.has_value() == false){
			return false;
		}

		*out_id = TextureID{ static_cast<uint32_t>(*texture_result) };
		return true;
	};



	auto EngineInterface::set_material_color(Material3D material, glm::vec4 color) noexcept -> void {
		this->backend->renderer.set_instance_ubo_3D(material.id, &color);
	};

	auto EngineInterface::set_material_texture(Material3D material, TextureID texture) noexcept -> void {
		this->backend->renderer.set_instance_texture_3D(material.id, texture.id);
	};



	auto EngineInterface::set_material_color_2D(Material2D material, glm::vec4 color) noexcept -> void {
		this->backend->renderer.set_instance_ubo_2D(material.id, &color);
	};

	auto EngineInterface::set_material_texture_2D(Material2D material, TextureID texture) noexcept -> void {
		this->backend->renderer.set_instance_texture_2D(material.id, texture.id);
	};


	auto EngineInterface::get_cube_mesh() const noexcept -> Mesh3D { return this->backend->default_mesh_3D;	};
	auto EngineInterface::get_default_texture() const noexcept -> TextureID { return this->backend->default_texture; };



	//////////////////////////////////////////////////////////////////////
	// window

	auto EngineInterface::get_window_size(int* width, int* height) const noexcept -> void {
		std::tie(*width, *height) = this->backend->window.size();
	};


	auto EngineInterface::capture_mouse() noexcept -> void { this->backend->window.capture_mouse();	};
	auto EngineInterface::release_mouse() noexcept -> void { this->backend->window.release_mouse();	};



	//////////////////////////////////////////////////////////////////////
	// physics

	///////////////////////////////////
	// static

	auto EngineInterface::create_static_cube(
		glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
	) noexcept -> StaticCollider {

		return this->backend->physics.create_static_cube(position, scale, PhysicsMaterial{static_friction, dynamic_friction, restitution});
	};



	///////////////////////////////////
	// dynamic

	auto EngineInterface::create_dynamic_cube(
		glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
	) noexcept -> DynamicCollider {

		return this->backend->physics.create_dynamic_cube(position, scale, PhysicsMaterial{static_friction, dynamic_friction, restitution});
	};



	auto EngineInterface::set_dynamic_collider_density(DynamicCollider collider, float density) noexcept -> void {
		this->backend->physics.set_dynamic_collider_density(collider, density);
	};

	auto EngineInterface::set_dynamic_collider_mass(DynamicCollider collider, float mass) noexcept -> void {
		this->backend->physics.set_dynamic_collider_mass(collider, mass);
	};


	auto EngineInterface::get_dynamic_collider_transform(DynamicCollider collider) const noexcept -> glm::mat4 {
		return this->backend->physics.get_dynamic_collider_transform(collider);
	};



	///////////////////////////////////
	// character controller

	auto EngineInterface::create_character_controller(glm::vec3 position, float height, float radius) noexcept -> CharacterController {
		return this->backend->physics.create_character_controller(position, height, radius);
	};

	auto EngineInterface::get_character_controller_position(CharacterController controller) noexcept -> glm::vec3 {
		return this->backend->physics.get_character_controller_position(controller);
	};

	auto EngineInterface::character_controller_move(CharacterController controller, glm::vec3 direction, float dt) noexcept -> void {
		this->backend->physics.character_controller_move(controller, direction, dt);
	};


	//////////////////////////////////////////////////////////////////////
	// internal


	auto EngineInterface::resize() noexcept -> bool {
		const auto [width, height] = this->backend->window.size();
		return this->resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	};


	auto EngineInterface::resize(uint32_t width, uint32_t height) noexcept -> bool {
		if(width == 0 || height == 0){
    		PH_INFO("Window was minimized or resized to 0 - suspending rendering");
    		this->backend->suspended = true;
    		return true;
    	}


		if(this->backend->suspended){
			PH_INFO("Window was unminimized or resized - resuming rendering");
			this->backend->suspended = false;
		}

    	if(this->backend->renderer.resize(width, height) == false){
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
		vulkan::Renderer::FrameResult begin_result = this->backend->renderer.begin_frame();

		while(begin_result == vulkan::Renderer::FrameResult::NeedResize){
			if(this->resize() == false){
				PH_ERROR("Vulkan requested a resize when beginning frame, and resize failed");
				return false;
			}
			begin_result = this->backend->renderer.begin_frame();
		};

		if(begin_result == vulkan::Renderer::FrameResult::Error){
			PH_ERROR("Failed to begin frame");
			return false;
		}


		///////////////////////////////////
		// update

		this->backend->update_callback();

		this->backend->physics.simulate(this->backend->frame_time);
		// this->backend->physics.simulate(1.0f / 1000.0f);


		///////////////////////////////////
		// render

		this->backend->renderer.begin_render_pass_3D();
			this->backend->renderer.bind_vertex_buffer_3D();
			this->backend->renderer.bind_index_buffer_3D();

			this->backend->render_callback_3D();

		this->backend->renderer.end_render_pass_3D();


		this->backend->renderer.begin_render_pass_2D();
			this->backend->renderer.bind_vertex_buffer_2D();
			this->backend->renderer.bind_index_buffer_2D();

			this->backend->render_callback_2D();

		this->backend->renderer.end_render_pass_2D();


		///////////////////////////////////
		// end frame

		vulkan::Renderer::FrameResult end_result = this->backend->renderer.end_frame();

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
		const evo::time::Nanoseconds time_diff = now - this->backend->frame_start_time;


		this->backend->frame_time = static_cast<float>(time_diff) / static_cast<float>( static_cast<evo::time::Nanoseconds>(evo::time::Seconds{1}) );

		this->backend->frame_start_time = now;


		evo::time::Nanoseconds foo = 12;
	};
	

	
};