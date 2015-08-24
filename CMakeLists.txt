cmake_minimum_required(VERSION 3.2)
project(LD33)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++0x" )
# for profiling
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer -gdwarf-2" )

include_directories(c:/DEV/gamedev/base/src/)
include_directories(c:/DEV/gamedev/sysent/src/)
include_directories(c:/DEV/gamedev/maths/src/)
include_directories(c:/DEV/gamedev/assets/include/)
include_directories(src/)
#geng
include_directories(c:/DEV/gamedev/geng/include/)
link_directories(c:/DEV/gamedev/geng/build/Debug/)
#glew
include_directories(c:/DEV/libs/mingw/glew-1.12.0/include/)
link_directories(c:/DEV/libs/mingw/glew-1.12.0/lib/)
#sdl
include_directories(c:/DEV/libs/mingw/SDL2-2.0.3/include/)
link_directories(c:/DEV/libs/mingw/SDL2-2.0.3/build/.libs/)
#sdl-image
include_directories(c:/DEV/libs/mingw/SDL2_image-2.0.0/)
link_directories(c:/DEV/libs/mingw/SDL2_image-2.0.0/.libs/)
#glm
include_directories(c:/DEV/libs/mingw/glm/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLM_FORCE_RADIANS" )
#freetype
include_directories(c:/DEV/libs/mingw/freetype-2.5.4/include/)
link_directories(c:/DEV/libs/mingw/freetype-2.5.4/build/)


set(LIBS "-lgeng
          -lglew32 -lopengl32 -lglu32
          -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
          -lfreetype")

set(SOURCE_FILES
        src/main.cpp
        src/MyGame.cpp
        src/MyGame.h
        src/entities/Monster.cpp
        src/entities/Monster.h
        src/MyAssets.cpp
        src/MyAssets.h
        src/Animation.cpp
        src/Animation.h
        src/Layers.h
        src/entities/Helicopter.cpp
        src/entities/Helicopter.h
        src/Helicopters.h
        src/Helicopters.cpp
        src/entities/HUD.cpp src/entities/HUD.h src/entities/Missile.cpp src/entities/Missile.h src/Missiles.h src/Missiles.cpp)
add_executable(LD33 ${SOURCE_FILES})
target_link_libraries(LD33 ${LIBS})