![Phoenix Logo](logo.png)
# Phoenix
Phoeninx Engine is realtime 2D/3D application creation platform written in c++ and uses OpenGL. Phoenix is an embeded engine allong with a custom editor (also built upon Phoenix). At this time, Phoenix only supports Windows, but more platforms will hopefully added in the future. In addition, DirectX12, Vulkan, etc. support may be added as well.



Phoenix is currently in pre-alpha.



### Installation:

Download Phoenix engine from [Github](https://github.com/12Thanjo/Phoenix).

`git clone https://github.com/12Thanjo/Phoenix`


### Building:
Phoenix has premake scripts. The default is uses Visual Studio 2022, but this can be changed. In `GENERATE_PROJECTS.bat`, you can change the default `vs2022` with any of the options found [here](https://premake.github.io/docs/Using-Premake).

Run `GENERATE_PROJECTS.bat` and your own build workflow. In the default example, the script will create the Visual Studio 2022 solution.



### Example Usage:

```c++
#include <Phoenix.h>

class Game : public Phoenix::Application{
	private:

	public:
		Game(){}	
		~Game(){}
};


Phoenix::Application* Phoenix::initialize(){
	return new Game();
}
```



## Documentation:
Documentation for Phoenix Engine can be found [here](https://12thanjo.github.io/Phoenix/Documentation/Documentation/index.html).