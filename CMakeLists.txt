CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(fw_name "capi-maps-service")

PROJECT(${fw_name})

SET(CMAKE_INSTALL_PREFIX /usr)
SET(PREFIX ${CMAKE_INSTALL_PREFIX})

SET(INC_DIR include)
SET(INTERNAL_INC_DIR
	src
	src/api
	src/plugin
	src/session
	src/view
	)
INCLUDE_DIRECTORIES(${INC_DIR} ${INTERNAL_INC_DIR})

SET(dependents
	# Maps API dependencies
	glib-2.0
	gmodule-2.0
	capi-base-common
	dlog

	# Mapping API dependencies
	eina
	evas
	ecore
	cairo
	elementary
	capi-media-image-util
	)
SET(pc_dependents "capi-base-common")

INCLUDE(FindPkgConfig)
pkg_check_modules(${fw_name} REQUIRED ${dependents})
FOREACH(flag ${${fw_name}_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIC -Wall -Werror -g -fdump-rtl-expand")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIC -Wall -Werror -fvisibility=hidden -fvisibility-inlines-hidden")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS} -fPIC -Wall -Werror -g -fPIC -fdump-rtl-expand -std=c++0x")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS} -fPIC -Wall -Werror -fvisibility=hidden")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

IF("${ARCH}" STREQUAL "arm")
	ADD_DEFINITIONS("-DTARGET")
ENDIF("${ARCH}" STREQUAL "arm")

ADD_DEFINITIONS("-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
ADD_DEFINITIONS("-DEXPORT_API=__attribute__((visibility(\"default\")))")
#ADD_DEFINITIONS("-DTIZEN_DEBUG")

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined -Wl,--as-needed")

aux_source_directory(src SOURCES)
aux_source_directory(src/api SOURCES_API)
aux_source_directory(src/plugin SOURCES_PLUGIN)
aux_source_directory(src/session SOURCES_SESSION)
aux_source_directory(src/view SOURCES_SESSION)
ADD_LIBRARY(${fw_name} SHARED ${SOURCES} ${SOURCES_API} ${SOURCES_PLUGIN} ${SOURCES_SESSION})

TARGET_LINK_LIBRARIES(${fw_name} ${${fw_name}_LDFLAGS})

SET_TARGET_PROPERTIES(${fw_name}
	PROPERTIES
	VERSION ${FULLVER}
	SOVERSION ${MAJORVER}
	CLEAN_DIRECT_OUTPUT 1
	)

INSTALL(TARGETS ${fw_name} DESTINATION ${LIBDIR})
INSTALL(
	DIRECTORY ${INC_DIR}/ DESTINATION include/maps
	FILES_MATCHING
	# *_plugin.h" are needed by plugin
	#        PATTERN "*_plugin.h" EXCLUDE
	PATTERN "${INC_DIR}/*.h"
	)

SET(PC_NAME ${fw_name})
SET(PC_DESCRIPTION "Tizen Maps Library")
SET(PC_REQUIRED ${pc_dependents})
SET(PC_LDFLAGS -l${fw_name})
SET(PC_INCLUDE /usr/include/maps)

CONFIGURE_FILE(
	${fw_name}.pc.in
	${CMAKE_CURRENT_SOURCE_DIR}/${fw_name}.pc
	@ONLY
	)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${fw_name}.pc DESTINATION ${LIBDIR}/pkgconfig)
