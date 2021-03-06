#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/
cmake_policy(SET CMP0017 NEW)

cmake_minimum_required(VERSION 3.1)

set(APP_NAME MyGame)
project (${APP_NAME})

set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/cocos2d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${COCOS2D_ROOT}/cmake/Modules/")
include(CocosBuildHelpers)


# Some macro definitions
if(WINDOWS)
  if(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS (-D_USRDLL -D_EXPORT_DLL_ -D_USEGUIDLL -D_USREXDLL -D_USRSTUDIODLL)
  else()
    ADD_DEFINITIONS (-DCC_STATIC)
  endif()

  ADD_DEFINITIONS (-DCOCOS2DXWIN32_EXPORTS -D_WINDOWS -DWIN32 -D_WIN32)
  set(PLATFORM_FOLDER win32)
elseif(MACOSX OR APPLE)
  ADD_DEFINITIONS (-DCC_TARGET_OS_MAC)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER mac)
elseif(LINUX)
  ADD_DEFINITIONS(-DLINUX)
  set(PLATFORM_FOLDER linux)
elseif(ANDROID)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER android)
else()
  message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()


# Compiler options
if(MSVC)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt /NODEFAULTLIB:libcmt")
  else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt")
  endif()
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS
                  -wd4251 -wd4244 -wd4334 -wd4005 -wd4820 -wd4710
                  -wd4514 -wd4056 -wd4996 -wd4099)
else()
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_DEFINITIONS(-DCOCOS2D_DEBUG=1)
  endif()
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
  if(CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif(MSVC)

set(PLATFORM_SPECIFIC_SRC)
set(PLATFORM_SPECIFIC_HEADERS)

if(MACOSX OR APPLE)
    set(PLATFORM_SPECIFIC_SRC
      proj.ios_mac/mac/main.cpp
    )
elseif(LINUX)
    set(PLATFORM_SPECIFIC_SRC
      proj.linux/main.cpp
    )
elseif ( WIN32 )
    set(PLATFORM_SPECIFIC_SRC
      proj.win32/main.cpp
    )
    set(PLATFORM_SPECIFIC_HEADERS
      proj.win32/main.h
      proj.win32/resource.h
    )
elseif(ANDROID)
    set(PLATFORM_SPECIFIC_SRC
      proj.android-studio/app/jni/hellocpp/main.cpp
    )
endif()

include_directories(
  /usr/local/include/GLFW
  /usr/include/GLFW
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/external
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/audio/include/
  Classes
)
if ( WIN32 )
  include_directories(
  ${COCOS2D_ROOT}/external/glfw3/include/win32
  ${COCOS2D_ROOT}/external/win32-specific/gles/include/OGLES
)
endif( WIN32 )

set(GAME_SRC
  Classes/AppDelegate.cpp
  ${PLATFORM_SPECIFIC_SRC}
  Classes/AppConsts.cpp
  Classes/Scene/SplashScene.cpp
  Classes/Scene/MainScene.cpp
  Classes/GameLogic/World.cpp
  Classes/GameLogic/Physics.cpp
  Classes/GameLogic/Collidable.cpp
  Classes/GameLogic/Player/Cannon.cpp
  Classes/GameLogic/Projectile/Projectile.cpp
  Classes/GameLogic/Projectile/CannonBall.cpp
  Classes/GameLogic/Enemy/Enemy.cpp
  Classes/GameLogic/Enemy/Balloon.cpp
  Classes/GameLogic/GameObject/Wind.cpp
  Classes/Support/StringHelper.cpp)

set(GAME_HEADERS
  Classes/AppDelegate.h
  ${PLATFORM_SPECIFIC_HEADERS}
  Classes/AppConsts.h
  Classes/Scene/SplashScene.h
  Classes/Scene/MainScene.h
  Classes/GameLogic/World.h
  Classes/GameLogic/Physics.h
  Classes/GameLogic/Collidable.h
  Classes/GameLogic/CollisionManager.h
  Classes/GameLogic/Player/Player.h
  Classes/GameLogic/Player/Cannon.h
  Classes/GameLogic/Projectile/Projectile.h
  Classes/GameLogic/Projectile/CannonBall.h
  Classes/GameLogic/Enemy/Enemy.h
  Classes/GameLogic/Enemy/Balloon.h
  Classes/GameLogic/GameObject/Wind.h
  Classes/Support/StringHelper.h)


# Configure libcocos2d:
# Uncomment to disable/enable compilation and linking of the following features:
#set(USE_CHIPMUNK OFF CACHE BOOL "Use chipmunk for physics library")
#set(USE_BOX2D ON CACHE BOOL "Use box2d for physics library")
#set(USE_BULLET OFF CACHE BOOL "Use bullet for physics3d library")
#set(USE_RECAST OFF CACHE BOOL "Use Recast for navigation mesh")
#set(USE_WEBP OFF CACHE BOOL "Use WebP codec")
#set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
#set(DEBUG_MODE OFF CACHE BOOL "Debug or release?")
#set(BUILD_EXTENSIONS OFF CACHE BOOL "Build extension library")
#set(BUILD_EDITOR_SPINE OFF CACHE BOOL "Build editor support for spine")
#set(BUILD_EDITOR_COCOSTUDIO OFF CACHE BOOL "Build editor support for cocostudio")
#set(BUILD_EDITOR_COCOSBUILDER OFF CACHE BOOL "Build editor support for cocosbuilder")
#set(BUILD_BOX2D OFF CACHE BOOL "Build box2d external without using it for physics library")
#set(USE_PREBUILT_LIBS OFF CACHE BOOL "Use prebuilt libraries in external directory") # Note: there's no support for this in Linux
#set(USE_SOURCES_EXTERNAL OFF CACHE BOOL "Use sources in external directory (automatically ON when USE_PREBUILT_LIBS is)") # Note: there's no support for this in Linux
set(BUILD_CPP_EMPTY_TEST OFF CACHE BOOL "turn off build cpp-empty-test")
set(BUILD_CPP_TESTS OFF CACHE BOOL "turn off build cpp-tests")
set(BUILD_LUA_LIBS OFF CACHE BOOL "turn off build lua related targets")
set(BUILD_JS_LIBS OFF CACHE BOOL "turn off build js related targets")
add_subdirectory(${COCOS2D_ROOT})


# MyGame
if( ANDROID )
    add_library(${APP_NAME} SHARED ${GAME_SRC} ${GAME_HEADERS})
    IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
        ADD_CUSTOM_COMMAND(TARGET ${APP_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} lib${APP_NAME}.so)
    ENDIF()
else()
    add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})
endif()

target_link_libraries(${APP_NAME} cocos2d)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")

if ( WIN32 )
  #also copying dlls to binary directory for the executable to run
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt/glew32.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt/zlib1.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	)
elseif( ANDROID )

else()
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    )

endif()
