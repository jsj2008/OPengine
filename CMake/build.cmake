include(CMake/architecture.cmake)
target_architecture( ARCHITECTURE )
SET(TARGET_ARCH_64 0)
if(${ARCHITECTURE} STREQUAL "x86_64")
	SET(TARGET_ARCH_64 1)
endif()

SET(OPIFEX_DEFAULT_OS "OPIFEX_LINUX32")

if(WIN32 AND TARGET_ARCH_64)
	SET(OPIFEX_DEFAULT_OS "OPIFEX_WIN64")
elseif(WIN32)
	SET(OPIFEX_DEFAULT_OS "OPIFEX_WIN32")
endif()

if(UNIX AND TARGET_ARCH_64)
	SET(OPIFEX_DEFAULT_OS "OPIFEX_LINUX64")
elseif(UNIX)
	SET(OPIFEX_DEFAULT_OS "OPIFEX_LINUX32")
endif()

if(APPLE AND TARGET_ARCH_64)
	SET(OPIFEX_DEFAULT_OS "OPIFEX_OSX64")
elseif(APPLE)
	SET(OPIFEX_DEFAULT_OS "OPIFEX_OSX32")
endif()

set(OPIFEX_OS ${OPIFEX_DEFAULT_OS} CACHE STRING
  "Operating system to build for.")

set(OPIFEX_REPOSITORY "../OPifex.Engine" CACHE STRING
  "Directory to Repository from Output Folder")
set(OPENGL_DESKTOP_TARGET "OPENGL_2_0" CACHE STRING
  "The OpenGL Version to Target")

set_property(CACHE OPIFEX_OS PROPERTY STRINGS OPIFEX_WIN32 OPIFEX_WIN64 OPIFEX_LINUX32 OPIFEX_LINUX64 OPIFEX_IOS OPIFEX_ANDROID OPIFEX_OSX32 OPIFEX_OSX64)
set_property(CACHE OPENGL_DESKTOP_TARGET PROPERTY STRINGS OPENGL_2_0 OPENGL_3_3)

option(OPIFEX_OCULUS "Build the Oculus library into the engine" OFF)
option(OPIFEX_MYO "Build the Myo library into the engine" OFF)