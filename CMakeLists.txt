cmake_minimum_required(VERSION 3.30)

# Vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(sdltemplate VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Dependencies (from vcpkg) ---

# other packages (all provide CMake configs)
find_package(imgui CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(lua REQUIRED)
find_package(sol2 REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

find_path(HUNSPELL_INCLUDE_DIR NAMES hunspell/hunspell.h REQUIRED)
find_library(HUNSPELL_LIBRARY 
    NAMES libhunspell-1.7 libhunspell-1.6 hunspell-1.7 hunspell-1.6 libhunspell hunspell
    REQUIRED
)


# No C++20 modules
set(CMAKE_CXX_SCAN_FOR_MODULES OFF) 

# --- Executable ---
add_executable(sdltemplate)

# Sources
file(GLOB_RECURSE sdltemplate_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
)
target_sources(sdltemplate PRIVATE ${sdltemplate_SOURCES})
#if(MSVC)
#    target_sources(sdltemplate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/icon.rc)
#elseif(MINGW)
#    enable_language(RC)
#    target_sources(sdltemplate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/icon.rc)
#endif()

# Includes
target_include_directories(sdltemplate PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
	${HUNSPELL_INCLUDE_DIR}
)

target_link_libraries(sdltemplate PRIVATE
	imgui::imgui
	OpenAL::OpenAL
	glm::glm
	nlohmann_json::nlohmann_json
	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_ttf::SDL3_ttf
    lua 
    sol2::sol2
	${HUNSPELL_LIBRARY}
)
# --- Compiler warnings ---
if(MSVC)
    target_compile_options(sdltemplate PRIVATE
        /W3
        /MP
        /WX
        /ZI
    )
else()
    target_compile_options(sdltemplate PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic
    )
endif()

# --- Visual Studio niceties ---
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sdltemplate)
set_target_properties(sdltemplate PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${sdltemplate_SOURCES})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
