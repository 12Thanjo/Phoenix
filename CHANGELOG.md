# Changelog

### 0.2.10
- fixed bug causing crash when changing texture type to image texture
- fixed reset values in Scene panel

### 0.2.8
- fixed readibility of CHANGELOG

### 0.2.7
- textures
- added dropdown UI
- improved color picking UI
<br/><br/>
- improved usability of UI collumns
- more unified UI formatting
- made Component::Cube and Component::Plane children of Component::Material
- removed "Shinyness not implemented" warning
- fixed startup popup (had incorrect title)
- fix UUID showing and hiding when resizing Entity Properties Panel
- removed separate holding of UUIDs in asset manager

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
- started changelog
- removed debug messages from jxcore (meant to remove these in last update)