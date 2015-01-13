cmake_minimum_required(VERSION 2.8)
project(tank)

include_directories(include)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -std=c++11")

set(component_files "")
set(manager_files "")
set(system_files "")

foreach(component
	expires
	gun
	health
	tank_controls
	transform
	velocity
	projectile
	UserInterface
	GUIObj
	MouseControls
	sprite
	texture
	VertexArray
	Tilemap
	ViewController
	Label
	solid
	bounding_box
	KeyControls
)
	list(APPEND component_files include/components/${component}.h src/components/${component}.cpp)
endforeach(component)

foreach(manager
	texture_manager
	entity_manager
	map_loader
	InputControls_Mgr
	UI_Manager
	CPU_Manager
	HUD_Manager
)
	list(APPEND manager_files include/managers/${manager}.h src/managers/${manager}.cpp)
endforeach(manager)

foreach(system
	expiring_system
	input_system
	physics_system
	render_system
	UI_System
	view_system	
	damage_system
	HUD_System
)
	list(APPEND system_files include/systems/${system}.h src/systems/${system}.cpp)
endforeach(system)


add_executable(tank 
	include/core/environment.h
	include/events.h
	include/math/vector.h
	${component_files}
	${system_files}
	${manager_files}

	src/application.cpp
	include/application.h
	src/main.cpp
)

set(SFML2_LOCATION "~/Documents/My Libraries/SFML-2.2")
find_package(SFML2)

include_directories(${SFML2_INCLUDE_DIRS})

target_link_libraries(tank ${SFML2_LIBRARIES})
