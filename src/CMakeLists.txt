include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

set(client_sources
    main.cpp
	window.cpp window.hpp
	game.cpp game.hpp
	SDL.cpp SDL.hpp
	board.cpp board.hpp
	camera.cpp camera.hpp
)

add_executable(tafl-client ${client_sources})
target_link_libraries(tafl-client ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
set_target_properties(tafl-client PROPERTIES OUTPUT_NAME tafl)