project(SBCartesianReadout)

cmake_minimum_required(VERSION 2.6.0)

set(${PROJECT_NAME}_SRCS
	${PROJECT_NAME}.m
)

IF(NOT APPLE)
include_directories(/usr/include/GNUstep)
ENDIF(NOT APPLE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

ADD_DEFINITIONS(-Wall -Wextra -Wno-unused-parameter -Werror)

ADD_DEFINITIONS(-DGNUSTEP)
ADD_SUBDIRECTORY(tests)

if (NOT SBWaveGen_SRCS)
  find_package(SBWaveGen REQUIRED)
endif (NOT SBWaveGen_SRCS)

IF(NOT APPLE)
ADD_DEFINITIONS(-fconstant-string-class=NSConstantString -fgnu-runtime)
ADD_DEFINITIONS(-DGSTHREAD -D_NATIVE_OBJC_EXCEPTIONS)
ENDIF(NOT APPLE)

# add headers to ide proj
file(GLOB_RECURSE INCS "*.h")

ADD_LIBRARY(${PROJECT_NAME} MODULE ${${PROJECT_NAME}_SRCS} ${INCS})

IF(NOT APPLE)
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/Info-gnustep.plist.in
	${CMAKE_CURRENT_BINARY_DIR}/Info-gnustep.plist
)
ELSE(NOT APPLE)
SET(EXECUTABLE_NAME ${CMAKE_SHARED_MODULE_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_MODULE_SUFFIX})
SET(PRODUCT_NAME ${PROJECT_NAME})
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
	${CMAKE_CURRENT_BINARY_DIR}/Info.plist
)
ENDIF(NOT APPLE)


IF(NOT APPLE)
SET(Objc_LIBRARIES objc gnustep-base)
ELSE(NOT APPLE)
SET(Objc_LIBRARIES "-framework Cocoa")
ENDIF(NOT APPLE)

target_link_libraries(${PROJECT_NAME} SBWaveGen ${Objc_LIBRARIES})

BREAKPAD_DUMP_SYMBOLS(${PROJECT_NAME}  $<TARGET_FILE_NAME:${PROJECT_NAME}>)

IF(NOT APPLE)
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib/SBPlugins/${PROJECT_NAME}.sbplugin)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Info-gnustep.plist DESTINATION lib/SBPlugins/${PROJECT_NAME}.sbplugin/Resources)
ELSE(NOT APPLE)
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib/SBPlugins/${PROJECT_NAME}.sbplugin/Contents/MacOS)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Info.plist DESTINATION lib/SBPlugins/${PROJECT_NAME}.sbplugin/Contents)
ENDIF(NOT APPLE)

