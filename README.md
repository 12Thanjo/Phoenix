# Phoenix Engine

Game / Render engine written in C++ and uses Vulkan.


## Building:

### Dependancies:
- Premake5
- VulkanSDK
- CMake (for PhysX)
- Phython (for PhysX)

#### Additional if on Linux (not supported yet):
- libx11-dev
- libxkbcommon-x11-dev
- libx11-xcb-dev


### Building PhysX (Windows):
- Enter directory `./extern/PhysX/physx`

- Open `buildtools/presets/public/vc17win64.xml`
	- set `PX_BUILDSNIPPETS` to `False`
	- set `PX_BUILDPVDRUNTIME` to `False`
	- set `NV_USE_STATIC_WINCRT` to `False`

- Go back to the PhysX root directory (`./extern/PhysX/physx`)
- Run the `generate_projects.bat` script

- Enter directory `./compiler/vc17win64`
	- Open `PhysXSDK.sln` in Visual Studio
	- Build the project in both `debug` and `checked` modes

- Close Visual Studio
- Delete directory `./compiler/vc17win64`

- Open `buildtools/presets/public/vc17win64.xml`
	- set `NV_USE_DEBUG_WINCRT` to `False`

- Go back to the PhysX root directory (`./extern/PhysX/physx`)
- Run the `generate_projects.bat` script

- Enter directory `./compiler/vc17win64`
	- Open `PhysXSDK.sln` in Visual Studio
	- Build the project in `release` 





## Updates:
Update notes can be found in the [changelog](https://github.com/12Thanjo/Phoenix/blob/main/CHANGELOG.md). Each version corresponds with a commit.
