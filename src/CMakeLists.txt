include_directories(
  ${SDL2_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${stb_SOURCE_DIR}
)

set(client_sources
  main.c
  texture.c texture.h
  shared.h
  array2d.h
  font.c font.h
)

add_executable(tafl-client ${client_sources})

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