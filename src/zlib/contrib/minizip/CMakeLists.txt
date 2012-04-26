project(zlibwapi)

cmake_minimum_required(VERSION 2.8)

OPTION(BUILD_SHARED "Build shared library." OFF)
OPTION(BUILD_TARGET64 "Build for 64 bit target if set to ON or 32 bit if set to OFF." OFF)

IF(BUILD_SHARED)
  SET(LIBRARY_TYPE SHARED)
ELSE(BUILD_SHARED)
  SET(LIBRARY_TYPE STATIC)
ENDIF(BUILD_SHARED)

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

add_definitions(-DUNICODE -D_UNICODE)
if (WIN32)
   add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS -DZLIB_WINAPI)
endif()
#add_definitions(-DASMV -DASMINF)

# default build types are None, Debug, Release, RelWithDebInfo and MinRelSize
IF (DEFINED CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
ELSE(CMAKE_BUILD_TYPE)
  SET (CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
ENDIF (DEFINED CMAKE_BUILD_TYPE)

INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}"
	  "${PROJECT_SOURCE_DIR}/../../"
)

#ENABLE_LANGUAGE(ASM)

#set_property ( SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../masmx86/inffas32.asm PROPERTY LANGUAGE ASM )
#set_property ( SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../masmx86/match686.asm PROPERTY LANGUAGE ASM )

set(libSrcs 
        ${CMAKE_CURRENT_SOURCE_DIR}/ioapi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/unzip.c
        ${CMAKE_CURRENT_SOURCE_DIR}/zip.c
        ${CMAKE_CURRENT_SOURCE_DIR}/mztools.c
)

if (WIN32)
  set(libSrcs
	${libSrcs}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../adler32.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../compress.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../crc32.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../deflate.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../gzclose.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../gzlib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../gzread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../gzwrite.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../infback.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inffast.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inflate.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inftrees.c
	${CMAKE_CURRENT_SOURCE_DIR}/../../minigzip.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../trees.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../uncompr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../zutil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/iowin32.c
  )
else()
#  set(libSrcs
#	${libSrcs}
#	${CMAKE_CURRENT_SOURCE_DIR}/minizip.c
#	${CMAKE_CURRENT_SOURCE_DIR}/miniunz.c
#  )
endif()

#if (WIN32)
#   MESSAGE(STATUS "using win32asm")
#   set(libSrcs
#         ${libSrcs}
#         ${CMAKE_CURRENT_SOURCE_DIR}/../masmx86/inffas32.asm
#         ${CMAKE_CURRENT_SOURCE_DIR}/../masmx86/match686.asm
#   )
#elseif (WIN64)
#   MESSAGE(STATUS "using win64asm")
#   set(libSrcs
#         ${libSrcs}
#         ${CMAKE_CURRENT_SOURCE_DIR}/../masmx64/inffasx64.asm
#         ${CMAKE_CURRENT_SOURCE_DIR}/../masmx64/gvmat64.asm         
#   )
#endif ()

add_library(zlibwapi ${LIBRARY_TYPE} ${libSrcs})

if (DEFINED MSVC_VERSION)
  set_target_properties(zlibwapi PROPERTIES COMPILE_FLAGS "/Zc:wchar_t")
endif ()
