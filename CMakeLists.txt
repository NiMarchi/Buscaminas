cmake_minimum_required(VERSION 3.31)
project(Buscaminas C)

set(CMAKE_C_STANDARD 17)

# Forzar que sea aplicación consola (usa main en lugar de WinMain)
set(CMAKE_WIN32_EXECUTABLE OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")

# Paths SDL2
set(SDL2_PATH "C:/Users/nicom/CLionProjects/Buscaminas/SDL2-2.0.22/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "C:/Users/nicom/CLionProjects/Buscaminas/SDL2_ttf-2.24.0/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "C:/Users/nicom/CLionProjects/Buscaminas/SDL2_image-2.8.8/x86_64-w64-mingw32")
set(SDL2_MIXER_PATH "C:/Users/nicom/CLionProjects/Buscaminas/SDL2_mixer-2.8.1/x86_64-w64-mingw32")

# Librerías SDL2
set(SDL2_LIBRARY "${SDL2_PATH}/lib/libSDL2.dll.a")
set(SDL2_INCLUDE_DIR "${SDL2_PATH}/include/SDL2")

# Librerías SDL2_ttf
set(SDL2_TTF_LIBRARY "${SDL2_TTF_PATH}/lib/libSDL2_ttf.dll.a")
set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_PATH}/include/SDL2")

# Librerías SDL2_image
set(SDL2_IMAGE_LIBRARY "${SDL2_IMAGE_PATH}/lib/libSDL2_image.dll.a")
set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_PATH}/include/SDL2")

# Librerías SDL2_mixer
set(SDL2_MIXER_LIBRARY "${SDL2_MIXER_PATH}/lib/libSDL2_mixer.dll.a")
set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_PATH}/include/SDL2")

# Incluir headers
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})

# Ejecutable
add_executable(Buscaminas main.c constantes.h campo.c campo.h impresion.c impresion.h juego.c juego.h menu.c menu.h variables.c variables.h ventana.c ventana.h)

# Linkear librerías SDL2 y forzar consola
target_link_libraries(Buscaminas -mconsole ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY})