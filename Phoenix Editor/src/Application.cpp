#include <Phoenix.h>

#include "ImGui.h"




#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace Phoenix{



	class Editor : public Engine{
		public:
			Editor() : _renderer_ImGui(dynamic_cast<Engine*>(this)) {};
			~Editor(){
				delete _output_buffer;
			};


			void create() override {
				// onEvent callback
				std::function<void(Event&,winID)> event_callback = [&](Event& e, winID id){
					EventType type = e.getType();


					bool ctrl_down = keyDown(id, PH_KEY_LEFT_CONTROL) || keyDown(id, PH_KEY_RIGHT_CONTROL);
					bool shift_down = keyDown(id, PH_KEY_LEFT_SHIFT) || keyDown(id, PH_KEY_RIGHT_SHIFT);

					switch(type){
						case PH_WINDOW_CLOSE_EVENT:
							exit();
							break;
						case PH_KEY_DOWN_EVENT:
							switch(static_cast<KeyDownEvent&>(e).getKeycode()){
								case PH_KEY_O:
									if(ctrl_down){
										_renderer_ImGui.open(id);
									}
									break;
								case PH_KEY_S:
									if(ctrl_down && shift_down){
										_renderer_ImGui.save_as(id);
									}else if(ctrl_down){
										_renderer_ImGui.save();
									}
									break;
								case PH_KEY_N:
									if(ctrl_down){
										_renderer_ImGui.newScene();
									}
									break;
							};
							break;
						case PH_MOUSE_MOVE_EVENT:
							if(_camera){
								float current_mouse_x = mouseX(id);
								float current_mouse_y = mouseY(id);
								if(_mouse_x != current_mouse_x || _mouse_y != current_mouse_y){
									if(mouseButtonDown(id, PH_MOUSE_MIDDLE)){
										float dx = current_mouse_x - _mouse_x;
										float dy = current_mouse_y - _mouse_y;

										if(shift_down){
											pan_camera(dx, dy);
										}else if(ctrl_down){
											zoom_camera(dx, dy);
										}else{
											rotate_camera(dx, dy);
										}
									}

									_mouse_x = current_mouse_x;
									_mouse_y = current_mouse_y;
								}
							}
							break;
					}
				};


				WindowConfig config;
					config.width = WINDOW_WIDTH;
					config.height = WINDOW_HEIGHT;
					config.name = "Phoenix Engine Editor";
					config.eventCallback = event_callback;
				_window = createWindow(config);



				///////////////////////////////////////////////////////////////////////////////////////////////////////



				FrameBufferConfig output_config;
					output_config.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };
					output_config.width = WINDOW_WIDTH;
					output_config.height = WINDOW_HEIGHT;
					// output_config.samples = 4;
				_output_buffer = new FrameBuffer(output_config);




				_renderer_ImGui.init(_window);
			}



			void render3D() override {
				_output_buffer->bind();
				clearColorDepth();
			}


			void render2D() override {
				_output_buffer->unbind();

				
				_renderer_ImGui.begin();
				_renderer_ImGui.render(_output_buffer, _window);
				_renderer_ImGui.end();
			}



			void setEditorCamera(const float& x, const float& y, const float& z){
				_camera_rho = sqrt(x*x + y*y + z*z);
				_camera_theta = acos(z / _camera_rho);
				_camera_phi = atan(y / x);
			}

		private:
			void pan_camera(float dx, float dy){
				auto& rotation = _camera.getComponent<Component::Transform>().rotation;
				float ammount = _camera_rho * 0.0015f;


				_camera_focal_point.z -= dx * ammount * cos(rotation.y);
				_camera_focal_point.x += dx * ammount * sin(rotation.y);

				_camera_focal_point.y += dy * ammount * cos(rotation.z);
				_camera_focal_point.x -= dy * ammount * sin(rotation.z) * cos(rotation.y);
				_camera_focal_point.z -= dy * ammount * sin(rotation.z) * sin(rotation.y);



				place_camera();
			}

			void rotate_camera(float dx, float dy){
				_camera_phi += dx / 146;
				_camera_theta -= dy / 146;

				place_camera();
			}

			void zoom_camera(float dx, float dy){
				_camera_rho *= (dy/400) + 1;
				place_camera();
			}

			void place_camera(){
				auto& transform = _camera.getComponent<Component::Transform>();

				if(_camera_theta < 0.01f){
					_camera_theta = 0.01f;
				}else if(_camera_theta > 3.14f - 0.01f){
					_camera_theta = 3.14f - 0.01f;
				};


				transform.translation.x = _camera_rho * sin(_camera_theta) * cos(_camera_phi) + _camera_focal_point.x;
				transform.translation.z = _camera_rho * sin(_camera_theta) * sin(_camera_phi) + _camera_focal_point.z;
				transform.translation.y = _camera_rho * cos(_camera_theta) + _camera_focal_point.y;

				transform.rotation.z = _camera_theta - (3.14f/2);
				transform.rotation.y = _camera_phi - 3.14f;
			}

		private:
			winID _window;
			RendererImGui _renderer_ImGui;

			FrameBuffer* _output_buffer;

			float _mouse_x = 0;
			float _mouse_y = 0;
		
			glm::vec3 _camera_focal_point{0.0f};
			float _camera_rho = 1.0f;
			float _camera_theta = 3.14f/2;
			float _camera_phi = 3.14f/4;
	};






	Engine* init(){
		// EngineConfig config;
			// config.name = "Phoenix Engine Editor";

		return new Editor();
	}
	
}
