include(${CMAKE_CURRENT_LIST_DIR}/architecture.cmake)

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

set(OPENGL_DESKTOP_TARGET "OPENGL_2_0" CACHE STRING
  "The OpenGL Version to Target")


set(OPIFEX_NODE_VERSION "0_10" CACHE STRING
  "The NodeJS Version to Build for")

set_property(CACHE OPIFEX_OS PROPERTY STRINGS OPIFEX_WIN32 OPIFEX_WIN64 OPIFEX_LINUX32 OPIFEX_LINUX64 OPIFEX_IOS OPIFEX_ANDROID OPIFEX_OSX32 OPIFEX_OSX64)
set_property(CACHE OPENGL_DESKTOP_TARGET PROPERTY STRINGS OPENGL_2_0 OPENGL_3_3 OPENGL_ES_2)
set_property(CACHE OPIFEX_NODE_VERSION PROPERTY STRINGS 0_10 0_12)

SET(OPIFEX_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})
SET(OPIFEX_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})

if(MSVC)
	set(CMAKE_C_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1" CACHE STRING "" FORCE)
	set(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE STRING "" FORCE)

	set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1" CACHE STRING "" FORCE)
	set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE STRING "" FORCE)
endif()
