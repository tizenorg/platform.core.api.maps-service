SET(fw_test "${fw_name}-test")

INCLUDE(FindPkgConfig)
pkg_check_modules(${fw_test} REQUIRED )
FOREACH(flag ${${fw_test}_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -Wall -Werror")

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
	# Maps Service sources

	# Maps API
	../src/api/maps_service.cpp
	../src/api/maps_preference.cpp
	../src/api/maps_plugin_info.cpp


	# Common Data Types API
	../src/api/maps_coordinates.cpp
	../src/api/maps_area.cpp
	../src/api/maps_extra_types.cpp


	# Geocode Service
	../src/api/maps_address.cpp


	# Place Service
	../src/api/maps_place.cpp
	../src/api/maps_place_contact.cpp
	../src/api/maps_place_image.cpp
	../src/api/maps_place_category.cpp
	../src/api/maps_place_filter.cpp
	../src/api/maps_place_review.cpp
	../src/api/maps_place_media.cpp
	../src/api/maps_place_attribute.cpp
	../src/api/maps_place_url.cpp
	../src/api/maps_place_rating.cpp
	../src/api/maps_place_editorial.cpp
	../src/api/maps_place_link_object.cpp
	../src/api/maps_place_category.cpp
	../src/api/maps_place_attribute.cpp
	../src/api/maps_place_contact.cpp
	../src/api/maps_place_editorial.cpp
	../src/api/maps_place_link_object.cpp
	../src/api/maps_place_media.cpp
	../src/api/maps_place_url.cpp
	../src/api/maps_place_review.cpp
	../src/api/maps_place_rating.cpp
	../src/api/maps_place_image.cpp


	# Route Service
	../src/api/maps_route.cpp
	../src/api/maps_route_maneuver.cpp
	../src/api/maps_route_segment.cpp


	# Plugin Management
	../src/plugin/discovery.cpp
	../src/plugin/module.cpp


	# Session Management
	../src/session/command.cpp
	../src/session/commands.cpp
	../src/session/command_queue.cpp
	../src/session/thread.cpp

	../src/maps_util.cpp


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

INSTALL(TARGETS capi-maps-service-test DESTINATION bin)
INSTALL(FILES maps-service-test.rule DESTINATION /etc/smack/accesses2.d/)

ADD_SUBDIRECTORY(dummy_plugin)
