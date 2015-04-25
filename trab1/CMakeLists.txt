CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(ait1)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(USE_STATIC_LIBS ON)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -fno-inline -Wno-unused-parameter")
SET(CMAKE_CXX_FLAGS_RELEASE "-O4 -ffast-math")
SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++11")

if(USE_STATIC_LIBS)
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -static-libgcc -static-libstdc++")
endif()

ADD_EXECUTABLE(ait1
src/houses.cpp
src/houses.h
src/item.cpp
src/item.h
src/items.cpp
src/items.h
src/knights.cpp
src/knights.h
src/main.cpp
src/map.cpp
src/map.h
src/painter.cpp
src/painter.h
src/player.cpp
src/player.h
src/position.h
#src/thing.cpp
src/thing.h
src/tile.cpp
src/tile.h
src/window.cpp
src/window.h
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

if(WIN32)
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -lmingw32 -lSDL2main -mconsole -mwindows")
endif()

TARGET_LINK_LIBRARIES(ait1 ${SDL2_LIBRARIES})
