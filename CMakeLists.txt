cmake_minimum_required(VERSION 3.8)

project(os-metrics)

# The version number.
set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)

IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING 
        "Choose the type of build, options are: 
        None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) 
        Debug Release RelWithDebInfo MinSizeRel." )
ELSE()
   SET(CMAKE_BUILD_TYPE Debug CACHE STRING 
        "Choose the type of build, options are: 
        None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) 
        Debug Release RelWithDebInfo MinSizeRel." )
ENDIF()

# target name
set(TARGETNAME ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}_${PROJECT_VERSION_PATCH})

set(PLATFORM "unknown")
set(libs "")

# OS dependent
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0")
  set(PLATFORM "darwin")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(PLATFORM "win64")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  set(PLATFORM "freebsd")
  set(libs "util")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(PLATFORM "linux")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

add_definitions( -DPLATFORM=${PLATFORM} )

add_definitions( -Wall -Werror )

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library( ${TARGETNAME}
  src/os-metrics.c
)

add_executable( test_${PLATFORM}_${CMAKE_BUILD_TYPE}
  src/test.c
)

target_link_libraries( test_${PLATFORM}_${CMAKE_BUILD_TYPE}
  ${TARGETNAME}
  ${libs}
)

# cpack
set(CPACK_)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
install(TARGETS test_${PLATFORM}_${CMAKE_BUILD_TYPE} RUNTIME DESTINATION bin)
install(TARGETS ${TARGETNAME} RUNTIME DESTINATION lib)
install(FILES src/os-metrics.h DESTINATION include)
include(CPack)
