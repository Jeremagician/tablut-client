include_directories(${SDL2_INCLUDE_DIR})

set(client_sources
    main.cpp
	window.cpp window.hpp
	tafl.cpp tafl.hpp
	SDL.cpp SDL.hpp
)

add_executable(tafl ${client_sources})

target_link_libraries(tafl ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY})