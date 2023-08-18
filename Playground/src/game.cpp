#include "game.h"


#include <Phoenix/Phoenix.h>

#include <iostream>
#include <algorithm>


namespace Game{


	constexpr ph::Material3D ground_material = {0};
	constexpr ph::Material3D wall_material = {1};
	constexpr ph::Material3D player_material = {2};

	constexpr ph::Material2D crosshair_material = {0};



	struct Player{
		ph::Transform transform{};
		float speed = 7.0f;

		ph::CharacterController controller;
		float falling_speed = 0.0001f;
		bool is_grounded = false;
		int jumps_left = 2;
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
			// .position = {0.35f + 0.5f, 0.55f, 5.0f},
		};

		auto player = Player{
			.transform = ph::Transform{
				.position = {-2.0f, 15.0f, 18.0f},
				.scale = {1.0f, 2.0f, 0.2f},
			},
			.controller = engine.physics.createCharacterController({-2.0f, 15.0f, 18.0f}, 2.0f, 0.5f),
		};





		// const ph::MeshID tank = engine.assets.loadMesh("./assets/models/tank.obj").value();

		setup_materials(engine);

		engine.window.captureMouse();


		auto ground_collider = engine.physics.createStaticCube(glm::vec3{0.0f, -0.5f, 0.0f}, glm::vec3{20.0f, 0.5f, 20.0f}, 0.5f, 0.5f, 0.9f);

		auto tower1_collider = engine.physics.createStaticCube({10.0f, 5.0f, -10.0f}, glm::vec3{1.0f, 10.0f, 1.0f}/2.0f, 0.5f, 0.5f, 0.9f);
		auto tower2_collider = engine.physics.createStaticCube({-10.0f, 5.0f, -10.0f}, glm::vec3{1.0f, 10.0f, 1.0f}/2.0f, 0.5f, 0.5f, 0.9f);


		auto box_collider1 = engine.physics.createDynamicCube(glm::vec3{0.0f, 10.0f, 0.0f}, glm::vec3{0.5f, 0.5f, 0.5f}, 0.5f, 0.5f, 0.75f);
		engine.physics.setDynamicColliderDensity(box_collider1, 1.5f);

		auto box_collider2 = engine.physics.createDynamicCube(glm::vec3{0.5f, 12.0f, 0.0f}, glm::vec3{0.5f, 0.5f, 0.5f}, 0.5f, 0.5f, 0.75f);
		engine.physics.setDynamicColliderDensity(box_collider2, 1.5f);

		auto box_collider3 = engine.physics.createDynamicCube(glm::vec3{-0.5f, 14.0f, 0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, 0.5f, 0.5f, 0.75f);
		engine.physics.setDynamicColliderDensity(box_collider3, 1.5f);




		engine.setUpdateCallback([&](){
			     if(engine.inputs.isDown(ph::Inputs::Key::Escape)){ engine.window.releaseMouse(); }
			else if(engine.inputs.isDown(ph::Inputs::Mouse::Left)){ engine.window.captureMouse(); }


			player.transform.rotation.y -= engine.inputs.mouseDX() * engine.getFrameTime();
			player.transform.rotation.x -= engine.inputs.mouseDY() * engine.getFrameTime();

			player.transform.rotation.x = std::clamp(player.transform.rotation.x, -glm::radians(89.0f), glm::radians(89.0f));



			auto player_velocity = glm::vec3{0.0f};

			const glm::mat4 player_current_transform = player.transform.calculate();

			     if(engine.inputs.isDown(ph::Inputs::Key::W)){ player_velocity += ph::getTransformForward(player_current_transform); }
			else if(engine.inputs.isDown(ph::Inputs::Key::S)){ player_velocity += ph::getTransformBackward(player_current_transform); }

			     if(engine.inputs.isDown(ph::Inputs::Key::A)){ player_velocity += ph::getTransformLeft(player_current_transform); }
			else if(engine.inputs.isDown(ph::Inputs::Key::D)){ player_velocity += ph::getTransformRight(player_current_transform); }


			if(player_velocity.x != 0 || player_velocity.y != 0 || player_velocity.z != 0){
				player_velocity = glm::normalize(player_velocity) * player.speed * engine.getFrameTime();
			}


			if(engine.inputs.wasPressed(ph::Inputs::Key::Spacebar) && player.jumps_left > 0){
				player_velocity.y = 6.0f * engine.getFrameTime();
				player.jumps_left -= 1;

			}else{
				player_velocity.y = 0.5f * ((2 * player.falling_speed) + (-9.81f * engine.getFrameTime())) * engine.getFrameTime();
			}


			engine.physics.characterControllerMove(player.controller, player_velocity, engine.getFrameTime());
			const glm::vec3 player_new_position = engine.physics.getCharacterControllerPosition(player.controller);


			if(engine.physics.isCharacterControllerGrounded(player.controller) == false){
				player.falling_speed = (player_velocity.y + (0.5f * -9.81f * engine.getFrameTime() * engine.getFrameTime())) / engine.getFrameTime();
				player.is_grounded = false;

			}else{
				player.falling_speed = 0.0f;
				player.is_grounded = true;
				player.jumps_left = 2;
			}



			player.transform.position = player_new_position;
		});





		
		engine.renderer.setCallback3D([&](){
			camera.position.z = 3.2f - glm::sin(player.transform.rotation.x);
			engine.renderer.setCamera(camera.calculate() * ph::Camera{player.transform.position, player.transform.rotation}.calculate());
			// engine.renderer.setCamera(player.transform.calculate() * camera.calculate());

			if(player.is_grounded){
				engine.renderer.bindMaterial(player_material);
			}else{
				engine.renderer.bindMaterial(wall_material);
			}


			const float player_rot_x = player.transform.rotation.x;
			player.transform.rotation.x = 0;
			engine.renderer.drawMesh(player.transform.calculate(), engine.assets.getCubeMesh());
			player.transform.rotation.x = player_rot_x;


			engine.renderer.bindMaterial(player_material);


			engine.renderer.drawMesh(engine.physics.getDynamicColliderTransform(box_collider1), engine.assets.getCubeMesh());
			engine.renderer.drawMesh(engine.physics.getDynamicColliderTransform(box_collider2), engine.assets.getCubeMesh());
			engine.renderer.drawMesh(engine.physics.getDynamicColliderTransform(box_collider3), engine.assets.getCubeMesh());


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