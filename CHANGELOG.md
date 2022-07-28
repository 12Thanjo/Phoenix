# Phoenix Engine Changelog

### 0.4.17
- fixed a bunch of compiler warnings	
- improved quality of lighting in 3d shader
- fixed bug where sprites weren't showing

### 0.4.14
- fixed 3D shader in runtime
- added opening and closing of panels (required changing ImGui internal code)

### 0.4.12
- backface culling
- added point light strength

### 0.4.10
- very tiny performance improvement to Basic3D shader

### 0.4.9
- improved `CHANGELOG`

### 0.4.8
- optimizied 3D renderer
- serialization/deserialization of sprite textures
- added card to cameras (in editor)
- updated `GITIGNORE`
- fixed premake build
- streamlined serialization code
- fixed bug causing release editor to sometimes crash when loading a new scene
- prevent crash when using editor tool when object doesn't have transform

## v0.4
- point lights
- cards

### 0.3.8
- 2D textures
- optimizied 2D and 3D renderers
- release build no longer brings up console
- fixed 2D editor controls
- renamed SpriteRenderer to Sprite
- improved PerformancePanel

### 0.3.2
- fixed bug adding scripts in editor
- cleaned up some code

## v0.3
- completely overhauled scripting
	- better performance overall
	- improved ease of use
	- extended API
	- wrote actual documentation

### 0.2.22
- allowed for editor camera movement with spacebar in addtion to the middle mouse button
- Pressing `Escape` while using an editor tool no longer diselects the entity
- created config file in appdata
- uses config file to automatically open file explorer to last opened project (when opening project)

### 0.2.18
- `Project`, `View`, and `Help` tabs in the menu bar
- `Commands and Controls` panel, shown in the `Help` tab in the menu bar
- UI show some descriptions on hover over
- SaveAs now automatically opens to the scenes folder

### 0.2.14
- when entity selected, click `G` (grab), `R` (rotate) or `S` (scale) to adjust component with mouse
- Press `X`, `Y`, or `Z` to delimit mouse control to that axis (adding `SHIFT` deliminates the axis)
- Press Numpad `7`, `9`, `1`, or `3` to snap camera to specific direction (can be found in the the `View` tab of the menu bar)

### 0.2.11
- mouse left click entity in editor viewport to select (clicking nothing unselects)

### 0.2.10
- fixed bug causing crash when changing texture type to image texture
- fixed reset values in Scene panel

### 0.2.8
- fixed readibility of `CHANGELOG`

### 0.2.7
- improved usability of UI collumns
- more unified UI formatting
- made Component::Cube and Component::Plane children of Component::Material
- removed "Shinyness not implemented" warning
- fixed startup popup (had incorrect title)
- fix UUID showing and hiding when resizing Entity Properties Panel
- removed separate holding of UUIDs in asset manager

## v0.2
- textures
- added dropdown UI
- improved color picking UI

### 0.1.11
- removed logging to console when the editor checks if the runtime is running
- tidied up the code in the scene serializer
- added shinyness
- added materials (backend, not frontend yet)
- updated UI for materials
- added clamping to UI drag-float

### 0.1.5
- fixed README

### 0.1.4
- added support for planes
- made checking for runtime only when it is open (smoother feel in editor)
- started CHANGELOG
- removed debug messages from jxcore (meant to remove these in last `GITIGNORE`