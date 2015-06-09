#!/bin/bash
valgrind --leak-check=yes /usr/bin/capi-maps-service-test
#G_DEBUG=gc-friendly G_SLICE=always-malloc valgrind ./maps_test
