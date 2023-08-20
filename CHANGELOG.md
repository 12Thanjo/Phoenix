# Phoenix Engine Change Log

### v0.2.0
- Added foundation for font loading / generation
- organized projects in premake build

### v0.1.9
- added asset loaders
	- images
	- meshes
- fixed PhysX error when shutting down

### v0.1.8
- fixed basic 3D lighting not dealing with model orientation properly

### v0.1.7
- changed type conversion of basic types from `static_cast` (for example, `static_cast<int>(num)` to `int(num)`)

### v0.1.6
- added new Inputs functions
	- wasPressed
	- wasReleased

### v0.1.5
- fixed camera transform calculation

### v0.1.4
- Added physics character controllers
- Removed glm aliasing over dynamic library boundary
- Added error checking in two places
	- engine resize
	- Physics init

### v0.1.3
- Exposed physics API

### v0.1.2
- Added backend implementation of PhysX rigid bodies
	- Static
	- Dynamic

### v0.1.1
- Added PhysX

### v0.1.0
- initial basic framework created

### v0.0.0
- init