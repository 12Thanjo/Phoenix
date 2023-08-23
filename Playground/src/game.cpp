#include "game.h"


#include <Phoenix/Phoenix.h>

#include <iostream>
#include <algorithm>


namespace Game{


	struct Player{
		ph::Transform transform{};
		float speed = 7.0f;

		ph::CharacterController controller;
		float falling_speed = 0.0001f;
		bool is_grounded = false;
		int jumps_left = 2;
	};







	class GameInstance{
		public:
			GameInstance() = default;
			~GameInstance() = default;


			auto run() noexcept -> void {
				this->engine.init();


				this->camera = ph::Camera{
					.position = {0.35f + 0.5f, 0.55f, 3.2f},
					// .position = {0.35f + 0.5f, 0.55f, 5.0f},
				};

				this->player = Player{
					.transform = ph::Transform{
						.position = {-2.0f, 15.0f, 18.0f},
						.scale = {1.0f, 2.0f, 0.2f},
					},
					.controller = this->engine.physics.createCharacterController({-2.0f, 15.0f, 18.0f}, 2.0f, 0.5f),
				};


				// const ph::Mesh3D tank = this->engine.assets.loadMesh("./assets/models/tank.obj").value();

				this->setup_materials();

				this->engine.window.captureMouse();


				auto ground_collider = this->engine.physics.createStaticCube(
					glm::vec3{0.0f, -0.5f, 0.0f}, glm::vec3{20.0f, 0.5f, 20.0f}, 0.5f, 0.5f, 0.9f
				);

				auto tower1_collider = this->engine.physics.createStaticCube(
					{10.0f, 5.0f, -10.0f}, glm::vec3{1.0f, 10.0f, 1.0f}/2.0f, 0.5f, 0.5f, 0.9f
				);
				auto tower2_collider = this->engine.physics.createStaticCube(
					{-10.0f, 5.0f, -10.0f}, glm::vec3{1.0f, 10.0f, 1.0f}/2.0f, 0.5f, 0.5f, 0.9f
				);


				this->box_collider1 = this->engine.physics.createDynamicCube(
					glm::vec3{0.0f, 10.0f, 0.0f}, glm::vec3{0.5f, 0.5f, 0.5f}, 0.5f, 0.5f, 0.75f
				);
				this->engine.physics.setDynamicColliderDensity(this->box_collider1, 1.5f);

				this->box_collider2 = this->engine.physics.createDynamicCube(
					glm::vec3{0.5f, 12.0f, 0.0f}, glm::vec3{0.5f, 0.5f, 0.5f}, 0.5f, 0.5f, 0.75f
				);
				this->engine.physics.setDynamicColliderDensity(this->box_collider2, 1.5f);

				this->box_collider3 = this->engine.physics.createDynamicCube(
					glm::vec3{-0.5f, 14.0f, 0.5f}, glm::vec3{0.5f, 0.5f, 0.5f}, 0.5f, 0.5f, 0.75f
				);
				this->engine.physics.setDynamicColliderDensity(this->box_collider3, 1.5f);



				this->engine.setUpdateCallback( [&](){ this->update(); } );
				this->engine.renderer3D.setCallback( [&](){ this->render_3D(); } );
				this->engine.renderer2D.setCallback( [&](){ this->render_2D(); } );



				if(this->engine.execute() == false){ /* TODO: */ }
				this->engine.shutdown();
			};




			auto setup_materials() noexcept -> void {
				this->ground_material = this->engine.renderer3D.createMaterial().value();
				this->engine.assets.setMaterialColor(this->ground_material, {0.3f, 0.3f, 0.3f, 1.0f});
				this->engine.assets.setMaterialTexture(this->ground_material, this->engine.assets.getDefaultTexture());

				this->wall_material = this->engine.renderer3D.createMaterial().value();
				this->engine.assets.setMaterialColor(this->wall_material, {0.5f, 0.5f, 0.5f, 1.0f});
				this->engine.assets.setMaterialTexture(this->wall_material, this->engine.assets.getDefaultTexture());

				this->player_material = this->engine.renderer3D.createMaterial().value();
				this->engine.assets.setMaterialColor(this->player_material, {0.0f, 0.08f, 0.3f, 1.0f});
				this->engine.assets.setMaterialTexture(this->player_material, this->engine.assets.getDefaultTexture());

				this->crosshair_material = this->engine.renderer2D.createMaterial().value();
				this->engine.assets.setMaterialColor2D(this->crosshair_material, {1.0f, 1.0f, 1.0f, 1.0f});
				this->engine.assets.setMaterialTexture2D(this->crosshair_material, this->engine.assets.getDefaultTexture());

				this->test_font_material = this->engine.renderer2D.createMaterial().value();
				this->engine.assets.setMaterialColor2D(this->test_font_material, {1.0f, 1.0f, 1.0f, 1.0f});
				this->engine.assets.setMaterialTexture2D(this->test_font_material, {1});
			};




			auto render_map() noexcept -> void {
				
				const auto ground_transform = ph::Transform{
					.position = {0.0f, -0.5f, 0.0f},
					.scale = {50.0f, 1.0f, 50.0f},
				};

				this->engine.renderer3D.bindMaterial(this->ground_material);
				this->engine.renderer3D.drawMesh(ground_transform.calculate(), this->engine.assets.getCubeMesh());


				this->engine.renderer3D.bindMaterial(this->wall_material);

				this->engine.renderer3D.drawMesh(
					ph::Transform{.position = {10.0f, 5.0f, -10.0f}, .scale = {1.0f, 10.0f, 1.0f}}.calculate(),
					this->engine.assets.getCubeMesh()
				);


				this->engine.renderer3D.drawMesh(
					ph::Transform{.position = {-10.0f, 5.0f, -10.0f}, .scale = {1.0f, 10.0f, 1.0f}}.calculate(),
					this->engine.assets.getCubeMesh()
				);

			};





			auto update() noexcept -> void {
				     if(this->engine.inputs.isDown(ph::Inputs::Key::Escape)){ this->engine.window.releaseMouse(); }
				else if(this->engine.inputs.isDown(ph::Inputs::Mouse::Left)){ this->engine.window.captureMouse(); }


				this->player.transform.rotation.y -= this->engine.inputs.mouseDX() * this->engine.getFrameTime();
				this->player.transform.rotation.x -= this->engine.inputs.mouseDY() * this->engine.getFrameTime();

				this->player.transform.rotation.x = std::clamp(this->player.transform.rotation.x, -glm::radians(89.0f), glm::radians(89.0f));



				auto player_velocity = glm::vec3{0.0f};

				const glm::mat4 player_current_transform = this->player.transform.calculate();

				     if(this->engine.inputs.isDown(ph::Inputs::Key::W)){ player_velocity += ph::getTransformForward(player_current_transform); }
				else if(this->engine.inputs.isDown(ph::Inputs::Key::S)){ player_velocity += ph::getTransformBackward(player_current_transform); }

				     if(this->engine.inputs.isDown(ph::Inputs::Key::A)){ player_velocity += ph::getTransformLeft(player_current_transform); }
				else if(this->engine.inputs.isDown(ph::Inputs::Key::D)){ player_velocity += ph::getTransformRight(player_current_transform); }


				if(player_velocity.x != 0 || player_velocity.y != 0 || player_velocity.z != 0){
					player_velocity = glm::normalize(player_velocity) * this->player.speed * this->engine.getFrameTime();
				}


				if(this->engine.inputs.wasPressed(ph::Inputs::Key::Spacebar) && this->player.jumps_left > 0){
					player_velocity.y = 6.0f * this->engine.getFrameTime();
					this->player.jumps_left -= 1;

				}else{
					player_velocity.y = 0.5f * ((2 * this->player.falling_speed) + (-9.81f * this->engine.getFrameTime()));
					player_velocity.y *= this->engine.getFrameTime();
				}


				this->engine.physics.characterControllerMove(this->player.controller, player_velocity, this->engine.getFrameTime());
				const glm::vec3 player_new_position = this->engine.physics.getCharacterControllerPosition(this->player.controller);


				if(this->engine.physics.isCharacterControllerGrounded(this->player.controller) == false){
					this->player.falling_speed = player_velocity.y + (0.5f * -9.81f * this->engine.getFrameTime() * this->engine.getFrameTime());
					this->player.falling_speed /= this->engine.getFrameTime();
					this->player.is_grounded = false;

				}else{
					this->player.falling_speed = 0.0f;
					this->player.is_grounded = true;
					this->player.jumps_left = 2;
				}



				this->player.transform.position = player_new_position;
			};




			auto render_3D() noexcept -> void {
				this->camera.position.z = 3.2f - glm::sin(this->player.transform.rotation.x);
				this->engine.renderer3D.setCamera(
					this->camera.calculate() * ph::Camera{this->player.transform.position, this->player.transform.rotation}.calculate()
				);
				// this->engine.renderer3D.setCamera(this->player.transform.calculate() * this->camera.calculate());

				if(this->player.is_grounded){
					this->engine.renderer3D.bindMaterial(this->player_material);
				}else{
					this->engine.renderer3D.bindMaterial(this->wall_material);
				}


				const float player_rot_x = this->player.transform.rotation.x;
				this->player.transform.rotation.x = 0;
				this->engine.renderer3D.drawMesh(this->player.transform.calculate(), this->engine.assets.getCubeMesh());
				this->player.transform.rotation.x = player_rot_x;


				this->engine.renderer3D.bindMaterial(this->player_material);


				this->engine.renderer3D.drawMesh(
					this->engine.physics.getDynamicColliderTransform(this->box_collider1), this->engine.assets.getCubeMesh()
				);
				this->engine.renderer3D.drawMesh(
					this->engine.physics.getDynamicColliderTransform(this->box_collider2), this->engine.assets.getCubeMesh()
				);
				this->engine.renderer3D.drawMesh(
					this->engine.physics.getDynamicColliderTransform(this->box_collider3), this->engine.assets.getCubeMesh()
				);


				this->render_map();
			};


			auto render_2D() noexcept -> void {
				this->engine.renderer2D.setCamera(glm::mat4{1.0f});

				this->engine.renderer2D.bindMaterial(this->crosshair_material);
				this->engine.renderer2D.drawQuad(ph::Transform2D{.width = 5.0f, .height = 5.0f}.calculate());


				this->engine.renderer2D.bindMaterial(this->test_font_material);
				this->engine.renderer2D.drawQuad(ph::Transform2D{ .x = 750.0f, .width = 1000.0f, .height = 1000.0f}.calculate());
			};


	
		private:
			ph::Engine engine{};

			Player player{};
			ph::Camera camera{};

			ph::DynamicCollider box_collider1;
			ph::DynamicCollider box_collider2;
			ph::DynamicCollider box_collider3;




			ph::Material3D ground_material;
			ph::Material3D wall_material;
			ph::Material3D player_material;

			ph::Material2D crosshair_material;
			ph::Material2D test_font_material;
	};






	auto run() noexcept -> void {
		auto game_instance = GameInstance{};

		game_instance.run();
	};
	
};