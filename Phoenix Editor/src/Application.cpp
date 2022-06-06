#include <Phoenix.h>

#include "ImGui.h"




#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

namespace Phoenix{



	class Editor : public Engine{
		private:
			winID _window;
			RendererImGui _renderer_ImGui;

			FrameBuffer* _output_buffer;
	
		public:
			Editor() : _renderer_ImGui(dynamic_cast<Engine*>(this)) {};
			~Editor(){
				free(_output_buffer);
			};


			void create() override {
				// onEvent callback
				std::function<void(Event&,winID)> event_callback = [&](Event& e, winID id){
					EventType type = e.getType();

					switch(type){
						case PH_WINDOW_CLOSE_EVENT:
							exit();
							break;
						case PH_MOUSE_DOWN_EVENT:
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

				///////////////////////////////////////////////////////////////////////////////////////////////////////

				Entity box = createEntity("box");
				box.addComponent<Component::Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 1.0f));
				box.addComponent<Component::SpriteRenderer>(glm::vec4(0.0f, 0.75f, 1.0f, 0.4f));


				Entity camera = createEntity("Camera");
				camera.addComponent<Component::Transform>();
				camera.addComponent<Component::Camera>(glm::radians(65.0f), 16/9.0f, 0.1f, 100.0f);
				setCamera(camera);


				// UUID color_shader = loadShader("./assets/shaders/color.shader");
				// bindShader(color_shader);

				serialize("C:/Users/andrew/OneDrive/programming/Phoenix Projects/test");
				deserialize("C:/Users/andrew/OneDrive/programming/Phoenix Projects/test");
			}



			void render3D() override {
				_output_buffer->bind();
				clearColorDepth();
			}


			void render2D() override {
				_output_buffer->unbind();

				

				_renderer_ImGui.begin();
				_renderer_ImGui.render(_output_buffer);
				_renderer_ImGui.end();
			}
			
	};






	Engine* init(){
		// EngineConfig config;
			// config.name = "Phoenix Engine Editor";

		return new Editor();
	}
	
}
