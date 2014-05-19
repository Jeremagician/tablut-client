add_subdirectory(tafl-lib)

include_directories(
  ${SDL2_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${stb_SOURCE_DIR}
)

set(tafl_lib_sources
  tafl-lib/packet.c tafl-lib/packet.h
  tafl-lib/struct_io.c tafl-lib/struct_io.h
)

set(client_sources
    main.cpp
	window.cpp window.hpp
	game.cpp game.hpp
	SDL.cpp SDL.hpp
	board.cpp board.hpp
	camera.cpp camera.hpp
	texture.cpp texture.hpp
	tiny_obj_loader.cpp tiny_obj_loader.hpp
	pawn_model.cpp pawn_model.hpp
	font.cpp font.hpp
	network_manager.cpp network_manager.hpp
)

add_executable(tafl-client ${client_sources} ${tafl_lib_sources})
target_link_libraries(tafl-client
  ${SDL2_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  stb
)

if(UNIX)
target_link_libraries(tafl-client m)
endif()

set_target_properties(tafl-client PROPERTIES OUTPUT_NAME tafl)
