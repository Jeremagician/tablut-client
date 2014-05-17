set(WARNINGS "-W -Wall -Wextra -Winit-self -Weverything -Wfatal-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")

# C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")