# Phoenix Engine
![Phoenix Logo](logo.png)
Phoeninx Engine is realtime 2D/3D application creation platform written in c++ and uses OpenGL. Phoenix is an embeded engine along with a custom editor (also built upon Phoenix). At this time, Phoenix only supports Windows.



Phoenix is currently in pre-alpha.



### Installation:

Download Phoenix engine from [Github](https://github.com/12Thanjo/Phoenix).

`git clone https://github.com/12Thanjo/Phoenix`


### Building:
Phoenix has premake scripts. The default is uses Visual Studio 2022, but this can be changed. In `GENERATE_PROJECTS.bat`, you can change the default `vs2022` with any of the options found [here](https://premake.github.io/docs/Using-Premake).

Run `GENERATE_PROJECTS.bat` with your own build workflow (discussed above). In the default example, the script will create the Visual Studio 2022 solution.



### Example Usage:

```c++
#include <Phoenix.h>
#include <core/init.h>

class Game : public Phoenix::Engine{
	public:
		void create() override{

		}

		void render() override{

		}
};


Phoenix::Engine* Phoenix::initialize(){
	return new Game();
}
```



## Documentation:
Documentation for Phoenix Engine can be found [here](https://12thanjo.github.io/Phoenix/Documentation/Documentation/index.html).