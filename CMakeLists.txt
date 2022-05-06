
cmake_minimum_required(VERSION 3.8)

project(WSMan C)

if(WIN32)
	set(C_FLAGS "")
	set(C_FLAGS "${C_FLAGS} -D_UNICODE")
	set(C_FLAGS "${C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	set(C_FLAGS "${C_FLAGS} -DWIN32_LEAN_AND_MEAN")
	set(C_FLAGS "${C_FLAGS} -D_WINSOCK_DEPRECATED_NO_WARNINGS")
	set(C_FLAGS "${C_FLAGS} -DWINVER=0x0602 -D_WIN32_WINNT=0x0602")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS}")
endif()

if(WIN32)
    set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")
    set(DETOURS_ROOT_DIR "${DEPENDENCIES_DIR}/detours")

    include_directories("${DETOURS_ROOT_DIR}/include")

    add_library(detours STATIC IMPORTED)

    if(CMAKE_GENERATOR_PLATFORM MATCHES "ARM64")
        set(MSVC_PLATFORM_TARGET "arm64")
    elseif(CMAKE_GENERATOR_PLATFORM MATCHES "Win32")
        set(MSVC_PLATFORM_TARGET "x86")
    else()
        set(MSVC_PLATFORM_TARGET "x64")
    endif()

    message(STATUS "MSVC_PLATFORM_TARGET: ${MSVC_PLATFORM_TARGET}")

    set_property(TARGET detours PROPERTY
        IMPORTED_LOCATION "${DETOURS_ROOT_DIR}/lib/${MSVC_PLATFORM_TARGET}/Release/detours.lib")
    set_property(TARGET detours PROPERTY
        IMPORTED_LOCATION_DEBUG "${DETOURS_ROOT_DIR}/lib/${MSVC_PLATFORM_TARGET}/Debug/detours.lib")
endif()

set(WSMAN_SOURCES
    Environment.c
    Environment.h
    Logger.c
    Logger.h
    WSMan.c
    WSMan.h)

if(WIN32)
    list(APPEND WSMAN_SOURCES
        ApiHooks.c
        ApiHooks.h)
endif()

add_library(wsman SHARED
    ${WSMAN_SOURCES})

set_target_properties(wsman PROPERTIES OUTPUT_NAME "WsmSvc")

if(WIN32)
    target_link_libraries(wsman detours)
endif()
