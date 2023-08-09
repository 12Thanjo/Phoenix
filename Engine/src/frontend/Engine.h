#pragma once

#include "../../include/Phoenix/defines.h"

#include "./EngineInterface.h"
#include "./Inputs.h"
#include "./AssetManager.h"
#include "./Renderer.h"
#include "./Window.h"

#include "common.h"

namespace ph{
	
	class Engine{
		public:
			Engine() = default;
			~Engine() = default;


			inline auto init() noexcept -> bool { return this->interface.init(); };
			PH_NODISCARD inline auto execute() noexcept -> bool { return this->interface.execute(); };
			inline auto shutdown() noexcept -> void { this->interface.shutdown(); };


			inline auto setUpdateCallback(const std::function<void()> callback) noexcept -> void {
				const static auto callback_func = callback;

				this->interface.set_update_callback([](){
					callback_func();
				});
			};

			PH_NODISCARD inline auto getFrameTime() const noexcept -> float { return this->interface.get_frame_time(); };
			

		public:
			Inputs inputs{&this->interface};
			AssetManager assets{&this->interface};
			Renderer renderer{&this->interface};
			Window window{&this->interface};
	
		private:
			EngineInterface interface{};
	};

};
