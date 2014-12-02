cmake_minimum_required(VERSION 2.8)
project(tank)

include_directories(include)

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -std=c++11 $(CMAKE_CXX_FLAGS)")

set(component_files "")
set(system_files "")

foreach(component
	MouseControls
	bounding_circle
	expires
	gun
	health
	render_properties
	tank_controls
	texture_handle
	transform
	velocity
	projectile
	UserInterface
)
	list(APPEND component_files include/components/${component}.h src/components/${component}.cpp)
endforeach(component)

foreach(system
	expiring_system
	input_system
	physics_system
	render_system
	texture_manager
)
	list(APPEND system_files include/systems/${system}.h src/systems/${system}.cpp)
endforeach(system)


add_executable(tank 
	include/core/environment.h
	${component_files}
	${system_files}

	src/application.cpp
	include/application.h
	src/main.cpp
)

target_link_libraries(tank sfml-graphics sfml-window sfml-system)
