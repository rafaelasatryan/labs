# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LABA7_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LABA7_autogen.dir\\ParseCache.txt"
  "LABA7_autogen"
  )
endif()
