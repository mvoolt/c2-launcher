cmake_minimum_required(VERSION 3.0)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Type of build.")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}" CACHE INTERNAL "Install target is not defined.")

project(C2-Launcher VERSION 1.0 LANGUAGES CXX RC)

if(NOT MSVC)
	message(FATAL_ERROR "MSVC is the only supported compiler!")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BUILD_64BIT TRUE)
	message(STATUS "c2-launcher: 64-bit build")
else()
	message(FATAL_ERROR "c2-launcher only supports 64-bit!")
endif()

# prevent modern MSVC from enabling ASLR
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DYNAMICBASE:NO")

option(C2LAUNCHER_GAME_LAUNCHER "Build game launcher - Crysis2.exe" ON)
if(C2LAUNCHER_GAME_LAUNCHER)
	add_executable(Crysis2 WIN32
	  Code/Launcher/Game/Main.cpp
	  Resources/GameLauncher.rc
	)
	target_include_directories(Crysis2 PRIVATE
	  Code/Launcher
	  Code/CryCommon
	  ${CMAKE_SOURCE_DIR}/vendor/subhook
	  ${CMAKE_SOURCE_DIR}/vendor/funhook
	  ${PROJECT_BINARY_DIR}
	)
	
	target_compile_definitions(Crysis2 PUBLIC SUBHOOK_STATIC)
	target_link_libraries(Crysis2 subhook)
	target_compile_definitions(Crysis2 PRIVATE BUILD_64BIT)
endif()

option(C1LAUNCHER_SERVER_LAUNCHER "Build dedicated server launcher - Crysis2DedicatedServer.exe" OFF)
if(C2LAUNCHER_SERVER_LAUNCHER)
	add_executable(Crysis2DedicatedServer WIN32
	  Code/Launcher/DedicatedServer/Main.cpp
	  Resources/DedicatedServerLauncher.rc
	)
	target_include_directories(Crysis2DedicatedServer PRIVATE
	  Code/Launcher
	  Code/CryCommon
	  ${PROJECT_BINARY_DIR}
	)

	target_compile_definitions(Crysis2DedicatedServer PRIVATE BUILD_64BIT)
endif()

option(C1LAUNCHER_STATIC_MSVC_RUNTIME "Enable static link of MSVC runtime library." ON)
if(C1LAUNCHER_STATIC_MSVC_RUNTIME)
	foreach(flag
	  CMAKE_CXX_FLAGS
	  CMAKE_CXX_FLAGS_DEBUG
	  CMAKE_CXX_FLAGS_RELEASE
	  CMAKE_CXX_FLAGS_RELWITHDEBINFO
	  CMAKE_CXX_FLAGS_MINSIZEREL)
		if(${flag} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
		endif()
	endforeach()
endif()

add_subdirectory(vendor)
configure_file(config.h.in ${PROJECT_BINARY_DIR}/config.h)
