cmake_minimum_required (VERSION 2.6)
project (conway)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -W -Wall -Wextra")
add_definitions (-D_XOPEN_SOURCE=700 -D_BSD_SOURCE -D_GNU_SOURCE)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
else (CMAKE_BUILD_TYPE MATCHES Debug)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

add_executable (conway main.c)
