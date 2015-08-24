mkdir build
cd build
mkdir Emscripten
cd ..
set EMCC_DEBUG=2


set IDIRS= -Ic:/DEV/libs/emscripten/freetype-2.5.4/include/ -Ic:/DEV/gamedev/geng/include -Isrc -Ic:/DEV/gamedev/base/src -Ic:/DEV/gamedev/maths/src -Ic:/DEV/gamedev/assets/include -Ic:/DEV/gamedev/sysent/src -Ic:/DEV/libs/mingw/glm
set FLAGS= -std=c++11 -DWEB -DGLM_FORCE_RADIANS %IDIRS% -s USE_SDL=2 -s USE_SDL_IMAGE=2
set GENG_SOURCES= c:/DEV/gamedev/geng/include/geng/graphics/Window.cpp c:/DEV/gamedev/geng/include/geng/graphics/Shader.cpp c:/DEV/gamedev/geng/include/geng/graphics/Texture2D.cpp c:/DEV/gamedev/geng/include/geng/graphics/Textures2DManager.cpp c:/DEV/gamedev/geng/include/geng/graphics/Graphics.cpp c:/DEV/gamedev/geng/include/geng/graphics/ViewPort.cpp c:/DEV/gamedev/geng/include/geng/graphics/Events.cpp c:/DEV/gamedev/geng/include/geng/physics/Physics.cpp
set LD33_SOURCES= src/entities/Helicopter.cpp src/entities/HUD.cpp src/entities/Missile.cpp src/entities/Monster.cpp src/Animation.cpp src/Helicopters.cpp src/Missiles.cpp src/MyAssets.cpp src/MyGame.cpp src/main.cpp

call emcc %GENG_SOURCES% %FLAGS% -o build/Emscripten/geng.bc
call emcc %LD33_SOURCES% %FLAGS% -o build/Emscripten/ld33.bc

call emcc build/Emscripten/geng.bc build/Emscripten/ld33.bc %FLAGS% --preload-file data  -o build/Emscripten/main.html
