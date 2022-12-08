cmake_minimum_required(VERSION 3.10)

set(GAME tsl)

project(${GAME}
  VERSION 0.1.0
)

include(GNUInstallDirs)

find_package(gf REQUIRED COMPONENTS graphics)

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

# set(GAME_DATADIR "${CMAKE_INSTALL_PREFIX}/share/games/${GAME}" CACHE PATH "Game Data Dir")
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(client
	client/client.cpp
)

add_executable(server
	server/server.cpp
)

target_link_libraries(client
	PRIVATE
		gf::graphics
		gf::net
)

target_link_libraries(server
	PRIVATE
		gf::net
)

install(
  TARGETS server client
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# tools

# add_executable(tiles code/tiles.cc)
# target_link_libraries(tiles gf::core)