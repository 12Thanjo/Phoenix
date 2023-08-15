#include "game.h"


#include <Phoenix/Phoenix.h>


namespace Game{


	constexpr ph::MaterialID ground_material = 0;
	constexpr ph::MaterialID wall_material = 1;
	constexpr ph::MaterialID player_material = 2;

	constexpr ph::MaterialID crosshair_material = 0;



	struct Player{
		ph::Transform transform{};
		float speed = 7.0f;
	};



	static auto setup_materials(ph::Engine& engine) noexcept -> void {
		
		engine.assets.setMaterialColor(ground_material, {0.3f, 0.3f, 0.3f, 1.0f});
		engine.assets.setMaterialTexture(ground_material, engine.assets.getDefaultTexture());

		engine.assets.setMaterialColor(wall_material, {0.5f, 0.5f, 0.5f, 1.0f});
		engine.assets.setMaterialTexture(wall_material, engine.assets.getDefaultTexture());

		engine.assets.setMaterialColor(player_material, {0.0f, 0.08f, 0.3f, 1.0f});
		engine.assets.setMaterialTexture(player_material, engine.assets.getDefaultTexture());


		engine.assets.setMaterialColor2D(crosshair_material, {1.0f, 1.0f, 1.0f, 1.0f});
		engine.assets.setMaterialTexture2D(crosshair_material, engine.assets.getDefaultTexture());
	};


	static auto render_map(ph::Engine& engine) noexcept -> void {
		
		const auto ground_transform = ph::Transform{
			.position = {0.0f, -0.5f, 0.0f},
			.scale = {50.0f, 1.0f, 50.0f},
		};

		engine.renderer.bindMaterial(ground_material);
		engine.renderer.drawMesh(ground_transform.calculate(), engine.assets.getCubeMesh());


		engine.renderer.bindMaterial(wall_material);

		engine.renderer.drawMesh(
			ph::Transform{.position = {10.0f, 5.0f, -10.0f}, .scale = {1.0f, 10.0f, 1.0f}}.calculate(),
			engine.assets.getCubeMesh()
		);


		engine.renderer.drawMesh(
			ph::Transform{.position = {-10.0f, 5.0f, -10.0f}, .scale = {1.0f, 10.0f, 1.0f}}.calculate(),
			engine.assets.getCubeMesh()
		);

	};







	auto run() noexcept -> void {
		auto engine = ph::Engine();

		engine.init();


		auto camera = ph::Camera{
			.position = {0.35f + 0.5f, 0.55f, 3.2f},
		};

		auto player = Player{
			.transform = ph::Transform{
				.position = {-2.0f, 1.0f, 70.0f},
				.scale = {1.0f, 2.0f, 0.2f},
			},
		};


		// const ph::MeshID tank = engine.assets.loadMesh("./assets/models/tank.obj").value();



		setup_materials(engine);

		engine.window.captureMouse();


		engine.setUpdateCallback([&](){
			     if(engine.inputs.isDown(ph::Inputs::Key::Escape)){ engine.window.releaseMouse(); }
			else if(engine.inputs.isDown(ph::Inputs::Mouse::Left)){ engine.window.captureMouse(); }


			player.transform.rotation.y -= engine.inputs.mouseDX() * engine.getFrameTime();


			auto player_velocity = glm::vec3{0.0f};

			const glm::mat4 player_current_transform = player.transform.calculate();

			     if(engine.inputs.isDown(ph::Inputs::Key::W)){ player_velocity += ph::getTransformForward(player_current_transform); }
			else if(engine.inputs.isDown(ph::Inputs::Key::S)){ player_velocity += ph::getTransformBackward(player_current_transform); }

			     if(engine.inputs.isDown(ph::Inputs::Key::A)){ player_velocity += ph::getTransformLeft(player_current_transform); }
			else if(engine.inputs.isDown(ph::Inputs::Key::D)){ player_velocity += ph::getTransformRight(player_current_transform); }

			if(player_velocity.x != 0 || player_velocity.y != 0 || player_velocity.z != 0){
				player_velocity = glm::normalize(player_velocity);
			}

			player.transform.position += player_velocity * player.speed * engine.getFrameTime();
		});



		
		engine.renderer.setCallback3D([&](){

			engine.renderer.setCamera(camera.calculate() * ph::Camera{player.transform.position, player.transform.rotation}.calculate());


			engine.renderer.bindMaterial(player_material);
			engine.renderer.drawMesh(player.transform.calculate(), engine.assets.getCubeMesh());


			render_map(engine);
		});




		engine.renderer.setCallback2D([&](){
			engine.renderer.setCamera2D(glm::mat4{1.0f});

			engine.renderer.bindMaterial2D(crosshair_material);
			engine.renderer.renderQuad2D(ph::Transform2D{.width = 5.0f, .height = 5.0f}.calculate());
		});


		if(engine.execute() == false){ /*do nothing (right now)*/ }
		engine.shutdown();
	};
	
};