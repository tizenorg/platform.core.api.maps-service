SET(fw_test "${fw_name}-test")

INCLUDE(FindPkgConfig)
pkg_check_modules(${fw_test} REQUIRED )
FOREACH(flag ${${fw_test}_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -Wall -Werror")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS} -Wall -Werror -fvisibility=default")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

INCLUDE_DIRECTORIES(
	src
	src/util
	src/api
	src/plugin
	src/session
	src/tutorial
	)

SET (TEST_SRCS

	###########################################################
	# Tutorial sources
	src/tutorial/maps_service_tutorial.cpp

	###########################################################
	# Unit test sources
	src/maps_service_test.cpp

	src/util/maps_object.cpp
	src/util/maps_test_utils.cpp

	src/plugin/discovery_test.cpp
	src/plugin/module_test.cpp

	src/session/command_test.cpp
	src/session/commands_test.cpp
	src/session/command_queue_test.cpp
	src/session/thread_test.cpp


	src/api/maps_api_test.cpp
	src/api/maps_area_test.cpp
	src/api/maps_coordinates_test.cpp
	src/api/maps_extra_types_test.cpp


	src/api/maps_address_test.cpp


	src/api/maps_place_test.cpp
	src/api/maps_place_category_test.cpp
	src/api/maps_place_attribute_test.cpp
	src/api/maps_place_contact_test.cpp
	src/api/maps_place_editorial_test.cpp
	src/api/maps_place_link_object_test.cpp
	src/api/maps_place_media_test.cpp
	src/api/maps_place_url_test.cpp
	src/api/maps_place_review_test.cpp
	src/api/maps_place_rating_test.cpp
	src/api/maps_place_image_test.cpp
	src/api/maps_place_filter_test.cpp

	src/api/maps_route_test.cpp
	src/api/maps_route_maneuver_test.cpp
	src/api/maps_route_segment_test.cpp

	src/api/maps_preference_test.cpp
	)

#aux_source_directory(src sources)
#FOREACH(src ${sources})
#	GET_FILENAME_COMPONENT(src_name ${src} NAME_WE)
#ENDFOREACH()
MESSAGE(">>>>>>>>>>>>>${TEST_SRCS}")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -Wl,--rpath=${LIBDIR}")


ADD_EXECUTABLE(${fw_test} ${TEST_SRCS})
TARGET_LINK_LIBRARIES(${fw_test} ${fw_name} ${${fw_test}_LDFLAGS})


INSTALL(TARGETS capi-maps-service-test DESTINATION /opt/usr/devel/)
INSTALL(FILES capi-maps-service-test.xml DESTINATION /usr/share/packages)

ADD_SUBDIRECTORY(dummy_plugin)
